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
	QString		m_channel_name;			//通道名称			
	int			m_channel_num;			//通道号
	int			m_realhandle;			//NET_DVR_RealPlay_V40 返回值
	DeviceInfo  *m_device;				//该通道所属的设备
};