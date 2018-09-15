#pragma once
#include <QDialog>
#include "DeviceInfo.h"
namespace Ui
{
	class AddDevice;
}

class AddDeviceDialog :
	public QDialog
{
	Q_OBJECT
public:
 	explicit AddDeviceDialog(QWidget *parent = nullptr);
	~AddDeviceDialog();
	
signals:
	void addDevice(DeviceInfo device);

private slots:
	void onAddDevice();

private:
	Ui::AddDevice *ui;
};

