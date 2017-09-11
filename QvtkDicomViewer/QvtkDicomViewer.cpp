#include "myVtkInteractorStyleImage.h"
#include "QvtkDicomViewer.h"

#include <vtkActor2D.h>
#include <vtkTextMapper.h>
#include <vtkTextProperty.h>
#include <vtkDistanceWidget.h>
#include <vtkDistanceRepresentation.h>
#include <vtkAngleWidget.h>
#include <vtkImageChangeInformation.h>

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
{//jiafeng
	// Read all the DICOM files in the specified directory.
	reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetDirectoryName(folder.c_str());
	//reader->SetDataSpacing(50, 50, 1.5);
	reader->Update();
	//=================================
	//vtkSmartPointer<vtkImageChangeInformation> changer =vtkSmartPointer<vtkImageChangeInformation>::New();
	//changer->SetInputData(reader->GetOutputDataObject(4));
	////miao?ha?
	//changer->SetOutputOrigin(100, 100, 0);
	//changer->SetOutputSpacing(5, 5, 1);
	//changer->SetCenterImage(1);
	//changer->Update();
	//===================================
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
	temp.append("Data Spacing:"); 
	temp.append("("); temp.append(std::to_string(*reader->GetDataSpacing()));
	temp.append(","); temp.append(std::to_string(*(reader->GetDataSpacing() + 1)));
	temp.append(","); temp.append(std::to_string(*(reader->GetDataSpacing() + 2)));
	temp.append(")");temp.append("\n");
	const char* message = temp.c_str();
	//measurement
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
	static_cast<vtkDistanceRepresentation *>(distanceWidget->GetRepresentation())->SetScale(1.42f);//?
	/*reader->GetPixelSpacing();
	 *
	m_pImageViewer->GetInteractorStyle();*/
	//reader->SetDataSpacing(3.2,3.2,1.5);
	//VTK��������ϵ�ĵ�λ������ĵ�λ=SetScale()
	// ʵ�������������������
	angleWidget = vtkSmartPointer<vtkAngleWidget>::New();
	angleWidget->SetInteractor(renderWindowInteractor);
	angleWidget->CreateDefaultRepresentation();
	renderWindowInteractor->Start();
	// ������Ⱦ
	//m_pImageViewer->GetRenderer()->ResetCamera();
	//// m_pImageViewer->Render();
	//ui.qvtkWidget->GetRenderWindow()->Render();
	//renderWindowInteractor->Start();
	//qDebug() << reader->GetDirectoryName();
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
