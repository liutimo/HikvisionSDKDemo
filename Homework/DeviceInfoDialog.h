#pragma once
#include <QDialog>
#include "DeviceInfo.h"
namespace Ui 
{
	class Dialog;
}

class DeviceInfoDialog :
	public QDialog
{
	Q_OBJECT
public:
	explicit DeviceInfoDialog(const DeviceInfo &deviceInfo, QWidget *parent = nullptr);
	~DeviceInfoDialog();

signals:
	void modifyDeviceInfo(DeviceInfo deviceInfo);

private:
	void _Init();
	
	Ui::Dialog *ui;
	DeviceInfo m_deviceInfo;
};

