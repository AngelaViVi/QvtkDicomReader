#pragma once

#include <QWidget>
#include "ui_Register.h"
#include <QVTKWidget.h>

#define ITK_IO_FACTORY_REGISTER_MANAGER

//�����궨�������Ŀǰ���޷���
//ȱ����Ӧ��ʵ��

enum Reg_enum
{
	Reg_Null,
	Reg_test,
	Reg_2Dtransform,
	Reg_AffineTrans,
	Reg_Multi
};

class Register : public QWidget
{
	Q_OBJECT

public:
	Register(QWidget *parent = Q_NULLPTR);
	~Register();
	void SetCount(int count);
	void SetQvtk(QVTKWidget* qvtk);
private:
	Ui::Register ui;
private:
	///��׼����
	void TranslationReg(char *argv[]);
	void CenteredSimilarityTransformReg(char *argv[]);
	void AffineTransformReg(char*argv[]);
	void MultiTransformReg(char*argv[]);
private slots:
	void OnSelectImageFix();		//ѡ���׼ͼ��
	void OnSelectImageMove();		//ѡ�����׼ͼ��
	void OnButtonOk();				//ok-��ʼ����
	void OnButtonCancel();			//�˳�
private:
	///���������ı���
	int reg_count;
	QVTKWidget* qvtk;
};
