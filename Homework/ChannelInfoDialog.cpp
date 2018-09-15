#include "ChannelInfoDialog.h"
#include "ui_ChannelInfoDialog.h"
#include "DeviceInfo.h"

ChannelInfoDialog::ChannelInfoDialog(const ChannelInfo &channelInfo, QWidget *parent) : QDialog(parent),
ui(new Ui::ChannelInfoDialog)
{
	ui->setupUi(this);
	m_channelInfo = channelInfo;
	_Init();
}


ChannelInfoDialog::~ChannelInfoDialog()
{
	delete ui;
}

void ChannelInfoDialog::_Init()
{
	ui->channelName->setText(m_channelInfo.getChannelName());
	ui->channelNumber->setText(QString::number(m_channelInfo.getChannelNum()));
	ui->deviceName->setText(m_channelInfo.getDevice()->getDeviceName());
	ui->ipAddr->setText(m_channelInfo.getDevice()->getIP());

	connect(ui->okBtn, &QPushButton::clicked, this, [this](){
		m_channelInfo.setChannelName(ui->channelName->text());
		emit modifyChannelInfo(m_channelInfo);
		close();
	});
}
