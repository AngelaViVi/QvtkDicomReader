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
//itk

class DicomDataBase;
/*
 * ����˵��:
 * 1.����������:��Qt��ص��������ڽ������->����������
 *				���������������Թ�����->Debug_x64,Release_x64������
 * 2.��Ŀ¼�Ͱ���Ŀ¼:�ڽ������->����������
 * 3.�����汾:Qt5.9.1
 *			  vtk8.0.0
 *			  itk4.12.0
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
	vtkSmartPointer<vtkTextProperty> sliceTextProp;
	vtkSmartPointer<vtkTextMapper> sliceTextMapper;
	vtkSmartPointer<vtkActor2D> sliceTextActor;
	vtkSmartPointer<vtkTextProperty> usageTextProp;
	vtkSmartPointer<vtkTextMapper> usageTextMapper;
	vtkSmartPointer<vtkActor2D> usageTextActor;
	vtkSmartPointer<myVtkInteractorStyleImage> myInteractorStyle;
	vtkSmartPointer<vtkDistanceWidget> distanceWidget;//���
	vtkSmartPointer<vtkAngleWidget> angleWidget;//�����Ƕ�
	vtkSmartPointer<vtkContourWidget> contourWidget;//����ѡ��
	vtkSmartPointer<vtkAxesActor> axes;
	vtkSmartPointer<vtkOrientedGlyphContourRepresentation> contourRepresentation;
	vtkSmartPointer<vtkOrientationMarkerWidget> widget;
	vtkSmartPointer<vtkBiDimensionalWidget> biDimensionalWidget;
	vtkSmartPointer<vtkBiDimensionalCallback> biDimensionalCallback;
	std::string folder;//�洢��ǰ��ȡ��Dicom��Ŀ¼,�Ƿ��б�Ҫ��Ϊȫ�ֱ����������޶���
	QIcon icon_Play;//����ͼ��
	QIcon icon_Pause;//��ͣͼ��
	bool PlayFlag;//false:ͼ��ӦΪ����,����׼������״̬,true:ͼ��ӦΪ��ͣ,���ڲ���״̬��׼����ͣ
	SlicePlayer *m_slice_player;//����һ���߳���
	std::string Current_patientId;//��ǰ�Ĳ���ID
private:
	///�ڲ�����
	void setCursor(CURSOR newValue);
	void DoRender(std::string folder);//������Դ,��ʾDicom����
	void SeriesRender(std::string first);//�ֶ�����һ��series
	void DirTreeRefresh(DicomDataBase * database);
	void GetMetaDataAndRender(std::string folder);//ʹ��ITK��ȡԪ����,����ʾ��Docking������
	void addDistanceWidget();
	void addAngleWidget();
	void addContourWidget();
	void SetSliceText();
	void SetUsageText();
	void addOrientationMarker();
	void addBiDimensionalWidget();
signals :
	void CursorValueChanged();        //�Զ���ֵ�����ź�,���ڼ�ص�ǰ���ı仯
public slots:

	void OnChangeCursorValue();//��Ӧ���ֵ���޸�,ִ��һЩˢ�ºͽ��ò���
	void OnOpenFile();
	void OnRenderText();
	void OnForward();//ǰһ��
	void OnBackward();//��һ��
	void OnResetToFirst();//�ص���һ��
	void OnSelectedPointer();//ѡ��Ĭ�����ָ�빤��
	void OnSelectedProtractor();//ѡ������������
	void OnSelectedRuler();//ѡ�в��߹���
	void OnSelectedContour();//ѡ����������
	void OnSelectedBiDimensional();//ѡ�ж�ά��߹���
	void OnSelectedGrayLevel();//ѡ�лҽ׹���
	void OnSelectedZoom();//ѡ�����Ź���
	void OnSelectedMove();//ѡ���ƶ�����
	void OnNegative();//ʹ�ø�ƬЧ��
	void OnReset();//��λ
	void OnPlay();//����/��ͣ
	void OnStop();//ֹͣ
	void OnSwitchProperty();//����docking���ڵĿ���
	///���ڲ��ԵĹ���
	void OnTestDCMTK_x64();//���Ե���DCMTK-x64��ȡԪ����
	void OnTestReadDICOMDIR();//���Ե���DCMTK-x64��ȡDIR�ļ�
	void receiveData(QString data);//��ӦDicomDir�ഫ�͹������ź�,���а�����һ�����˵�ID
};
