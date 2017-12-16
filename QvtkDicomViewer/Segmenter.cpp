#include "Segmenter.h"
#include <vtkDICOMImageReader.h>

/*
 * Ĭ�Ϲ���
 */
Segmenter::Segmenter(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}
/*
 * ����������
 */
Segmenter::Segmenter(std::string Filename, QWidget* parent) : QWidget(parent)
{
	ui.setupUi(this);
	m_Filename = Filename;

}
/*
 * ����,��������еķ�����ָ�����
 */
Segmenter::~Segmenter()
{
}
/*
 *��ͨ��ֵ����
 */
void Segmenter::OnConnectedThreshold()
{
}

/*
 *����㷨,�����䷽�
 */
void Segmenter::OnOtsuThreshold()
{
}
/*
 *�������ӷ�
 */
void Segmenter::OnNeighborhoodConnected()
{
}
/*
 * �������ӷ�
 */
void Segmenter::OnConfidenceConnected()
{
}
/*
 * ����ƥ��
 */
void Segmenter::OnFastMarching()
{
}
/*
 * ��״���
 */
void Segmenter::OnShapeDetection()
{
}
/*
 * ��Ӧ�ָ���ı仯/�״�����
 * 
 * �Ҿ�����������Բ𿪳�������,һ����һ���Եĳ�ʼ��������������Ĺ�����,��һ����ÿ�λ��㷨����ִ�еķ�����
 */
void Segmenter::OnSegmentFuncChange()
{
	// Read the image
	vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	if (m_Filename=="")
	{
		//��ʾһ��:�����������ʵ�����ǲ����ܳ��ֵ�
		return;
	}
	reader->SetFileName(m_Filename.c_str());
	reader->Update();

	vtkSmartPointer<vtkImageViewer2> imageViewer = vtkSmartPointer<vtkImageViewer2>::New();
	imageViewer->SetInputConnection(reader->GetOutputPort());
	//imageViewer->SetGlobalWarningDisplay(0);

	// Picker 
	vtkSmartPointer<vtkPropPicker> propPicker = vtkSmartPointer<vtkPropPicker>::New();
	propPicker->PickFromListOn();
	propPicker->AddPickList(imageViewer->GetImageActor());// Give the picker a prop to pick

	// ��ʾ
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	imageViewer->SetInputConnection(reader->GetOutputPort());
	imageViewer->SetupInteractor(renderWindowInteractor);
	imageViewer->SetSize(600, 600);

	vtkRenderer* renderer = imageViewer->GetRenderer();
	renderer->ResetCamera();
	renderer->GradientBackgroundOn();
	renderer->SetBackground(0, 0, 0);//������ɫ
	//renderer->SetBackground2(1, 1, 1);

	// Annotate the image with window/level and mouse over pixel information
	vtkSmartPointer<vtkCornerAnnotation> cornerAnnotation = vtkSmartPointer<vtkCornerAnnotation>::New();
	cornerAnnotation->SetLinearFontScaleFactor(2);
	cornerAnnotation->SetNonlinearFontScaleFactor(1);
	cornerAnnotation->SetMaximumFontSize(20);
	cornerAnnotation->SetText(0, "Off Image");
	cornerAnnotation->SetText(3, "<window>\n<level>");
	cornerAnnotation->GetTextProperty()->SetColor(1, 0, 0);

	imageViewer->GetRenderer()->AddViewProp(cornerAnnotation);

	// Callback listens to MouseMoveEvents invoked by the interactor's style
	vtkSmartPointer<vtkImageSegmentCallback> callback = vtkSmartPointer<vtkImageSegmentCallback>::New();//�����ص�����������
	callback->SetViewer(imageViewer);
	callback->SetAnnotation(cornerAnnotation);
	callback->SetPicker(propPicker);
	callback->SetQvtk(ui.qvtkWidget_Segment);
	callback->SetCount(m_segment_fun);//ע��ֵ�Ķ�Ӧ
	//callback->SetDir(dir);//�������������ô
	// InteractorStyleImage allows for the following controls:
	// 1) middle mouse + move = camera pan
	// 2) left mouse + move = window/level
	// 3) right mouse + move = camera zoom
	// 4) middle mouse wheel scroll = zoom
	// 5) 'r' = reset window/level
	// 6) shift + 'r' = reset camera
	vtkInteractorStyleImage* imageStyle = imageViewer->GetInteractorStyle();
	ui.qvtkWidget_Segment->SetRenderWindow(imageViewer->GetRenderWindow());

	imageStyle->AddObserver(vtkCommand::MouseMoveEvent, callback);
	imageStyle->AddObserver(vtkCommand::LeftButtonPressEvent, callback);

	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();
	//reader->Delete();

	//return EXIT_SUCCESS;
}
