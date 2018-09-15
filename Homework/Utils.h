#pragma once
#include <QRegExp>
#include <QSettings>
#include <QDateTime>
#include "ChannelInfo.h"
#include "DeviceInfo.h"
class Utils
{
public:
	//ÅÐ¶ÏÊÇ·ñÊÇIPµØÖ·
	static bool isIP(QString IP) 
	{
		QRegExp RegExp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
		return RegExp.exactMatch(IP);
	}

	static void initSettings()
	{
		s_setting = new  QSettings("homework.ini", QSettings::IniFormat);
	}

	static void setSettingValue(const QString &filed, const QString &value)
	{
		s_setting->setValue(filed, value);
	}

	static QString getSettingValue(const QString &filed)
	{
		return s_setting->value(filed).toString();
	}
	
	/**/
	static QString getVideoFileName(const ChannelInfo &channelInfo)
	{
		QString IP = channelInfo.getDevice()->getIP();
		QString channelName = channelInfo.getChannelName();
		QString currTime = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");

		QString fileName = QString("%1_%2_%3.mp4").arg(IP).arg(channelName).arg(currTime);

		return fileName;
	}


private:
	static QSettings *s_setting;
};

