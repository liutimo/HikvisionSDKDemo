#include "ChannelInfo.h"
#include "DeviceInfo.h"
 
ChannelInfo::ChannelInfo()
{
	m_channel_num = -1;
	m_realhandle = -1;
} 

ChannelInfo::~ChannelInfo()
{

}

 
void ChannelInfo::setChannelName(QString name)
{
	m_channel_name = name;
}

 
QString ChannelInfo::getChannelName() const
{
	return m_channel_name;
}
 
void ChannelInfo::setChannelNum(int num)
{
	m_channel_num = num;
}
 
int ChannelInfo::getChannelNum() const
{
	return m_channel_num;
}



void ChannelInfo::setRealhandle(int num)
{
	m_realhandle = num;
}

int ChannelInfo::getRealhandle() const
{
	return m_realhandle;
}

void ChannelInfo::setDevice(DeviceInfo *deviceInfo)
{
	m_device = deviceInfo;
}

DeviceInfo* ChannelInfo::getDevice() const
{
	return m_device;
}
