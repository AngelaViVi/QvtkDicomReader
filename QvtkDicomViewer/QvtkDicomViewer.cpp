#include "myVtkInteractorStyleImage.h"
#include "QvtkDicomViewer.h"

#include <vtkActor2D.h>
#include <vtkTextMapper.h>
#include <vtkTextProperty.h>
#include <vtkDistanceWidget.h>
#include <vtkDistanceRepresentation.h>
#include <vtkAngleWidget.h>

#include "itkImage.h"
#include "itkImageSeriesReader.h"
#include "itkGDCMImageIO.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include <QMessageBox>
#include <qDebug>
/*
 * ���췽��
 */
QvtkDicomViewer::QvtkDicomViewer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_pImageViewer = vtkSmartPointer< vtkImageViewer2 >::New();
	m_pRenderder = vtkSmartPointer< vtkRenderer >::New();
	// ����m_QVTKWidget����Ⱦ��
	ui.qvtkWidget->GetRenderWindow()->AddRenderer(m_pRenderder);
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
	std::string folder = dir.toStdString();
	DoRender(folder);
}
/*
 * ��ʾ����·���е�Dicom����
 */
void QvtkDicomViewer::DoRender(std::string folder)
{
	// Read all the DICOM files in the specified directory.
	reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetDirectoryName(folder.c_str());
	reader->Update();
	// Visualize
	m_pImageViewer->SetInputConnection(reader->GetOutputPort());

	// ��Ƭҳ����Ϣ
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

	// DICOM�ļ�ͷ��Ϣ
#pragma region ��ȡDICOM�ļ�ͷ��Ϣ
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
	const char* message = temp.c_str();
#pragma endregion 
	usageTextProp = vtkSmartPointer<vtkTextProperty>::New();
	usageTextProp->SetFontFamilyToCourier();
	usageTextProp->SetFontSize(14);
	usageTextProp->SetVerticalJustificationToTop();
	usageTextProp->SetJustificationToLeft();

	usageTextMapper = vtkSmartPointer<vtkTextMapper>::New();
	usageTextMapper->SetInput(message);//////////////////////////////////////////////////////
	usageTextMapper->SetTextProperty(usageTextProp);

	usageTextActor = vtkSmartPointer<vtkActor2D>::New();
	usageTextActor->SetMapper(usageTextMapper);
	usageTextActor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
	usageTextActor->GetPositionCoordinate()->SetValue(0.05, 0.95);//����

	// ����������ʵ��
	renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	// �������(InteractorStyle)��ͨ���̳�vtkInteractorStyleImageʵ�ֵ�
	// ��������ʵ���������ֵĽ����¼�
	myInteractorStyle = vtkSmartPointer<myVtkInteractorStyleImage>::New();
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

	// ʵ���������ò�����
	distanceWidget = vtkSmartPointer<vtkDistanceWidget>::New();
	distanceWidget->SetInteractor(renderWindowInteractor);
	distanceWidget->CreateDefaultRepresentation();
	static_cast<vtkDistanceRepresentation *>(distanceWidget->GetRepresentation())->SetLabelFormat("%-#6.3g mm");

	// ʵ�������������������
	angleWidget = vtkSmartPointer<vtkAngleWidget>::New();
	angleWidget->SetInteractor(renderWindowInteractor);
	angleWidget->CreateDefaultRepresentation();
	renderWindowInteractor->Start();

	// ������Ⱦ
	m_pImageViewer->GetRenderer()->ResetCamera();
	// m_pImageViewer->Render();
	ui.qvtkWidget->GetRenderWindow()->Render();
	renderWindowInteractor->Start();
}
/*
 *��ȡDicomԪ����,���ػ�ȡ������Ϣ
 */
std::string QvtkDicomViewer::DoGetMetaData(std::string folder)
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

	gdcmImageIO->GetDimensions(dim);
	gdcmImageIO->GetOrigin(ori);
	gdcmImageIO->GetSpacing(spa);
	gdcmImageIO->GetPatientName(name);
	gdcmImageIO->GetModality(modility);
	gdcmImageIO->GetPatientID(patientID);
	gdcmImageIO->GetManufacturer(manufacture);
	gdcmImageIO->GetStudyDate(time);
	gdcmImageIO->GetInstitution(hospital);

	std::string temp = "";
	temp.append("Patient Name:"); temp.append(name);						 temp.append("\n");
	temp.append("Patient ID:");	  temp.append(patientID);					 temp.append("\n");
	temp.append("Time:");		  temp.append(time);						 temp.append("\n");
	temp.append("Manufacture:");  temp.append(manufacture);					 temp.append("\n");
	temp.append("Modility:");	  temp.append(modility);					 temp.append("\n");
	temp.append("hospital:");	  temp.append(hospital);					 temp.append("\n");
	temp.append("dim:");		  temp.append(std::to_string(dim));			 temp.append("\n");
	temp.append("ori:");		  temp.append(std::to_string(ori));			 temp.append("\n");
	temp.append("spa:");		  temp.append(std::to_string(spa));			 temp.append("\n");
	temp.append("pixelType:");	  temp.append(std::to_string(pixelType));    temp.append("\n");
	temp.append("componetType:"); temp.append(std::to_string(componetType)); temp.append("\n");
	temp.append("fileType:");	  temp.append(std::to_string(fileType));	 temp.append("\n");
	temp.append("componetSize:"); temp.append(std::to_string(componetSize)); temp.append("\n");
	temp.append("dimension:");	  temp.append(std::to_string(dimension));	 temp.append("\n");
	return temp;
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
	DoGetMetaData(folder);
	std::string temp = DoGetMetaData(folder);
	//qDebug() << DoGetMetaData(folder);
	QMessageBox::information(NULL, QStringLiteral("�ļ�ͷ��Ϣ"), temp.c_str());
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
 * ѡ��Ĭ�����ָ�빤��
 */
void QvtkDicomViewer::OnSelectedPointer()
{
	angleWidget->EnabledOff();
	distanceWidget->EnabledOff();
	ui.action_Pointer->setChecked(true);
	ui.action_Protractor->setChecked(false);
	ui.action_Ruler->setChecked(false);
}
/*
 * ѡ������������
 */
void QvtkDicomViewer::OnSelectedProtractor()
{
	angleWidget->EnabledOn();
	distanceWidget->EnabledOff();
	ui.action_Protractor->setChecked(true);
	ui.action_Pointer->setChecked(false);
	ui.action_Ruler->setChecked(false);
}
/*
 * ѡ�в��߹���
 */
void QvtkDicomViewer::OnSelectedRuler()
{
	distanceWidget->EnabledOn();
	angleWidget->EnabledOff();
	ui.action_Ruler->setChecked(true);
	ui.action_Protractor->setChecked(false);
	ui.action_Pointer->setChecked(false);
}
