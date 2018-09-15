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

	//启动程序时初始化SDK
	if (!NET_DVR_Init())
	{
		//初始化SDK失败
		return -1;
	}

	//设置连接时间与重连时间
	NET_DVR_SetConnectTime(2000, 1);
	NET_DVR_SetReconnect(10000, true);

	//注册报警消息回调信息
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
