#pragma once
#include <string>

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
	DicomDataBase(std::string dir);
	~DicomDataBase();
};

