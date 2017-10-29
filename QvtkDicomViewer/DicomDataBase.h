#pragma once
#include <string>
#include <vector>
#include "DicomPatient.h"

/*
 *ʵ����,����Dicom�����ļ��Ķ�ȡ���ڴ�洢
 *
 *����:1.��DICOMDIR�ļ�����
 *	   2.�Ӻ���DICOMDIR�ļ����ļ��й���
 *	   3.�Ӻ���һ��Series���ļ��й���
 *	   4.�ӵ���ͼƬ����
 *	   
 *2017��10��28��21:37:38
 *	  Ŀǰֻ���Ǵ�DICOMDIR�ļ���������
 *
 */
class DicomDataBase
{
public:
	static DicomDataBase* getInstance();
	void Init(std::string dir);
private:
	//static DicomDataBase* m_pSingleton;
	DicomDataBase();
	~DicomDataBase();
public:
	std::vector<DicomPatient*> PatientList;//���ݿ��б�������еĲ���
	std::string DicomDirFilePath;//DicomDir�ļ��ľ���·��
	std::string DicomForderPath;//DicomDIR�ļ����ڵ��ļ���,�ں������RefFileID�͵õ�ͼƬ�ľ���·��
};
///����ģʽ
//DicomDataBase* DicomDataBase::m_pSingleton = new DicomDataBase();
