#pragma once
#include <string>
#include "DicomImage.h"
#include <vector>

/*
 * ���ڴ洢series�йص���Ϣ
 * ��Dicom��׼��,seriesָ����������������ͼƬ���ɵ�һ������
 * 
 * ����:
 */
class DicomSeries
{
public:
	DicomSeries();
	~DicomSeries();
public:
	std::string Manufacturer = "";					//������
	std::string StationName = "";					//��������
	std::string SeriseNumber = "";					//
	std::string NumberOfSeriesRelatedInstances = "";	//
	std::string XmlFilePath = "";					//���series�е�xml�ļ��ľ����ļ�·��
	std::vector<DicomImage*> ImageList;			//��Series�е�ͼƬ�洢���������
};

