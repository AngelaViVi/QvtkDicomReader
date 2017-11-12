#pragma once
//�ڲ�
#include "ui_QvtkDicomViewer.h"
//Qt
#include <QtWidgets/QMainWindow>
#include <QString>
#include <QFileDialog>
//vtk
#include <vtkRenderWindow.h>
#include <vtkDICOMImageReader.h>
#include <vtkImageActor.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkImageViewer2.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkAutoInit.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkDICOMImageReader.h>
#include <vtkTextProperty.h>
#include <vtkTextMapper.h>
#include <vtkActor2D.h>
#include <vtkTextProperty.h>
#include <vtkTextMapper.h>
#include <vtkActor2D.h>
#include <myVtkInteractorStyleImage.h>
#include <vtkDistanceWidget.h>
#include <vtkAngleWidget.h>
#include <vtkContourWidget.h>
#include <vtkOrientedGlyphContourRepresentation.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkBiDimensionalWidget.h>
#include "vtkBiDimensionalCallback.h"
#include "SlicePlayer.h"
#include "DicomPatient.h"
#include "DicomDirTreeModel.h"

class DicomDataBase;
/*
 * ����˵��:
 * 1.����������:��Qt��ص��������ڽ������->����������
 *				���������������Թ�����->Debug_x64,Release_x64������
 * 2.��Ŀ¼�Ͱ���Ŀ¼:�ڽ������->����������
 * 3.�����汾:Qt5.9.1
 *			  vtk8.0.0
 *			  dcmtk3.6.2(64λmsvc15-2017)
 * 4.QtĿ¼:
 *			�ڽ������Ŀ¼->QvtkDicomViewer->QvtkDicomViewer.vcxproj.user�ļ���
 *			����Qt��Ŀ¼
 */
VTK_MODULE_INIT(vtkRenderingFreeType)
//Qt��������
class QvtkDicomViewer : public QMainWindow
{
	Q_OBJECT

public:
	QvtkDicomViewer(QWidget *parent = Q_NULLPTR);
	enum CURSOR		
	{	POINTRE,		//Ĭ��ָ��
		ZOOM,			//����
		GRAYLEVEL,		//�ҽ�
		PROTRACTOR,		//������
		RULER,			//����
		CONTOUR,		//����
		BIDI,			//��ά��
		MOVE			//�ƶ�
	}CursorType;
private:
	//CURSOR CursorType;//�������
	Ui::QvtkDicomViewerClass ui;
	vtkSmartPointer< vtkImageViewer2 > m_pImageViewer;
	vtkSmartPointer< vtkRenderer > m_pRenderder;
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;
	vtkSmartPointer<vtkDICOMImageReader> reader;
	//vtkSmartPointer<vtkTextProperty> sliceTextProp;
	//vtkSmartPointer<vtkTextMapper> sliceTextMapper;
	vtkSmartPointer<vtkActor2D> sliceTextActor;//������ȫ�ֵ�.�����޷�������һ�εõ��ۼ�
	//vtkSmartPointer<vtkTextProperty> usageTextProp;
	//vtkSmartPointer<vtkTextMapper> usageTextMapper;
	vtkSmartPointer<vtkActor2D> usageTextActor1;
	vtkSmartPointer<vtkActor2D> usageTextActor2;
	vtkSmartPointer<vtkActor2D> usageTextActor3;
	vtkSmartPointer<myVtkInteractorStyleImage> myInteractorStyle;
	vtkSmartPointer<vtkDistanceWidget> distanceWidget;//���
	vtkSmartPointer<vtkAngleWidget> angleWidget;//�����Ƕ�
	vtkSmartPointer<vtkContourWidget> contourWidget;//����ѡ��
	vtkSmartPointer<vtkAxesActor> axes;
	vtkSmartPointer<vtkOrientedGlyphContourRepresentation> contourRepresentation;
	vtkSmartPointer<vtkOrientationMarkerWidget> widget;
	vtkSmartPointer<vtkBiDimensionalWidget> biDimensionalWidget;
	vtkSmartPointer<vtkBiDimensionalCallback> biDimensionalCallback;

