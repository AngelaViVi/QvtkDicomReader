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
	std::string Manufactuer;					//������
	std::string StationName;					//��������
	std::string SeriseNumber;					//
	std::string NumberOfSeriesRelatedInstance;	//
	std::vector<DicomImage> ImageList;			//��Series�е�ͼƬ�洢���������
};

