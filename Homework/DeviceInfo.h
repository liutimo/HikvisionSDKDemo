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
	int		m_alarmHandle;							//����������
	bool	m_working;								//�豸���ڹ���
	bool    m_isFirstLogin;							//��¼�Ƿ��ǵ�һ�ε�¼
	LONG	m_login_id;								//NET_DVR_Login_V40()  ����ֵ
	QString m_devicename;							//�豸����
	QString m_ip;									//�豸IP
	int     m_port;									//�˿ں�
	QString m_username;								//�û���
	QString m_password;								//����					
	QList<ChannelInfo>		m_listChannel;			//���豸�µ�����ͨ��
	NET_DVR_DEVICEINFO_V30	m_deviceinfo;			//NET_DVR_Login_V40() �л�ȡ�����豸��Ϣ
};
