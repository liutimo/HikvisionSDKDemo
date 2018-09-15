#pragma once
#include <QDialog>
#include "ChannelInfo.h"
namespace Ui 
{
	class ChannelInfoDialog;
}

class ChannelInfoDialog :
	public QDialog
{
	Q_OBJECT
public:
	explicit ChannelInfoDialog(const ChannelInfo &channelInfo, QWidget *parent = nullptr);
	~ChannelInfoDialog();

signals:
	void modifyChannelInfo(ChannelInfo channelInfo);
private:
	void _Init();
	
	Ui::ChannelInfoDialog *ui;
	ChannelInfo m_channelInfo;
};

