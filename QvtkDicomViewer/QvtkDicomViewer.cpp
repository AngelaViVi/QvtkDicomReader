#include "myVtkInteractorStyleImage.h"
#include "vtkBiDimensionalCallback.h"
#include "DicomDir.h"

#include "QvtkDicomViewer.h"
#include <QMessageBox>
#include <qDebug>
#include <QComboBox>

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

#include "itkImage.h"
#include "itkImageSeriesReader.h"
#include "itkGDCMImageIO.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include <dcmtk\config\osconfig.h>
#include <dcmtk\dcmdata\dctk.h>

#include "SlicePlayer.h"
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
	ui.dockWidget_1->setHidden(false);
	icon_Play.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/play_128px_1197036_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
	icon_Pause.addFile(QStringLiteral(":/QvtkDicomViewer/Resources/pause_128px_1197034_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
	PlayFlag = false;
	ui.action_Stop->setEnabled(false);
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
 * ���ļ�
 */
void QvtkDicomViewer::OnOpenFile()
{
	//��ȡDicom�ļ��еľ���·����
	QString dir = QFileDialog::getExistingDirectory(this, QStringLiteral("��Ŀ¼"), "F:/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (dir.isEmpty() == true)
		return;
	folder = dir.toStdString();
	DoRender(folder);
	//0.�ж���ѡ�ļ���DIR�ļ�,DICOM�ļ���,���ǵ���DICOMͼƬ
		//0.1����DIR�ļ�,ʹ��DCMTK-64��,����Ŀ¼��Ϣ,�����ļ��б�,���û�ѡ��DICOM�ļ���
		//0.2����DICOM�ļ���,ֱ����Ϊ����ͼƬ����,�������������ŵȹ���
		//0.3���ڵ���DICOMͼƬ,ֱ����ʾ,�������ȥ�������ŵ���������
	//1.

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
void QvtkDicomViewer::SetSliceText()
{
	sliceTextProp = vtkSmartPointer<vtkTextProperty>::New();
	sliceTextProp->SetFontFamilyToCourier();
	sliceTextProp->SetFontSize(20);
	sliceTextProp->SetVerticalJustificationToBottom();
	sliceTextProp->SetJustificationToLeft();

	sliceTextMapper = vtkSmartPointer<vtkTextMapper>::New();
	std::string msg = StatusMessage::Format(m_pImageViewer->GetSliceMin(), m_pImageViewer->GetSliceMax());
	sliceTextMapper->SetInput(msg.c_str());
	sliceTextMapper->SetTextProperty(sliceTextProp);

	sliceTextActor = vtkSmartPointer<vtkActor2D>::New();
	sliceTextActor->SetMapper(sliceTextMapper);
	sliceTextActor->SetPosition(15, 10);
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
 *�޸ĵ�ǰ�������
 */
void QvtkDicomViewer::setCursor(CURSOR newValue)
{
	CursorType = newValue;
	emit CursorValueChanged();//ֵ����,�����ź�
}
/*
 * ��ʾ����·���е�Dicom����
 */
void QvtkDicomViewer::DoRender(std::string folder)
{
	// reader����������ļ�
	reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetDirectoryName(folder.c_str());
	//�����޸�һЩ����
	vtkSmartPointer<vtkImageChangeInformation> changer =vtkSmartPointer<vtkImageChangeInformation>::New();
	changer->SetInputData(reader->GetOutput());
	changer->SetOutputOrigin(0, 0, 0);
	changer->SetOutputSpacing(10, 10, 1);
	changer->SetCenterImage(1);
	changer->Update();

	reader->Update();//�������Ӧ�÷������������?
	// Renderer��ʼ��
	m_pRenderder = vtkSmartPointer< vtkRenderer >::New();
	// Renderer��������ڵ�Qvtkwidget
	ui.qvtkWidget->GetRenderWindow()->AddRenderer(m_pRenderder);
	//Viewer��ʼ������reader
	m_pImageViewer = vtkSmartPointer< vtkImageViewer2 >::New();
	m_pImageViewer->SetInputConnection(reader->GetOutputPort());
	SetSliceText();// ��Ƭҳ����Ϣ
	SetUsageText();// ��ʾһЩDicom�ļ�ͷ��Ϣ

	//================================================================================
	//ͼ��任�¹���ʵ������
	//================================================================================
	// ����������ʵ��
	renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	// �������(InteractorStyle)��ͨ���̳�vtkInteractorStyleImageʵ�ֵ�
	// ��������ʵ���������ֵĽ����¼�
	myInteractorStyle = vtkSmartPointer<myVtkInteractorStyleImage>::New();
	myInteractorStyle->MouseFunction = myVtkInteractorStyleImage::POINTER;
	//���Զ���Ľ�������д��ݱ�Ҫ�Ĳ���,�Ա��ڴ��������¼���ʵ����Ƭҳ��ĸ���
	myInteractorStyle->SetImageViewer(m_pImageViewer);
	myInteractorStyle->SetStatusMapper(sliceTextMapper);

	//ΪImageViewer���ý��������������,��������������QvtkWidget
	m_pImageViewer->SetupInteractor(renderWindowInteractor);
	m_pImageViewer->SetRenderWindow(ui.qvtkWidget->GetRenderWindow());

	// �ѽ������еĽ������ĳ��Զ���Ľ������
	// ����m_pImageViewer->SetupInteractor(renderWindowInteractor)�������û��Զ���
	// Ϊ��������Ĭ�Ͻ������,���Ա���Ҫ����֮���ٰ��Զ���Ľ������
	renderWindowInteractor->SetInteractorStyle(myInteractorStyle);

	// ��׼���õ�������ӵ�ImageViewer��
	m_pImageViewer->GetRenderer()->AddActor2D(sliceTextActor);
	m_pImageViewer->GetRenderer()->AddActor2D(usageTextActor);

	// ���ô��ڵĴ�С�ͱ���ɫ
	//imageViewer->GetRenderWindow()->SetSize(400, 300);
	//imageViewer->GetRenderer()->SetBackground(0.2, 0.3, 0.4);
	// m_pImageViewer->Render();

	//����һЩ�������
	addDistanceWidget();// ʵ���������ò�����
	addAngleWidget();	// ʵ�������������������
	addContourWidget();	// ʵ�����������������
	addOrientationMarker();//���������ָʾ(û�п���)
	addBiDimensionalWidget();

	// �����������г�ʼ��
	m_slice_player = new SlicePlayer(myInteractorStyle->getMinSlice(),
									 myInteractorStyle->getMaxSlice(),
									 myInteractorStyle->getSlice(),
									 50);
	connect(m_slice_player, SIGNAL(isTimeToTurnNextSlice()), this, SLOT(OnForward()));
	connect(m_slice_player, SIGNAL(isTimeToReset()), this, SLOT(OnResetToFirst()));
	// ������Ⱦ
#pragma region ������Ⱦ
	// m_pImageViewer->Render();//ò�Ʋ���Ҫ?
	m_pImageViewer->GetRenderer()->ResetCamera();
	ui.qvtkWidget->GetRenderWindow()->Render();
	renderWindowInteractor->Start();
#pragma endregion 
}
/*
 *ʹ��ITK��ȡԪ����,����ʾ��Docking������
 */
void QvtkDicomViewer::GetMetaDataAndRender(std::string folder)
{
	typedef signed short InputPixelType;//dicom ��Ӧ��������

	const unsigned int   InputDimension = 2;
	typedef itk::Image< InputPixelType, InputDimension > InputImageType;
	typedef itk::ImageSeriesReader< InputImageType > ReaderType;
	typedef itk::GDCMImageIO ImageIOType;//GDCMImageIO��DICOM

	ReaderType::Pointer _reader = ReaderType::New();
	_reader->SetFileName(folder);

	ImageIOType::Pointer gdcmImageIO = ImageIOType::New();
	//����GDCMImageIO���DICOM������Ϣ�Ͷ����ڴ棬ITK�ܻ�ȡ����ȫ�����Ϣ������VTK��
	_reader->SetImageIO(gdcmImageIO);
	_reader->Update();
	_reader->GetMetaDataDictionary();//��ȡDIOCMͷ�ļ�����Ϣ
	gdcmImageIO->GetMetaDataDictionary();//��ȡDIOCMͷ�ļ�����Ϣ

										 //������ȡͷ�ļ��в�����Ϣ
	char* name = new char[50];			//��������
	char* patientID = new char[50];		//����ID
	char* time = new char[50];			//ʱ��
	char* manufacture = new char[50];	//������
	char* modility = new char[50];		//����ֶ�
	char* hospital = new char[50];		//ҽԺ
	char* sex = new char[50];			//�Ա�
	char* age = new char[50];			//����

	unsigned int dim = 0;				//�ߴ�
	int ori = 0;						//����
	int spa = 0;						//�ռ�
										//���ܻ�ȡ�ܶ��ļ�ͷ��Ϣ

	int pixelType = gdcmImageIO->GetPixelType();
	int componetType = gdcmImageIO->GetComponentType();
	int fileType = gdcmImageIO->GetFileType();
	int componetSize = gdcmImageIO->GetComponentSize();
	int dimension = gdcmImageIO->GetNumberOfDimensions();

	ImageIOType::ByteOrder byteOrder = gdcmImageIO->GetByteOrder();
	ImageIOType::SizeType imgSize = gdcmImageIO->GetImageSizeInPixels();

	gdcmImageIO->GetPatientName(name);
	gdcmImageIO->GetPatientID(patientID);
	gdcmImageIO->GetStudyDate(time);
	gdcmImageIO->GetManufacturer(manufacture);
	gdcmImageIO->GetModality(modility);
	gdcmImageIO->GetInstitution(hospital);
	gdcmImageIO->GetPatientSex(sex);
	gdcmImageIO->GetPatientAge(age);
	QString temp = "(";
	temp.append(QString::fromStdString(std::to_string(gdcmImageIO->GetDimensions(0)))); temp.append(",");
	temp.append(QString::fromStdString(std::to_string(gdcmImageIO->GetDimensions(1))));temp.append(",");
	temp.append(QString::fromStdString(std::to_string(gdcmImageIO->GetDimensions(2))));temp.append(")");
	ui.lineEdit_Dimensions->setText(temp);
	temp = "(";
	temp.append(QString::fromStdString(std::to_string(gdcmImageIO->GetOrigin(0)))); temp.append(",");
	temp.append(QString::fromStdString(std::to_string(gdcmImageIO->GetOrigin(1)))); temp.append(",");
	temp.append(QString::fromStdString(std::to_string(gdcmImageIO->GetOrigin(2)))); temp.append(")");
	ui.lineEdit_Origins->setText(temp);
	temp = "(";
	temp.append(QString::fromStdString(std::to_string(gdcmImageIO->GetSpacing(0)))); temp.append(",");
	temp.append(QString::fromStdString(std::to_string(gdcmImageIO->GetSpacing(1)))); temp.append(",");
	temp.append(QString::fromStdString(std::to_string(gdcmImageIO->GetSpacing(2)))); temp.append(")");
	ui.lineEdit_Spacing->setText(temp);
	
	ui.lineEdit_Name->setText(name);
	ui.lineEdit_ID->setText(patientID);
	ui.lineEdit_Time->setText(time);
	ui.lineEdit_Manufacturer->setText(manufacture);
	ui.lineEdit_Modality->setText(modility);
	ui.lineEdit_Hospital->setText(hospital);
	ui.lineEdit_Sex->setText(sex);
	ui.lineEdit_Age->setText(age);
	
	//ui.lineEdit_Spacing->setText(std::to_string(sp);
	/*return temp;*/
	//ui.lineEdit_Age->setText(QStringLiteral("���"));
}
/*
 * ������->��ʾ�ļ�ͷ��Ϣ
 */
void QvtkDicomViewer::OnRenderText()
{
	//��ȡ�ı���Ϣ
	QString path = QFileDialog::getOpenFileName(this, QStringLiteral("���ļ�"), ".", QStringLiteral("ȫ������(*.*)"));
	if (path.isEmpty() == true)
		return;
	std::string folder = path.toStdString();
	GetMetaDataAndRender(folder);
}
/*
 * ������->ǰһ��
 */
void QvtkDicomViewer::OnForward()
{
	myInteractorStyle->MoveSliceForward();
}
/*
 * ������->��һ��
 */
void QvtkDicomViewer::OnBackward()
{
	myInteractorStyle->MoveSliceBackward();
}
/*
 * �ص���һ��
 */
void QvtkDicomViewer::OnResetToFirst()
{
	myInteractorStyle->ResetSliceToMin();
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
	setCursor(CURSOR::POINTRE);
	myInteractorStyle->MouseFunction = myVtkInteractorStyleImage::POINTER;
	DoRender(folder);
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
		m_slice_player->setCurrentSlice(myInteractorStyle->getSlice());//��ͬ��ҳ��
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
	myInteractorStyle->ResetSliceToMin();
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
		 ui.dockWidget_1->setHidden(false);
	 }
	 else
	 {
		 ui.dockWidget_1->setHidden(true);
	 }
 }
 /*
  *	���Ե���DCMTK-x64��ȡԪ����
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
	 connect(m_dicomdir, SIGNAL(sendData(QString)), this, SLOT(receiveData(QString)));
	 m_dicomdir->show();
 }
 /*
  *	��ӦDicomDir�ഫ�͹������ź�,���а�����һ�����˵�ID
  */
 void QvtkDicomViewer::receiveData(QString data)
 {
	//��δ���Ч���ر�Ͷ�����ȫ�ֱ������ò���������
	 /*DcmDirectoryRecord *   PatientRecord = NULL;
	 OFString tmpString;
	 std::string m_FilePath = path.toStdString();
	 DcmDicomDir dicomdir(m_FilePath.c_str());
	 root1 = &(dicomdir.getRootRecord());
	 while (((PatientRecord = root1->nextSub(PatientRecord)) != NULL))
	 {
		 if (PatientRecord->findAndGetOFString(DCM_PatientID, tmpString).good())
		 {
			 if (data == tmpString.c_str()) {
				 ui.lineEdit_Number->setText(tmpString.c_str());
				 if (PatientRecord->findAndGetOFString(DCM_PatientName, tmpString).good()) {
					 ui.lineEdit_Name->setText(tmpString.c_str());
				 }
				 if (PatientRecord->findAndGetOFString(DCM_PatientBirthDate, tmpString).good()) {
					 ui.lineEdit_Birth->setText(tmpString.c_str());
				 }

			 }
		 }
	 }*/
 }
