#include "DicomDir.h"

#include <dcmdata/dcdicdir.h>
#include <dcmdata/dcdeftag.h>
#include <QTableWidget>
#include <QDebug>
#include<QFileSystemModel>

#include<iomanip>
#include<fstream>
#include "DicomDataBase.h"

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
	DicomDataBase* m_database =DicomDataBase::getInstance();
	m_database->Init(DicomDirFilePath.toStdString());
	/*
	 *������
	 */
	if (m_database->PatientList.size()==0)
	{
		return;
	}else
	{
		//����,������
		ui.tableWidget->setColumnCount(4);
		ui.tableWidget->setRowCount(m_database->PatientList.size());//����
		ui.tableWidget->setHorizontalHeaderLabels(
			QStringList() <<  "Patient ID" << "Patient Name" << "Birth Date" << "Gender");
		ui.tableWidget->verticalHeader()->setVisible(false); // ����ˮƽheader
		//ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectItems);   // ����ѡ��
		ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
		//ui.tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);  // ����ѡ�ж�� 
		ui.tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
		/* ���� Id Name �ֶε�ֵ�����޸� */
		/* ���ò����޸ĵ�Id Name���еı�����ɫΪ��ɫ */
		for (int i = 0; i < 4; i++) {
			QTableWidgetItem *item = new QTableWidgetItem();
			item->setBackground(QBrush(QColor(Qt::lightGray)));
			item->setFlags(item->flags() & (~Qt::ItemIsEditable));
			ui.tableWidget->setItem(i, 0, item);
		}
		for (int i = 0; i < 4; i++) {
			QTableWidgetItem *item = new QTableWidgetItem();
			item->setBackground(QBrush(QColor(Qt::lightGray)));
			item->setFlags(item->flags() & (~Qt::ItemIsEditable));
			ui.tableWidget->setItem(i, 1, item);
		}
		//������ݿ�ʼ
		for (int i = 0; i < ui.tableWidget->rowCount(); i++) {
			QTableWidgetItem *item_2 = new QTableWidgetItem();
			item_2->setText(QString::fromStdString(m_database->PatientList[i]->PatientID));
			ui.tableWidget->setItem(i, 0, item_2);
		}

		for (int i = 0; i < ui.tableWidget->rowCount(); i++) {
			QTableWidgetItem *item_max = new QTableWidgetItem();
			item_max->setText(QString::fromStdString(m_database->PatientList[i]->PatientName));
			item_max->setBackground(QBrush(QColor(Qt::lightGray)));
			item_max->setFlags(item_max->flags() & (~Qt::ItemIsEditable));
			ui.tableWidget->setItem(i, 1, item_max);

			QTableWidgetItem *item_min = new QTableWidgetItem();
			item_min->setText(QString::fromStdString(m_database->PatientList[i]->StudyList[0]->PatientBirthDate));
			item_min->setBackground(QBrush(QColor(Qt::lightGray)));
			item_min->setFlags(item_min->flags() & (~Qt::ItemIsEditable));
			ui.tableWidget->setItem(i, 2, item_min);

			QTableWidgetItem *item_type = new QTableWidgetItem();
			item_type->setText(QString::fromStdString(m_database->PatientList[i]->StudyList[0]->PatientSex));
			item_type->setBackground(QBrush(QColor(Qt::lightGray)));
			item_type->setFlags(item_type->flags() & (~Qt::ItemIsEditable));
			ui.tableWidget->setItem(i, 3, item_type);
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
