#pragma once

#include <QWidget>
#include "ui_Register.h"
#include <QVTKWidget.h>
#include <vtkImageActor.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <itkImage.h>
#include <itkImageToVTKImageFilter.h>

#define ITK_IO_FACTORY_REGISTER_MANAGER

//�����궨�������Ŀǰ���޷���
//ȱ����Ӧ��ʵ��

enum RegistrationFunc
{
	RegFunc_Translation,
	RegFunc_CenteredSimilarity,
	RegFunc_Affine,
	RegFunc_Multi
};

class Register : public QWidget
{
	Q_OBJECT
	typedef  float           PixelType;
	typedef itk::Image< PixelType, 2 > OutputImageType;
	typedef itk::ImageToVTKImageFilter<OutputImageType>   ConnectorType;
public:
	Register(QWidget *parent = Q_NULLPTR);
	~Register();
private:
	Ui::Register ui;
private:
	///��׼����
	void TranslationReg(char *argv[]);
	void CenteredSimilarityTransformReg(char *argv[]);
	void AffineTransformReg(char*argv[]);
	void MultiTransformReg(char*argv[]);

	void updateOutputImage();
private slots:
	void OnSelectImageFix();		    //ѡ���׼ͼ��
	void OnSelectImageMove();		    //ѡ�����׼ͼ��
	void OnButtonOk();				    //ok-��ʼ����
	void OnButtonCancel();			    //�˳�
	void OnSelectTranslation();			//ѡ��ƽ�Ʊ任
	void OnSelectCenteredSimilarity();	//ѡ���������ƶ�ά�任
	void OnSelectAffine();				//ѡ�����任
	void OnSelectMulit();				//ѡ��Ĥ���任
private:
	vtkSmartPointer<vtkImageActor> actor[4];
	vtkSmartPointer<vtkRenderer> renderer[4];
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor[4];
	vtkSmartPointer<vtkInteractorStyleImage> style[4];
	QVTKWidget * m_output_widgets[4];
	ConnectorType::Pointer connector[4];
	///���������ı���
	int reg_count= RegFunc_Multi;
	RegistrationFunc m_CurrentRegFunc;//ѡ·����

};