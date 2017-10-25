#include "DicomDir.h"

#include <dcmdata/dcdicdir.h>
#include <dcmdata/dcdeftag.h>
#include <QTableWidget>
#include <QDebug>
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
	
	//std::filebuf fb;
	//fb.open("DicomDir.log", std::ios::out);//�����־�ļ�
	//std::ostream out(&fb);
	//dicomdir.print(out);
	//fb.close();
 // 
	//DcmFileFormat DCMFF = dicomdir.getDirFileFormat();
	//std::filebuf lb;
	//lb.open("DcmFileFormat.log", std::ios::out);//�����־�ļ�2
	//std::ostream out2(&lb);
	//DCMFF.print(out2);
	//lb.close();

	DicomDirRootRecord = &(dicomdir.getRootRecord());
	
	bool flag = DicomDirRootRecord->isEmpty();
	DcmDirectoryRecord *   PatientRecord = NULL;
	OFString tmpString;
	
	std::vector<PatientMsg*>*data=new std::vector<PatientMsg*>();//
	PatientMsg * temp_patient_msg;
	int row = 0;//����

	while (((PatientRecord = DicomDirRootRecord->getSub(row)) != NULL))
	{
		temp_patient_msg=new PatientMsg();

		if (PatientRecord->findAndGetOFString(DCM_ReferencedFileID, tmpString).good())//DCM_PatientName
		{
			temp_patient_msg->FileID = (tmpString.empty() == true ? "��" : tmpString.c_str());
			qDebug() << "FileID : " << temp_patient_msg->FileID;
		}
		if (PatientRecord->findAndGetOFString(DCM_PatientID, tmpString).good())
		{
			temp_patient_msg->PatientID = (tmpString.empty() == true ? "��" : tmpString.c_str());
			qDebug() << "PatientID:" << temp_patient_msg->PatientID;
		}
		if (PatientRecord->findAndGetOFString(DCM_PatientName, tmpString).good())
		{
			temp_patient_msg->PatientName = (tmpString.empty() == true ? "��" : tmpString.c_str());
			qDebug() << "PatientName:" << temp_patient_msg->PatientName;
		}
		if (PatientRecord->findAndGetOFString(DCM_PatientBirthDate, tmpString).good())
		{
			temp_patient_msg->BirthDate = (tmpString.empty() == true ? "��" : tmpString.c_str());
			qDebug() << "BirthDate:" << temp_patient_msg->BirthDate;
		}
		if (PatientRecord->findAndGetOFString(DCM_PatientSex, tmpString).good())
		{
			temp_patient_msg->Gender = (tmpString.empty() == true ? "��" : tmpString.c_str());
			qDebug() << "Gender:" << temp_patient_msg->Gender;
		}
		row = row + 1;
		data->push_back(temp_patient_msg);//ѹһ�����˵�����
		//delete(temp_patient_msg);
	}

	for(int i=0;i<data->size();i++)
	{
		qDebug() << QStringLiteral("��") << i << QStringLiteral("������:");
		qDebug() << "FileID : " << (*data)[i]->FileID;
		qDebug() << "PatientID : " << (*data)[i]->PatientID;
		qDebug() << "PatientName : " << (*data)[i]->PatientName;
		qDebug() << "BirthDate : " << (*data)[i]->BirthDate;
		qDebug() << "Gender : " << (*data)[i]->Gender;
	}
	if (row<=0)
	{
		//��,�����˳�
	}else
	{
		//����,������
		ui.tableWidget->setColumnCount(5);
		ui.tableWidget->setRowCount(row);//����
		ui.tableWidget->setHorizontalHeaderLabels(
			QStringList() << "File ID" << "Patient ID" << "Patient Name" << "Birth Date" << "Gender");
		ui.tableWidget->verticalHeader()->setVisible(false); // ����ˮƽheader
		//ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectItems);   // ����ѡ��
		ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
		//ui.tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);  // ����ѡ�ж�� 
		ui.tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
		/* ���� Id Name �ֶε�ֵ�����޸� */
		/* ���ò����޸ĵ�Id Name���еı�����ɫΪ��ɫ */
		for (int i = 0; i < 5; i++) {
			QTableWidgetItem *item = new QTableWidgetItem();
			item->setBackground(QBrush(QColor(Qt::lightGray)));
			item->setFlags(item->flags() & (~Qt::ItemIsEditable));
			ui.tableWidget->setItem(i, 0, item);
		}
		for (int i = 0; i < 5; i++) {
			QTableWidgetItem *item = new QTableWidgetItem();
			item->setBackground(QBrush(QColor(Qt::lightGray)));
			item->setFlags(item->flags() & (~Qt::ItemIsEditable));
			ui.tableWidget->setItem(i, 1, item);
		}
		//������ݿ�ʼ
		for (int i = 0; i < ui.tableWidget->rowCount(); i++) {
			QTableWidgetItem *item_1 = new QTableWidgetItem();
			item_1->setText((*data)[i]->FileID);
			ui.tableWidget->setItem(i, 0, item_1);

			QTableWidgetItem *item_2 = new QTableWidgetItem();
			item_2->setText((*data)[i]->PatientID);
			ui.tableWidget->setItem(i, 1, item_2);
		}

		for (int i = 0; i < ui.tableWidget->rowCount(); i++) {
			QTableWidgetItem *item_max = new QTableWidgetItem();
			item_max->setText((*data)[i]->PatientName);
			item_max->setBackground(QBrush(QColor(Qt::lightGray)));
			item_max->setFlags(item_max->flags() & (~Qt::ItemIsEditable));
			ui.tableWidget->setItem(i, 2, item_max);

			QTableWidgetItem *item_min = new QTableWidgetItem();
			item_min->setText((*data)[i]->BirthDate);
			item_min->setBackground(QBrush(QColor(Qt::lightGray)));
			item_min->setFlags(item_min->flags() & (~Qt::ItemIsEditable));
			ui.tableWidget->setItem(i, 3, item_min);

			QTableWidgetItem *item_type = new QTableWidgetItem();
			item_type->setText((*data)[i]->Gender);
			item_type->setBackground(QBrush(QColor(Qt::lightGray)));
			item_type->setFlags(item_type->flags() & (~Qt::ItemIsEditable));
			ui.tableWidget->setItem(i, 4, item_type);
		}
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
