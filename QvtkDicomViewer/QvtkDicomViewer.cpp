#include "myVtkInteractorStyleImage.h"
#include "vtkBiDimensionalCallback.h"
#include "DicomDir.h"

#include "QvtkDicomViewer.h"
#include <QMessageBox>
#include <qDebug>
#include <QComboBox>
#include <QStandardItem>
#include <QTableView>

#include <vtkActor2D.h>
#include <vtkTextMapper.h>
#include <vtkTextProperty.h>
#include <vtkDistanceWidget.h>
#include <vtkDistanceRepresentation.h>
#include <vtkAngleWidget.h>
#include <vtkImageChangeInformation.h>
#include <vtkProperty.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include "vtkImageData.h"
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkMath.h>
#include <vtkCellArray.h>
#include <vtkImageIterator.h>
#include <vtkStringArray.h>
#include "vtkVolumeRayCastCompositeFunction.h"
#include "vtkVolumeProperty.h"
#include "vtkVolume.h"
#include "vtkVolumeRayCastMapper.h"

#include <dcmtk\config\osconfig.h>
#include <dcmtk\dcmdata\dctk.h>

#include "SlicePlayer.h"
#include "DicomDataBase.h"
#include <QFileSystemModel>
#include "TreeItem.h"

void functest()
{
	qDebug()<< QStringLiteral("���óɹ�!");
}
/*
 * ���췽��
 */
QvtkDicomViewer::QvtkDicomViewer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.action_Pointer->setChecked(true);
	CursorType = CURSOR::POINTRE;
	//��ع�����͵��޸�
	connect(this, SIGNAL(CursorValueChanged()), this, SLOT(OnChangeCursorValue()));
	//����������һ�������б�
	QComboBox* _Combobox = new QComboBox();
	_Combobox->addItem(QStringLiteral("����"));
	_Combobox->addItem(QStringLiteral("����"));
	_Combobox->addItem(QStringLiteral("�����"));
	//��ӵ��˵���
	ui.mainToolBar->addSeparator();
	ui.mainToolBar->addWidget(_Combobox);
	ui.mainToolBar->addSeparator();
	//�Զ����ʼ��
	ui.action_SwitchOfProperty->setChecked(true);
	ui.dockWidget_Dir->setHidden(false);
	icon_Play.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/play_128px_1197036_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
	icon_Pause.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/pause_128px_1197034_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
	PlayFlag = false;
	ui.action_Stop->setEnabled(false);
	//�����Ĳ˵�
	CreateContextMenu();
	//DirTreeRefresh(NULL);
}
/*
 * ��Ӧ���ֵ���޸�,ִ��һЩˢ�ºͽ��ò���
 */
void QvtkDicomViewer::OnChangeCursorValue()
{
	biDimensionalWidget->EnabledOff();
	contourWidget->EnabledOff();
	distanceWidget->EnabledOff();
	angleWidget->EnabledOff();

	ui.action_BiDimensional->setChecked(false);
	ui.action_Contour->setChecked(false);
	ui.action_Ruler->setChecked(false);
	ui.action_Protractor->setChecked(false);
	ui.action_Pointer->setChecked(false);

	ui.action_GrayLevel->setChecked(false);
	ui.action_Zoom->setChecked(false);
	ui.action_Move->setChecked(false);
	switch (CursorType)
	{
	case POINTRE://Ĭ��ָ��
		ui.action_Pointer->setChecked(true);
		break;
	case ZOOM://����
		ui.action_Zoom->setChecked(true);
		break;
	case GRAYLEVEL://�ҽ�
		ui.action_GrayLevel->setChecked(true);
		break;//������
	case PROTRACTOR:
		angleWidget->EnabledOn();
		angleWidget->SetWidgetStateToStart();
		ui.action_Protractor->setChecked(true);
		break;
	case RULER://����
		distanceWidget->EnabledOn();
		distanceWidget->SetWidgetStateToStart();
		ui.action_Ruler->setChecked(true);
		break;
	case CONTOUR://����
		contourWidget->EnabledOff();
		ui.action_Contour->setChecked(true);
		break;
	case BIDI://��ά��
		biDimensionalWidget->EnabledOn();
		biDimensionalWidget->SetWidgetStateToStart();
		ui.action_BiDimensional->setChecked(true);
		break;
	case MOVE:
		ui.action_Move->setChecked(true);
		break;
	default:
		break;
	}
}
/*
 * ��Series�ļ���
 */
