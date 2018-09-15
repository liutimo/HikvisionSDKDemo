#include <QtWidgets/QApplication>
#include <HCNetSDK.h>
#include <QSqlDatabase>
#include "MainWindow.h"
#include "Utils.h"
#include "PlayBackWidget.h"
#include "MainPlayBackWidget.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	/*VideoWidget w;
	w.show();*/

	//��������ʱ��ʼ��SDK
	if (!NET_DVR_Init())
	{
		//��ʼ��SDKʧ��
		return -1;
	}

	//��������ʱ��������ʱ��
	NET_DVR_SetConnectTime(2000, 1);
	NET_DVR_SetReconnect(10000, true);

	//ע�ᱨ����Ϣ�ص���Ϣ
	if (NET_DVR_SetDVRMessageCallBack_V31((MSGCallBack_V31)_AlarmInfoCallBack, nullptr) == false)
	{
		return -1;
	}

	QSqlDatabase dbConn = QSqlDatabase::addDatabase("QSQLITE");
	dbConn.setDatabaseName("homework.db");
	if (!dbConn.open())
	{
		return 1;
	}

	Utils::initSettings();

	/*MainWindow w;
	w.show();
*/
	MainPlayBackWidget w;
	w.show();

	return a.exec();
}
