#include "DeviceInfo.h"

DeviceInfo::DeviceInfo()
{
	m_working	= false;
	m_isFirstLogin = true;
	m_login_id	= -1;
	memset(&m_deviceinfo, 0, sizeof(NET_DVR_DEVICEINFO_V30));
	m_port = 0;
}

DeviceInfo::~DeviceInfo()
{

}

void DeviceInfo::setWorkable(bool working)
{
	m_working = working;
}

bool DeviceInfo::isWorking() const
{
	return m_working;
}

void DeviceInfo::setFirstLogin(bool flag)
{
	m_isFirstLogin = flag;
}

bool DeviceInfo::isFirstLogin() const
{
	return m_isFirstLogin;
}

void DeviceInfo::setLoginId(int id)
{
	m_login_id = id;
}

int DeviceInfo::getLoginID() const
{
	return m_login_id;
}

void DeviceInfo::setDeviceName(QString devicename)
{
	m_devicename = devicename;
}


QString DeviceInfo::getDeviceName() const
{
	return m_devicename;
}

void DeviceInfo::setIP(QString  ip)
{
	m_ip = ip;
}

QString DeviceInfo::getIP() const
{
	return m_ip;
}

void DeviceInfo::setPort(int port)
{
	m_port = port;
}

int DeviceInfo::getPort() const
{
	return m_port;
}

void DeviceInfo::setUserName(QString usrname)
{
	m_username = usrname;
}

QString DeviceInfo::getUserName() const
{
	return m_username;
}

void DeviceInfo::setPasswd(QString passwd)
{
	m_password = passwd;
}

QString DeviceInfo::getPasswd() const
{
	return m_password;
}

void DeviceInfo::addChannel(const ChannelInfo& channel)
{
	m_listChannel.append(channel);
}

QList<ChannelInfo> DeviceInfo::getChannels()
{
	return m_listChannel;
}

void DeviceInfo::setDeviceInfo(NET_DVR_DEVICEINFO_V30 info)
{
	m_deviceinfo = info;
}

NET_DVR_DEVICEINFO_V30 DeviceInfo::getDeviceInfo() const
{
	return m_deviceinfo;
}

void DeviceInfo::setDeviceId(int id)
{
	m_deviceId = id;
}

int DeviceInfo::getDeviceId() const 
{
	return m_deviceId;
}

void DeviceInfo::setAlarmHandle(int alarmHandle)
{
	m_alarmHandle = alarmHandle;
}

int DeviceInfo::getAlarmHandle() const
{
	return m_alarmHandle;
}