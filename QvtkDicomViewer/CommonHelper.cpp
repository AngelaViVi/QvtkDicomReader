#include "CommonHelper.h"



CommonHelper::CommonHelper()
{
}


CommonHelper::~CommonHelper()
{
}
/*
 * �������Ա�
 */
void CommonHelper::setStyle(const QString& style)
{
	QFile qss(style);
	qss.open(QFile::ReadOnly);
	qApp->setStyleSheet(qss.readAll());
	qss.close();
}
