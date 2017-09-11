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
//itk

/*
 * ����˵��:
 * 1.����������:��Qt��ص��������ڽ������->����������
 *				���������������Թ�����->Debug_x64,Release_x64������
 * 2.��Ŀ¼�Ͱ���Ŀ¼:�ڽ������->����������
 * 3.�����汾:Qt5.9.1
 *			  vtk8.0.0
 *			  itk4.12.0
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
	enum CURSOR		//������� 
	{POINTRE,		//Ĭ��ָ��
		PROTRACTOR,	//������
		RULER			//����
	};
private:
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
private:
	///�ڲ�����
	void DoRender(std::string folder);//������Դ,��ʾDicom����
	std::string DoGetMetaData(std::string folder);//��ȡDicomԪ����,���ػ�ȡ������Ϣ
public slots:
	void OnOpenFile();
	void OnRenderText();
	void OnForward();//ǰһ��
	void OnBackward();//��һ��
	void OnSelectedPointer();//ѡ��Ĭ�����ָ�빤��
	void OnSelectedProtractor();//ѡ������������
	void OnSelectedRuler();//ѡ�в��߹���
};
