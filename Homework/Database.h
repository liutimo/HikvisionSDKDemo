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
	* Comments: ���豸��Ϣ���浽���ݿ�
	* Param device :  DeviceInfo
	* @Return  ����int����Ϊ�豸ID��
	*/
	static int addDevice(const DeviceInfo &device);

	/*
	* Comments: ���豸�����ݿ�ɾ��
	* Param id :  addDevice�ķ���ֵ
	* @Return  void
	*/
	static void delDevice(int id);


	/*
	* Comments: ��ѯ�豸
	* Param id :  addDevice�ķ���ֵ
	* @Return  �����豸��Ϣ
	*/
	static QVector<DeviceInfo> getDevices();


	/*
	* Comments: ����ͨ����Ϣ
	* Param id :  ͨ����Ϣ
	* @Return  void
	*/
	static void addChannel(const ChannelInfo &channelInfo);
};

