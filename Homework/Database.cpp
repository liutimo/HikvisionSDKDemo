#include "Database.h"
#include <QSqlQuery>
#include <QVariant>
Database::Database()
{
}


Database::~Database()
{
}

int Database::addDevice(const DeviceInfo &device)
{
	QString sql = "INSERT INTO DeviceInfo(DeviceName, IP, Port, UserName, Password) values(?,?,?,?,?);";

	QSqlQuery sqlQuery;
	sqlQuery.prepare(sql);
	sqlQuery.addBindValue(device.getDeviceName());
	sqlQuery.addBindValue(device.getIP());
	sqlQuery.addBindValue(device.getPort());
	sqlQuery.addBindValue(device.getUserName());
	sqlQuery.addBindValue(device.getPasswd());

	bool flag = sqlQuery.exec();
	int ret = -1;

	if (flag)
	{
		ret = sqlQuery.lastInsertId().toInt();
	}

	return ret;
}



void Database::delDevice(int id)
{
	QString querySql1 = "DELETE FROM DeviceInfo WHERE id = ?;";

	QSqlQuery query;
	query.prepare(querySql1);
	query.addBindValue(id);

	query.exec();
}


QVector<DeviceInfo> Database::getDevices()
{
	QString querySql1 = "SELECT * FROM DeviceInfo;";

	QSqlQuery query1;
	query1.exec(querySql1);

	QVector<DeviceInfo> ret;

	while (query1.next())
	{
		DeviceInfo deviceInfo;
		deviceInfo.setDeviceName(query1.value("DeviceName").toString());
		deviceInfo.setIP(query1.value("IP").toString());
		deviceInfo.setPort(query1.value("Port").toInt());
		deviceInfo.setUserName(query1.value("UserName").toString());
		deviceInfo.setPasswd(query1.value("Password").toString());
		deviceInfo.setDeviceId(query1.value("id").toInt());

		int id = query1.value("id").toInt();

		QString querySql2 = "SELECT * FROM ChannelInfo WHERE DeviceID = " + QString::number(id);
		QSqlQuery query2;
		query2.exec(querySql2);

		while (query2.next())
		{
			ChannelInfo channelInfo;
			channelInfo.setChannelName(query2.value("ChannelName").toString());
			deviceInfo.addChannel(channelInfo);
		}
		ret.push_back(deviceInfo);
	}

	return ret;
}

void Database::addChannel(const ChannelInfo &channelInfo)
{
	QString sql = "INSERT INTO ChannelInfo(DeviceID, ChannelName) values(?,?);";

	QSqlQuery query;
	query.prepare(sql);
	query.addBindValue(channelInfo.getDevice()->getDeviceId());
	query.addBindValue(channelInfo.getChannelName());
	query.exec();
}