#pragma once
#include <QString>
#include <QList>
#include <HCNetSDK.h>
#include "ChannelInfo.h"

class DeviceInfo
{
public:
	
	DeviceInfo();
	~DeviceInfo();

	void setWorkable(bool working = true);
	bool isWorking() const;

	void setFirstLogin(bool flag = true);
	bool isFirstLogin() const;

	void setLoginId(int id);
	int getLoginID() const;

	void setDeviceName(QString devicename);
	QString getDeviceName() const;

	void setIP(QString  ip);
	QString getIP() const;

	void setPort(int port);
	int getPort() const;

	void setUserName(QString usrname);
	QString getUserName() const;

	void setPasswd(QString passwd);
	QString getPasswd() const;

	void setDeviceInfo(NET_DVR_DEVICEINFO_V30 info);
	NET_DVR_DEVICEINFO_V30 getDeviceInfo() const;

	void addChannel(const ChannelInfo& channel);
	QList<ChannelInfo> getChannels();

	void setDeviceId(int id);
	int getDeviceId() const;

	void setAlarmHandle(int id);
	int getAlarmHandle() const;

private:
	int		m_deviceId;
	int		m_alarmHandle;							//报警处理句柄
	bool	m_working;								//设备正在工作
	bool    m_isFirstLogin;							//记录是否是第一次登录
	LONG	m_login_id;								//NET_DVR_Login_V40()  返回值
	QString m_devicename;							//设备名称
	QString m_ip;									//设备IP
	int     m_port;									//端口号
	QString m_username;								//用户名
	QString m_password;								//密码					
	QList<ChannelInfo>		m_listChannel;			//该设备下的所有通道
	NET_DVR_DEVICEINFO_V30	m_deviceinfo;			//NET_DVR_Login_V40() 中获取到的设备信息
};
