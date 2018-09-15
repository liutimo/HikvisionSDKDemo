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
		QMessageBox::warning(this, QStringLiteral("��ʾ"), QStringLiteral("�û�������Ϊ��"), QMessageBox::Ok);
		return;
	}

	deviceInfo.setPasswd(ui->password->text());
	if (deviceInfo.getPasswd().isEmpty())
	{
		QMessageBox::warning(this, QStringLiteral("��ʾ"), QStringLiteral("���벻��Ϊ��"), QMessageBox::Ok);
		return;
	}

	QString ipAddr = ui->ipAddr->text();
	if (Utils::isIP(ipAddr))
	{
		deviceInfo.setIP(ipAddr);

		//����ڵ�����Ϊ�գ���Ĭ��ΪIP��ַ
		if (deviceInfo.getDeviceName().isEmpty())
		{
			deviceInfo.setDeviceName(ipAddr);
		}
	}
	else
	{
		QMessageBox::warning(this, QStringLiteral("��ʾ"), QStringLiteral("IP��ַ��ʽ����"), QMessageBox::Ok);
		return;
	}

	if (ui->port->text().isEmpty())
	{
		QMessageBox::warning(this, QStringLiteral("��ʾ"), QStringLiteral("�˿ںŲ���Ϊ��"), QMessageBox::Ok);
		return;
	}
	deviceInfo.setPort(ui->port->text().toUInt());

	emit addDevice(deviceInfo);
	close();
}