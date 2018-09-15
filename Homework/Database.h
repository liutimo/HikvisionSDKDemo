#pragma once
#include <QObject>
#include <QVector>
#include "DeviceInfo.h"
#include "ChannelInfo.h"

class Database :
	public QObject
{
	Q_OBJECT
public:

	Database();
	~Database();

	/*
	* Comments: 将设备信息保存到数据库
	* Param device :  DeviceInfo
	* @Return  返回int，其为设备ID。
	*/
	static int addDevice(const DeviceInfo &device);

	/*
	* Comments: 将设备从数据库删除
	* Param id :  addDevice的返回值
	* @Return  void
	*/
	static void delDevice(int id);


	/*
	* Comments: 查询设备
	* Param id :  addDevice的返回值
	* @Return  所有设备信息
	*/
	static QVector<DeviceInfo> getDevices();


	/*
	* Comments: 保存通道信息
	* Param id :  通道信息
	* @Return  void
	*/
	static void addChannel(const ChannelInfo &channelInfo);
};

