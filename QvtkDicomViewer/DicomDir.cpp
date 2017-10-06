#include "DicomDir.h"

#include <dcmdata/dcdicdir.h>
#include <dcmdata/dcdeftag.h>

/*
 *Ĭ�Ϲ��췽��
 */
DicomDir::DicomDir(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}
/*
 *���ι��췽��,����DicomDIR�ļ��ľ���·��
 */
DicomDir::DicomDir(QString DicomDirFilePath, QWidget* parent)
{
	ui.setupUi(this);

	//TODO:�û���ʼ����������

	/*=============����ȫ�ֱ�����===============*/
	DcmDirectoryRecord * DicomDirRootRecord;//������ȡ���ļ��ĸ�Ŀ¼
	/*==========================================*/

	DcmDicomDir dicomdir(DicomDirFilePath.toStdString().c_str());
	
	std::filebuf fb;
	fb.open("DicomDir.log", std::ios::out);//�����־�ļ�
	std::ostream out(&fb);
	dicomdir.print(out);
	fb.close();

	DcmFileFormat DCMFF = dicomdir.getDirFileFormat();
	std::filebuf lb;
	lb.open("DcmFileFormat.log", std::ios::out);//�����־�ļ�2
	std::ostream out2(&lb);
	DCMFF.print(out2);
	lb.close();

	DicomDirRootRecord = &(dicomdir.getRootRecord());
	
	bool flag = DicomDirRootRecord->isEmpty();
	DcmDirectoryRecord *   PatientRecord = NULL;
	OFString tmpString;

	int row = 0;//nextSub(PatientRecord)
	while (((PatientRecord = DicomDirRootRecord->getSub(row)) != NULL))
	{
		ui.tableWidget->setRowCount(row + 1);//���һ��
		if (PatientRecord->findAndGetOFString(DCM_ReferencedFileID, tmpString).good())//DCM_PatientName
		{
			ui.tableWidget->setItem(row, 0, new QTableWidgetItem(tmpString.c_str()));
		}
		if (PatientRecord->findAndGetOFString(DCM_PatientID, tmpString).good())
		{
			ui.tableWidget->setItem(row, 1, new QTableWidgetItem(tmpString.c_str()));
		}
		if (PatientRecord->findAndGetOFString(DCM_PatientBirthDate, tmpString).good())
		{
			ui.tableWidget->setItem(row, 2, new QTableWidgetItem(tmpString.c_str()));
		}
		if (PatientRecord->findAndGetOFString(DCM_PatientSex, tmpString).good())
		{
			ui.tableWidget->setItem(row, 3, new QTableWidgetItem(tmpString.c_str()));
		}
		row = row + 1;
	}
}
/*
 * ��������
 */
DicomDir::~DicomDir()
{

}
/*
 * ���ȷ������
 */
void DicomDir::OnPushOk()
{
	int row1 = ui.tableWidget->currentItem()->row();//��ǰѡ����
	emit sendData(ui.tableWidget->item(row1, 1)->text());//�����˵�ID����һ������һ������в��˵Ĳ�ѯ
	//�������δ������岻��
	/*===================�����õ��ı���===================*/
	QString data1, data2, data3, data4, data5;//���岻�����������
	int row;//�������Ĭ��row�б�����Ǳ�������

	/*if (ui.tableWidget1->item(0, 0) != 0) {
		data1 = ui.tableWidget1->item(0, 0)->text();
		for (int j = 1; j < row; j++) {
			data2 = ui.tableWidget->item(j, 0)->text();
			if (data1 == data2) {
				emit sendData(ui.tableWidget->item(j, 1)->text());
			}
		}
	}
	if (ui.tableWidget1->item(0, 1) != 0) {
		data1 = ui.tableWidget1->item(0, 1)->text();
		for (int j = 1; j < row; j++) {
			data2 = ui.tableWidget->item(j, 1)->text();
			if (data1 == data2) {
				emit sendData(ui.tableWidget->item(j, 1)->text());
			}
		}
	}
	if (ui.tableWidget1->item(0, 2) != 0) {
		data1 = ui.tableWidget1->item(0, 2)->text();
		for (int j = 1; j < row; j++) {
			data2 = ui.tableWidget->item(j, 2)->text();
			if (data1 == data2) {
				emit sendData(ui.tableWidget->item(j, 1)->text());
			}
		}
	}
	if (ui.tableWidget1->item(0, 3) != 0) {
		data1 = ui.tableWidget1->item(0, 3)->text();
		for (int j = 1; j < row; j++) {
			data2 = ui.tableWidget->item(j, 3)->text();
			if (data1 == data2) {
				emit sendData(ui.tableWidget->item(j, 1)->text());
			}
		}
	}*/
	this->close();
}
/*
 * ���ȡ������
 */
void DicomDir::OnPushCancel()
{
	this->close();
}