	QIcon icon_Play;//����ͼ��
	QIcon icon_Pause;//��ͣͼ��
	bool PlayFlag;//false:ͼ��ӦΪ����,����׼������״̬,true:ͼ��ӦΪ��ͣ,���ڲ���״̬��׼����ͣ
	SlicePlayer *m_slice_player;//����һ���߳���

	QPoint PrePosition;					//ǰһ�ε��Ҽ����λ��
	QMenu * TreeViewMenu_OnEmpty;		//���Ҽ��˵�->��Ϊ��
	QMenu * TreeViewMenu_OnPatient;		//���Ҽ��˵�->���˽ڵ�
	QMenu * TreeViewMenu_OnSeries;		//���Ҽ��˵�->Series�ڵ�
	QMenu * TreeViewMenu_OnImage;		//���Ҽ��˵�->Image�ڵ�

	std::string Current_patientId;//��ǰ�Ĳ���ID
	DicomPatient * CurrentPatient;		//��ǰ����
	QModelIndex indexSelect;//����ͼ��
	DicomDirTreeModel *m_dicomdirtreemodel;
private:
	///�ڲ�����
	void setCursor(CURSOR newValue);
	void RenderInitializer(std::string folder, int NumOfImage = 1);//��Ⱦ����ʼ��
	void RenderRefresh(std::string imagefilename, int currentPagenumber, int maxPageNumber);//������Ⱦ
	void DirTreeRefresh(DicomPatient * patient);
	void addDistanceWidget();
	void addAngleWidget();
	void addContourWidget();
	void SetSliceText(int current, int max);			//������ʾҳ����Ϣ
	void SetUsageText(std::string imagefilename);
	void addOrientationMarker();
	void addBiDimensionalWidget();
	void CreateContextMenu();//����ͼ�����Ĳ˵�
	void ShowImageByIndex(int Index);//��ʾ��ǰseries�еĵ�Index��ͼ,Index��0��ʼ,����������
signals :
	void CursorValueChanged();      //�Զ���ֵ�����ź�,���ڼ�ص�ǰ���ı仯
public slots:
	void OnChangeCursorValue();		//��Ӧ���ֵ���޸�,ִ��һЩˢ�ºͽ��ò���
	void OnOpenSeriesFolder();		//�ĳ�OnOpenSeriesFolder
	void OnOpenDicomFile();			//�򿪵���Dicom�ļ�
	void OnOpenDicomDirFile();		//��DICOMDIR�ļ�
	void OnForward();				//ǰһ�Ű���
	void OnBackward();				//��һ�Ű���
	void OnResetToFirst();			//�ص���һ��
	void OnSelectedPointer();		//ѡ��Ĭ�����ָ�빤��
	void OnSelectedProtractor();	//ѡ������������
	void OnSelectedRuler();			//ѡ�в��߹���
	void OnSelectedContour();		//ѡ����������
	void OnSelectedBiDimensional();	//ѡ�ж�ά��߹���
	void OnSelectedGrayLevel();		//ѡ�лҽ׹���
	void OnSelectedZoom();			//ѡ�����Ź���
	void OnSelectedMove();			//ѡ���ƶ�����
	void OnNegative();				//ʹ�ø�ƬЧ��
	void OnReset();					//��λ
	void OnPlay();					//����/��ͣ
	void OnStop();					//ֹͣ
	void OnSwitchProperty();		//����docking���ڵĿ���
	void on_treeView_customContextMenuRequested(QPoint pos);//����ͼ�����Ĳ˵��ַ�
	void OnShowDicomCurrentTags();	//��ʾ��ǰ���˵�������Ϣ
	void OnShowSelectedSeries();	//��ʾѡ�е�Series
	void OnShowSelectedImage();		//��ʾ��ǰѡ�е�Image
	void OnSliceScrollBarValueChange(int a);//Slice������ֵ�����¼�
	void receiveData(QString data,QString dir);//��ӦDicomDir�ഫ�͹������ź�,���а�����һ�����˵�ID
	///�������
	void OnTestEntrance_01();//�������1
	void OnTestEntrance_02();//�������2
};
