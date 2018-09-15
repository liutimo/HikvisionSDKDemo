#include "AddDeviceDialog.h"
#include "ui_AddDevice.h"
#include "Utils.h"
#include <QMessageBox>

AddDeviceDialog::AddDeviceDialog(QWidget *parent) :
QDialog(parent),
ui(new Ui::AddDevice)
{
	ui->setupUi(this);
	resize(300, 400);

	connect(ui->btnAdd, &QPushButton::clicked, this, &AddDeviceDialog::onAddDevice);
	connect(ui->btnClose, &QPushButton::clicked, this, &AddDeviceDialog::close);
}


AddDeviceDialog::~AddDeviceDialog()
{
	delete ui;
}


void AddDeviceDialog::onAddDevice()
{
	DeviceInfo deviceInfo;
	deviceInfo.setDeviceName(ui->nodeName->text());
	deviceInfo.setUserName(ui->userName->text());
	if (deviceInfo.getUserName().isEmpty())
	{
		QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("用户名不能为空"), QMessageBox::Ok);
		return;
	}

	deviceInfo.setPasswd(ui->password->text());
	if (deviceInfo.getPasswd().isEmpty())
	{
		QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("密码不能为空"), QMessageBox::Ok);
		return;
	}

	QString ipAddr = ui->ipAddr->text();
	if (Utils::isIP(ipAddr))
	{
		deviceInfo.setIP(ipAddr);

		//如果节点名称为空，则默认为IP地址
		if (deviceInfo.getDeviceName().isEmpty())
		{
			deviceInfo.setDeviceName(ipAddr);
		}
	}
	else
	{
		QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("IP地址格式错误"), QMessageBox::Ok);
		return;
	}

	if (ui->port->text().isEmpty())
	{
		QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("端口号不能为空"), QMessageBox::Ok);
		return;
	}
	deviceInfo.setPort(ui->port->text().toUInt());

	emit addDevice(deviceInfo);
	close();
}