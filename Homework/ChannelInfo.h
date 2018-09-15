#pragma once
#include <QString>


QT_BEGIN_NAMESPACE
class DeviceInfo;
QT_END_NAMESPACE

class ChannelInfo
{

public:
	ChannelInfo();
	~ChannelInfo();

	void setChannelName(QString name);
	QString getChannelName() const;

	void setChannelNum(int num);
	int getChannelNum() const;

	void setRealhandle(int num);
	int getRealhandle() const;

	void setDevice(DeviceInfo *deviceInfo);
	DeviceInfo* getDevice() const;

private:
	QString		m_channel_name;			//ͨ������			
	int			m_channel_num;			//ͨ����
	int			m_realhandle;			//NET_DVR_RealPlay_V40 ����ֵ
	DeviceInfo  *m_device;				//��ͨ���������豸
};