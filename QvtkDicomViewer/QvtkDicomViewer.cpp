/*
 * �ع�:2017��7��19��09:57:21
 * ���:��ʹ΢����
 * ��־:1.���´���ṹ,��ȡ���ɺ���,�޸���ҳ����ʾ��bug,����ע��
 *		2.�ڴ˻�����,��Ӳ�����Ϣ��ʾ�Ĺ���.
 *		3.��Ӳ���С����
 */
#include "myVtkInteractorStyleImage.h"

#include <vtkActor2D.h>
#include "QvtkDicomViewer.h"
#include <vtkTextMapper.h>
#include <vtkTextProperty.h>

#include <vtkDistanceWidget.h>
#include <vtkDistanceRepresentation.h>

#include <vtkAngleWidget.h>
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
 * ������->����(�շ���)
 */
void QvtkDicomViewer::OnRenderText()
{
	//��ȡ�ı���Ϣ
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
