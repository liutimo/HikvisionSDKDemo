#include "DeviceInfoDialog.h"
#include "ui_DeviceInfoDialog.h"

DeviceInfoDialog::DeviceInfoDialog(const DeviceInfo &deviceInfo, QWidget *parent) : QDialog(parent),
ui(new Ui::Dialog)
{
	ui->setupUi(this);
	m_deviceInfo = deviceInfo;
	_Init();
}


DeviceInfoDialog::~DeviceInfoDialog()
{
	delete ui;
}

void DeviceInfoDialog::_Init()
{
	ui->deviceName->setText(m_deviceInfo.getDeviceName());
	ui->channelNumber->setText(QString::number(m_deviceInfo.getDeviceInfo().byChanNum));
	ui->ipAddr->setText(m_deviceInfo.getIP());
	ui->port->setText(QString::number(m_deviceInfo.getPort()));
	ui->userName->setText(m_deviceInfo.getUserName());
	ui->password->setText(m_deviceInfo.getPasswd());
	ui->serialNumber->setText(QString((char*)m_deviceInfo.getDeviceInfo().sSerialNumber));

	connect(ui->btnOk, &QPushButton::clicked, this, [this]()
	{
		m_deviceInfo.setDeviceName(ui->deviceName->text());
		m_deviceInfo.setIP(ui->ipAddr->text());
		m_deviceInfo.setPort(ui->port->text().toInt());
		m_deviceInfo.setUserName(ui->userName->text());
		m_deviceInfo.setPasswd(ui->password->text());

		emit modifyDeviceInfo(m_deviceInfo);
	});
}