void QvtkDicomViewer::OnOpenSeriesFolder()
{
	//��ȡSeries�ļ��еľ���·����
	QString dir = QFileDialog::getExistingDirectory(this, QStringLiteral("��SeriesĿ¼"), "F:/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (dir.isEmpty() == true)
		return;
	std::string folder = dir.toStdString();
	RenderInitializer(folder);
}
/*
 * �򿪵���Dicom�ļ�
 */
void QvtkDicomViewer::OnOpenDicomFile()
{

}
/*
 * ��DICOMDIR�ļ�
 */
void QvtkDicomViewer::OnOpenDicomDirFile()
{							  
	//���ļ�ѡ��ҳ��
	QString path = QFileDialog::getOpenFileName(this, QStringLiteral("��DICOMDIR�ļ�"), ".", QStringLiteral("ȫ������(*.*)"));
	if (path.isEmpty() == true)
		return;
	/*
	 * ��У��path�ǲ���DICOMDIR�ļ���·��,�������,�������沢�˳�
	 * �����,ִ��������������
	 */
	DicomDir *m_dicomdir = new DicomDir(path);
	connect(m_dicomdir, SIGNAL(sendData(QString,QString)), this, SLOT(receiveData(QString,QString)));
	m_dicomdir->show();
}
/*
 * ��Ӳ���
 */
void QvtkDicomViewer::addDistanceWidget()
{
	distanceWidget = vtkSmartPointer<vtkDistanceWidget>::New();
	distanceWidget->SetInteractor(renderWindowInteractor);
	distanceWidget->CreateDefaultRepresentation();
	//reader->SetDataSpacing(50, 50, 1.5);
	//static_cast<vtkDistanceRepresentation *>(distanceWidget->GetRepresentation())->SetScale(1.42f);//?
	//VTK��������ϵ�ĵ�λ������ĵ�λ=SetScale()
	static_cast<vtkDistanceRepresentation *>(distanceWidget->GetRepresentation())->SetLabelFormat("%-#6.3g mm");
}
/*
 * ���������
 */
void QvtkDicomViewer::addAngleWidget()
{
	angleWidget = vtkSmartPointer<vtkAngleWidget>::New();
	angleWidget->SetInteractor(renderWindowInteractor);
	angleWidget->CreateDefaultRepresentation();
}
/*
 * �����������
 */
void QvtkDicomViewer::addContourWidget()
{
	contourWidget =vtkSmartPointer<vtkContourWidget>::New();

	///�����ߵ���ɫ
	//contourRepresentation = vtkSmartPointer<vtkOrientedGlyphContourRepresentation>::New();
	//contourRepresentation->GetLinesProperty()->SetColor(1, 0, 0); // Set color to red
	//contourWidget->SetRepresentation(contourRepresentation);
	// Generate a set of points arranged in a circle
	int numPts = 10;
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	for (int i = 0; i < numPts; i++)
	{
		// Create numPts points evenly spread around a circumference of radius 0.1
		const double angle = 2.0*vtkMath::Pi()*i / numPts;
		points->InsertPoint(static_cast<vtkIdType>(i), 10*cos(angle), 10*sin(angle), 0.0);
	}

	// Create a cell array to connect the points into meaningful geometry
	vtkIdType* vertexIndices = new vtkIdType[numPts + 1];
	for (int i = 0; i < numPts; i++) { vertexIndices[i] = static_cast<vtkIdType>(i); }
	// Set the last vertex to 0; this means the last line segment will join the 19th point (vertices[19])
	// with the first one (vertices[0]), thus closing the circle.
	vertexIndices[numPts] = 0;
	vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
	lines->InsertNextCell(numPts + 1, vertexIndices);

	// Create polydata to hold the geometry just created, and populate it
	vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
	polydata->SetPoints(points);
	polydata->SetLines(lines);

	contourWidget->SetInteractor(renderWindowInteractor);
}
/*
 * �����Ƭҳ��
 */
void QvtkDicomViewer::SetSliceText(int current,int max)
{
	m_pImageViewer->GetRenderer()->RemoveActor(sliceTextActor);
	vtkSmartPointer<vtkTextProperty> sliceTextProp = vtkSmartPointer<vtkTextProperty>::New();
	sliceTextProp->SetFontFamilyToCourier();
	sliceTextProp->SetFontSize(20);
	sliceTextProp->SetVerticalJustificationToBottom();
	sliceTextProp->SetJustificationToLeft();

	vtkSmartPointer<vtkTextMapper> sliceTextMapper = vtkSmartPointer<vtkTextMapper>::New();
	std::string msg = StatusMessage::Format(current-1, max-1);//��ʾ6/21
	sliceTextMapper->SetInput(msg.c_str());
	sliceTextMapper->SetTextProperty(sliceTextProp);

	/*vtkSmartPointer<vtkActor2D>*/ sliceTextActor = vtkSmartPointer<vtkActor2D>::New();
	sliceTextActor->SetMapper(sliceTextMapper);
	sliceTextActor->SetPosition(15, 10);
	m_pImageViewer->GetRenderer()->AddActor2D(sliceTextActor);
	
}
/*
 * ���Dicom�ļ�ͷ��Ϣ
 */
void QvtkDicomViewer::SetUsageText()
{
	std::string temp = "";
	temp.append("PatientName:"); temp.append(reader->GetPatientName()); temp.append("\n");
	temp.append("DescriptiveName:"); temp.append(reader->GetDescriptiveName());	temp.append("\n");
	temp.append("BitsAllocated:"); temp.append(std::to_string(reader->GetBitsAllocated())); temp.append("\n");
	temp.append("GantryAngle:"); temp.append(std::to_string(reader->GetGantryAngle())); temp.append("\n");
	temp.append("StudyID:"); temp.append(reader->GetStudyID()); temp.append("\n");
	temp.append("StudyUID:"); temp.append(reader->GetStudyUID()); temp.append("\n");
	temp.append("DataByteOrder:"); temp.append(reader->GetDataByteOrderAsString()); temp.append("\n");
	temp.append("Width:"); temp.append(std::to_string(reader->GetWidth())); temp.append("\n");
	temp.append("Height:"); temp.append(std::to_string(reader->GetHeight())); temp.append("\n");
	temp.append("Data Spacing:"); 
	temp.append("("); temp.append(std::to_string(*reader->GetDataSpacing()));
	temp.append(","); temp.append(std::to_string(*(reader->GetDataSpacing() + 1)));
	temp.append(","); temp.append(std::to_string(*(reader->GetDataSpacing() + 2)));
	temp.append(")");temp.append("\n");
	const char* message = temp.c_str();
	//measurement

	// DICOM�ļ�ͷ��Ϣ
	usageTextProp = vtkSmartPointer<vtkTextProperty>::New();
	usageTextProp->SetFontFamilyToCourier();
	usageTextProp->SetFontSize(14);
	usageTextProp->SetColor(1.0, 1.0, 0.0);
	usageTextProp->SetVerticalJustificationToTop();
	usageTextProp->SetJustificationToLeft();

	usageTextMapper = vtkSmartPointer<vtkTextMapper>::New();
	usageTextMapper->SetInput(message);//////////////////////////////////////////////////////
	usageTextMapper->SetTextProperty(usageTextProp);

	usageTextActor = vtkSmartPointer<vtkActor2D>::New();
	usageTextActor->SetMapper(usageTextMapper);
	usageTextActor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
	usageTextActor->GetPositionCoordinate()->SetValue(0.05, 0.95);//����
}
/*
 * ���������ָʾ
 */
void QvtkDicomViewer::addOrientationMarker()
{
	axes =vtkSmartPointer<vtkAxesActor>::New();
	widget =vtkSmartPointer<vtkOrientationMarkerWidget>::New();
	widget->SetOutlineColor(0.9300, 0.5700, 0.1300);
	widget->SetOrientationMarker(axes);
	widget->SetInteractor(renderWindowInteractor);
	widget->SetViewport(0.0, 0.0, 0.2, 0.2);
	widget->SetEnabled(1);
	widget->InteractiveOff();//���ý���
	//widget->InteractiveOn();
}
/*
 * ��Ӷ�ά�߶ȱ��
 */
void QvtkDicomViewer::addBiDimensionalWidget()
{
	biDimensionalWidget =vtkSmartPointer<vtkBiDimensionalWidget>::New();
	biDimensionalWidget->SetInteractor(renderWindowInteractor);
	biDimensionalWidget->CreateDefaultRepresentation();
	biDimensionalCallback =vtkSmartPointer<vtkBiDimensionalCallback>::New();
	biDimensionalWidget->AddObserver(vtkCommand::InteractionEvent, biDimensionalCallback);
}
/*
 * ��������ͼ�����Ĳ˵�
 */
void QvtkDicomViewer::CreateContextMenu()
{

	QAction* action_New_Open_DICOMDIR_File = new QAction(QStringLiteral("&��DICOMDIR�ļ�"), ui.treeView);
	connect(action_New_Open_DICOMDIR_File, SIGNAL(triggered()), this, SLOT(OnOpenDicomDirFile()));

	QAction* action_New_Query_Patient_Msg = new QAction(QStringLiteral("&��ʾ������˵�ȫ����Ϣ"), ui.treeView);
	connect(action_New_Query_Patient_Msg, SIGNAL(triggered()), this, SLOT(FolderCreater()));

	QAction* action_New_Render_Series = new QAction(QStringLiteral("&��ʾ���Series"), ui.treeView);
	connect(action_New_Render_Series, SIGNAL(triggered()), this, SLOT(FolderCreater()));

	QAction* action_New_Render_Image = new QAction(QStringLiteral("&��ʾ���Image"), ui.treeView);
	connect(action_New_Render_Image, SIGNAL(triggered()), this, SLOT(FolderCreater()));
	
	//���Ҽ��˵�->����
	TreeViewMenu_OnEmpty = new QMenu(ui.treeView);
	TreeViewMenu_OnEmpty->addAction(action_New_Open_DICOMDIR_File);
	//���Ҽ��˵�->Patient�ڵ�
	TreeViewMenu_OnPatient = new QMenu(ui.treeView);
	TreeViewMenu_OnPatient->addAction(action_New_Query_Patient_Msg);
	//���Ҽ��˵�->Series�ڵ�
	TreeViewMenu_OnSeries = new QMenu(ui.treeView);
	TreeViewMenu_OnSeries->addAction(action_New_Render_Series);
	//���Ҽ��˵�->Image�ڵ�
	TreeViewMenu_OnImage = new QMenu(ui.treeView);
	TreeViewMenu_OnImage->addAction(action_New_Render_Image);
}
/*
 * ��ʾ��ǰseries�еĵ�Index��ͼ,Index��0��ʼ,����������
 */
void QvtkDicomViewer::ShowImageByIndex(int Index)
{
	RenderRefresh(imageAbsFilePath[Index], Index + 1, imageAbsFilePath.size());
}
/*
 *�޸ĵ�ǰ�������
 */
void QvtkDicomViewer::setCursor(CURSOR newValue)
{
	CursorType = newValue;
	emit CursorValueChanged();//ֵ����,�����ź�
}
/*
 * ��Ⱦ����ʼ��
 * ����:һ��ͼƬ���ļ�·��
 *	    ��Ҫ��ʾ��ͼƬ������,Ĭ��Ϊ1
 * ˵��:������Ⱦ��,Ҫô����Ⱦ����ͼ,Ҫô����Ⱦһ��ͼ������(����ͼ)
 */
void QvtkDicomViewer::RenderInitializer(std::string folder,int NumOfImage )
{
	/*
	 * reader��ʼ��
	 */
	reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetFileName(folder.c_str());
	/*
	 * reader��������
	 */
	vtkSmartPointer<vtkImageChangeInformation> changer =vtkSmartPointer<vtkImageChangeInformation>::New();
	changer->SetInputData(reader->GetOutput());
	changer->SetOutputOrigin(0, 0, 0);
	changer->SetOutputSpacing(10, 10, 1);
	changer->SetCenterImage(1);
	changer->Update();
	reader->Update();
	/*
	 * renderer��ʼ��
	 */
	m_pRenderder = vtkSmartPointer< vtkRenderer >::New();
	ui.qvtkWidget->GetRenderWindow()->AddRenderer(m_pRenderder);
	/*
	 * Viewer��ʼ��
	 */
	m_pImageViewer = vtkSmartPointer< vtkImageViewer2 >::New();
	m_pImageViewer->SetInputConnection(reader->GetOutputPort());
	/*
	 * ��������
	 */
	SetSliceText(1, NumOfImage);// ��Ƭҳ����Ϣ

	SetUsageText();// ��ʾһЩDicom�ļ�ͷ��Ϣ
	m_pImageViewer->GetRenderer()->AddActor2D(usageTextActor);
	/*
	 * �������ͽ�������ʼ��
	 */
	renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	myInteractorStyle = vtkSmartPointer<myVtkInteractorStyleImage>::New();
	myInteractorStyle->MouseFunction = myVtkInteractorStyleImage::POINTER;
	m_pImageViewer->SetupInteractor(renderWindowInteractor);
	m_pImageViewer->SetRenderWindow(ui.qvtkWidget->GetRenderWindow());
	renderWindowInteractor->SetInteractorStyle(myInteractorStyle);
	/*
	 * ����һЩVTK�ؼ�
	 */
	addDistanceWidget();// ʵ���������ò�����
	addAngleWidget();	// ʵ�������������������
	addContourWidget();	// ʵ�����������������
	addOrientationMarker();//���������ָʾ(û�п���)
	addBiDimensionalWidget();
	/*
	 * �Զ��������Ź��ܳ�ʼ��
	 */
	m_slice_player = new SlicePlayer(0,NumOfImage-1,ui.SliceScrollBar->sliderPosition(),50);
	connect(m_slice_player, SIGNAL(isTimeToTurnNextSlice()), this, SLOT(OnForward()));
	connect(m_slice_player, SIGNAL(isTimeToReset()), this, SLOT(OnResetToFirst()));
	/*
	 * ��Ⱦ
	 */
	m_pImageViewer->GetRenderer()->ResetCamera();
	ui.qvtkWidget->GetRenderWindow()->Render();
}
/*
 * ������Ⱦ/��Ⱦ��һ֡
 * �����������֮ǰ�����ȵ�����Ⱦ����ʼ������,���򽫻ᷢ������ʱ����
 */
void QvtkDicomViewer::RenderRefresh(std::string imagefilename,int currentPagenumber,int maxPageNumber)
{
	//�л��ļ�
	reader->SetFileName(imagefilename.c_str());
	//�л�ҳ����Ϣ
	SetSliceText(currentPagenumber, maxPageNumber);
	//�л�������Ϣ

	//������Ⱦ
	reader->Update();
	ui.qvtkWidget->GetRenderWindow()->Render();
}
/*
 * ˢ��DIR��
 */
void QvtkDicomViewer::DirTreeRefresh(DicomDataBase * database)
{
	QStandardItem* pItem0 = new QStandardItem(QStringLiteral("����"));
	pItem0->setToolTip(QStringLiteral("��������"));

	//QStandardItem* pItem1 = new QStandardItem(QStringLiteral("StudyID"));
	//pItem1->setToolTip(QStringLiteral("����"));

	//QStandardItem* pItem2 = new QStandardItem(QStringLiteral("Series"));
	//pItem2->setToolTip(QStringLiteral("ɨ������"));

	//QStandardItem* pItem3 = new QStandardItem(QStringLiteral("Image"));
	//pItem3->setToolTip(QStringLiteral("DicomͼƬ"));
	QStandardItemModel *model = new QStandardItemModel();

	model->setHorizontalHeaderItem(0, pItem0);
	//model->setHorizontalHeaderItem(1, pItem1);
	//model->setHorizontalHeaderItem(2, pItem2);
	//model->setHorizontalHeaderItem(3, pItem3);

	ui.treeView->setModel(model);

	QStandardItem* PatientItem = NULL;
	QStandardItem* StudyItem = NULL;
	QStandardItem* SeriesItem = NULL;
	QStandardItem* ImageItem = NULL;
	//DicomTreeItem*test = NULL;

	//model->appendRow(test);
	for(int i=0;i<database->PatientList.size();i++)
	{
		PatientItem= new QStandardItem(("Patient:"+database->PatientList[i]->PatientName).c_str());
		model->appendRow(PatientItem);
		for(int j=0;j<database->PatientList[i]->StudyList.size();j++)
		{
			StudyItem = new QStandardItem(("Study:"+database->PatientList[i]->StudyList[j]->StudyId).c_str());
			PatientItem->appendRow(StudyItem);
			//PatientItem->setChild(PatientItem->row(), 1, StudyItem);
			for(int k=0;k<database->PatientList[i]->StudyList[j]->SeriesList.size();k++)
			{
				SeriesItem = new QStandardItem(("Series:"+database->PatientList[i]->StudyList[j]->SeriesList[k]->SeriseNumber).c_str());
				StudyItem->appendRow(SeriesItem);
				//StudyItem->setChild(StudyItem->row(), 2, SeriesItem);
				for (int l = 0; l<database->PatientList[i]->StudyList[j]->SeriesList[k]->ImageList.size(); l++)
				{
					ImageItem = new QStandardItem(("Image:"+database->PatientList[i]->StudyList[j]->SeriesList[k]->ImageList[l]->ReferencedFileID).c_str());
					SeriesItem->appendRow(ImageItem);
					//SeriesItem->setChild(SeriesItem->row(), 3, ImageItem);
				}
			}
		}
	}
}
/*
 * ������->ǰһ��
 */
void QvtkDicomViewer::OnForward()
{
	ui.SliceScrollBar->setSliderPosition(ui.SliceScrollBar->sliderPosition() + 1);
}
/*
 * ������->��һ��
 */
void QvtkDicomViewer::OnBackward()
{
	ui.SliceScrollBar->setSliderPosition(ui.SliceScrollBar->sliderPosition() - 1);
}

/*
 * �ص���һ��
 */
void QvtkDicomViewer::OnResetToFirst()
{
	ui.SliceScrollBar->setSliderPosition(0);
}
/*
 * ѡ��Ĭ�����ָ�빤��
 */
void QvtkDicomViewer::OnSelectedPointer()
{
	setCursor(CURSOR::POINTRE);
	myInteractorStyle->MouseFunction = myVtkInteractorStyleImage::POINTER;
}
/*
 * ѡ������������
 */
void QvtkDicomViewer::OnSelectedProtractor()
{
	setCursor(CURSOR::PROTRACTOR);
}
/*
 * ѡ�в��߹���
 */
void QvtkDicomViewer::OnSelectedRuler()
{
	setCursor(CURSOR::RULER);
}
/*
 * ѡ����������
 */
void QvtkDicomViewer::OnSelectedContour()
{
	setCursor(CURSOR::CONTOUR);
}
/*
 * ѡ�ж�ά��߹���
 */
void QvtkDicomViewer::OnSelectedBiDimensional()
{
	setCursor(CURSOR::BIDI);
}
/*
 * ѡ�лҽ׹���
 */
void QvtkDicomViewer::OnSelectedGrayLevel()
{
	setCursor(CURSOR::GRAYLEVEL);
	myInteractorStyle->MouseFunction = myVtkInteractorStyleImage::GRAYLEVEL;
}
/*
 * ѡ�����Ź���
 */
void QvtkDicomViewer::OnSelectedZoom()
{
	setCursor(CURSOR::ZOOM);
	myInteractorStyle->MouseFunction = myVtkInteractorStyleImage::ZOOM;
}
/*
 * ѡ���ƶ�����
 */
void QvtkDicomViewer::OnSelectedMove()
{
	setCursor(CURSOR::MOVE);
	myInteractorStyle->MouseFunction = myVtkInteractorStyleImage::MOVE;
}
/*
 * ʹ�ø�ƬЧ��
 */
void QvtkDicomViewer::OnNegative()
{
	int subRegion[6] = { 0, 511, 0, 511, 0, 61 };//64, 448, 64, 448, 0, 0
												
	vtkImageIterator<unsigned char> iter(reader->GetOutput(), subRegion);
												
	while (!iter.IsAtEnd())//ע������������Դ��Ļ���Ч��					 
	{								 	
		unsigned char *inSI = iter.BeginSpan();									 	
		unsigned char *inSIEnd = iter.EndSpan();					 	
		while (inSI != inSIEnd)								 	
		{								 		
			*inSI = 255 - *inSI;								 		
			++inSI;									 	
		}									 	
		iter.NextSpan();								
	}
	reader->Update();//ˢ��
}
/*
 * ��λ��ť
 */
void QvtkDicomViewer::OnReset()
{
	//setCursor(CURSOR::POINTRE);
	//myInteractorStyle->MouseFunction = myVtkInteractorStyleImage::POINTER;
	//RenderInitializer(folder);
	/*
	 * ��λ���ܻ�һ��д��
	 */
}
/*
 * ����
 */
void QvtkDicomViewer::OnPlay()
{
	//myInteractorStyle->MoveSliceBackward();
	//
	if (PlayFlag==true)//��ʱ���ڲ���,����֮��Ӧ�ý�����ͣ,����ͼ���л�Ϊ����
	{
		ui.action_Play->setIcon(icon_Play);
		ui.action_Play->setText(QStringLiteral("����"));
		m_slice_player->pause.lock();//�����߳�
		ui.action_Stop->setEnabled(false);
		

	}else//��ʱΪֹͣ״̬,����֮��Ӧ�ý��в���,����ͼ���л�Ϊ��ͣ
	{
		ui.action_Play->setIcon(icon_Pause);
		ui.action_Play->setText(QStringLiteral("��ͣ"));
		//m_slice_player->setCurrentSlice(myInteractorStyle->getSlice());//��ͬ��ҳ��
		if (m_slice_player->isRunning()==true)//�̱߳�������
		{
			m_slice_player->pause.unlock();
		}else//�߳�û������(��һ������)
		{
			m_slice_player->start();
		}
		ui.action_Stop->setEnabled(true);
	}
	PlayFlag = !PlayFlag;
}
/*
 * ֹͣ
 */
void QvtkDicomViewer::OnStop()
{
	ui.action_Play->setIcon(icon_Play);
	ui.action_Play->setText(QStringLiteral("����"));
	PlayFlag = false;
	m_slice_player->pause.lock();
	//myInteractorStyle->ResetSliceToMin();
	ui.action_Stop->setEnabled(false);
}
/*
 * ����docking���ڵĿ���
 */
 void QvtkDicomViewer::OnSwitchProperty()
 {
	 //ui.action_SwitchOfProperty->isChecked();
	 if (ui.action_SwitchOfProperty->isChecked()==true)
	 {
		 ui.dockWidget_Dir->setHidden(false);
	 }
	 else
	 {
		 ui.dockWidget_Dir->setHidden(true);
	 }
 }
/*
 * ����ͼ�����Ĳ˵��ַ�
 */
 void QvtkDicomViewer::on_treeView_customContextMenuRequested(QPoint pos)
 {
	/*
	 *Ϊ�˴Ӹ����Ͻ���������,�����QAbstractModel����һ��Model��,
	 *�����Ҫ,��Ҫ�ж�Ӧ��Item��
	 *�������ֱ�Ӵ����ݿ����ʵ�������������������ֶ�,
	 *һ����������ʾ������ֶ�,��һ���������ڲ�����������ID�ֶ�
	 */
	 if (PrePosition != pos) {//��δ�������������
		 PrePosition = pos;
		 if (ui.treeView->model() == NULL) {
			 //��ʱ�ļ����ǿյ�
			 TreeViewMenu_OnEmpty->exec(QCursor::pos());//��ʾ�Ҽ��˵�
		 }
		 else//�ļ����ǿյ�ʱ������������
		 {
			 QModelIndex indexSelect = ui.treeView->indexAt(pos);  //��ǰ�ڵ�����
			 QString IndexTxt = indexSelect.data().toString();       //��ǰ�ڵ�����
			 if (IndexTxt.contains("Patient")==true)//�����ǰ�һ������ڲ�����
			 {
				 TreeViewMenu_OnPatient->exec(QCursor::pos());
			 }else if(IndexTxt.contains("Series") == true)
			 {
				 TreeViewMenu_OnSeries->exec(QCursor::pos());
			 }else if(IndexTxt.contains("Study") == true)
			 {
				 
			 }else if(IndexTxt.contains("Image") == true)
			 {
				 TreeViewMenu_OnImage->exec(QCursor::pos());
			 }
			 //qDebug() << indexSelect.column();
			 //QMessageBox::warning(this, QStringLiteral("����ѧ����ʺ�"), _colum);
		 }
	 }
	 else
	 {
		 //��δ����Ƿ���������
		 PrePosition.setX(-1);
		 PrePosition.setY(-1);
	 }
 }
/*
 * Slice������ֵ�����¼�
 */
 void QvtkDicomViewer::OnSliceScrollBarValueChange(int a)
 {
	 ShowImageByIndex(a);
 }
 /*
  *	���Զ�ȡ����ͼ
  */
 void QvtkDicomViewer::OnTestDCMTK_x64()
 {
	 DcmFileFormat fileformat;
	 OFCondition oc = fileformat.loadFile("E:/Դ���ʾ������/DICOM/S427870/S10/I10");
	 if (oc.good()) {
		 OFString patientName;
		 QString temp = "";
		 if (fileformat.getDataset()->findAndGetOFString(DCM_PatientName, patientName).good())
		 {
			 temp.append("Patient Name:");
			 temp.append(patientName.c_str());
			 temp.append("\n");
		 }
		 if (fileformat.getDataset()->findAndGetOFString(DCM_PatientID, patientName).good())
		 {
			 temp.append("Patient ID:");
			 temp.append(patientName.c_str());
			 temp.append("\n");
		 }
		 ///DCM_PatientBirthDate DCM_PatientSex
		 if (fileformat.getDataset()->findAndGetOFString(DCM_PatientBirthDate, patientName).good())
		 {
			 temp.append("Patient Birth Date:");
			 temp.append(patientName.c_str());
			 temp.append("\n");
		 }

		 if (fileformat.getDataset()->findAndGetOFString(DCM_PatientSex, patientName).good())
		 {
			 temp.append("Patient Sex:");
			 temp.append(patientName.c_str());
			 temp.append("\n");
		 }

		 QMessageBox::information(this, "Information", temp);
	 }
 }
 /*
  *	���Ե���DCMTK-x64��ȡDIR�ļ�
  */
 void QvtkDicomViewer::OnTestReadDICOMDIR()
 {
	/*===================�ú�����ȫ�ֱ�����===================*/
	 QString path;
	 DcmDirectoryRecord * root, *root1;//�ļ��ľ���·��
	/*=======================================================*/
	 //���ļ�ѡ��ҳ��
	 path = QFileDialog::getOpenFileName(this, QStringLiteral("���ļ�"), ".", QStringLiteral("ȫ������(*.*)"));
	 if (path.isEmpty() == true)
		 return;
	 DicomDir *m_dicomdir = new DicomDir(path);
	 connect(m_dicomdir, SIGNAL(sendData(QString,QString)), this, SLOT(receiveData(QString,QString)));
	 m_dicomdir->show();
 }
 /*
  *	��ӦDicomDir�ഫ�͹������ź�,���а�����һ�����˵�ID
  */
 void QvtkDicomViewer::receiveData(QString data,QString dir)
 {
	DicomDataBase * tempDaatabase = DicomDataBase::getInstance();
	Current_patientId = data.toStdString();//��ǰ�Ĳ���ID

	DirTreeRefresh(tempDaatabase);

	DicomDataBase * temp_database = DicomDataBase::getInstance();
	std::vector<DicomImage*> temp_dicom_images_v;//Ҫ�򿪵�series�е�ͼƬ����
												 //�ҵ�Ŀ��series
	/*
	 * Ŀǰû��ѡ��series�Ĺ���,����ʱ�ҵ�ѡ���Ĳ���,Ĭ�ϼ������ĵ�һ��study�ĵ�һ��series
	 */
	for (int i = 0; i<temp_database->PatientList.size(); i++)
	{
		if (temp_database->PatientList[i]->PatientID == Current_patientId)
		{
			temp_dicom_images_v = temp_database->PatientList[i]->StudyList[0]->SeriesList[0]->ImageList;//ע��������0
			break;
		}
	}	
	//��ȡdir�ļ���ǰ׺
	QFile *dirfile = new QFile(dir);
	auto dirfile_info= QFileInfo(*dirfile);
	QString FolderPrefix =dirfile_info.absolutePath();
	dirfile->close();
	/*
	 * ���ϸ�series�е�ȫ��image�ļ�·��
	 */
	//std::vector<std::string> imageAbsFilePath;


	for (int i = 0; i<temp_dicom_images_v.size(); i++)
	{
		QString temp = QString::fromStdString(FolderPrefix.toStdString() +"\\" + temp_dicom_images_v[i]->ReferencedFileID);

		temp.replace(QChar('\\'), QChar('/'));
		imageAbsFilePath.push_back(temp.toStdString());
		qDebug() << temp << "  i:  " << i << "  " << temp_dicom_images_v[i]->InstanceNumber.c_str() << endl;
	}
	RenderInitializer(imageAbsFilePath[0], imageAbsFilePath.size());
	ui.SliceScrollBar->setRange(0, imageAbsFilePath.size()-1);
 }