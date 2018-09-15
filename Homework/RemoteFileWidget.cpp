#include "RemoteFileWidget.h"
#include "ui_RemoteFileWidget.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDate>
#include <QMessageBox>
#include <QTimer>


RemoteFileWidget::RemoteFileWidget(PlayBackRemoteFileType type, QWidget *parent) :
QWidget(parent), ui(new Ui::RemoteFileWidget), m_userId(-1), m_playHandle(-1), m_playType(type)
{
	ui->setupUi(this);

	_Init();
}

RemoteFileWidget::~RemoteFileWidget()
{
	delete ui;
}


void RemoteFileWidget::setMainWindow(MainWindow *main)
{
	m_mainWindow = main;
}

/*******************槽***********************/
void RemoteFileWidget::onFindFileClicked()
{
	ui->tableWidget->clear();
	ui->tableWidget->setRowCount(0);

	int channelNumber = _GetCurrentChannelNumer();
	if (channelNumber == -1)
	{
		return;
	}
	_FindFileByName(channelNumber);
}

void RemoteFileWidget::onVoiceChange(int value)
{
	NET_DVR_PlayBackControl_V40(m_playHandle, NET_DVR_PLAYAUDIOVOLUME, &value, sizeof(int), nullptr, nullptr);
}

void RemoteFileWidget::onPlayback()
{
	//当前有视频正在播放
	if (m_playHandle != -1)
	{
		//正在播放
		if (m_isPlaying)
		{
			//暂停
			if (_SetPlayBackControlCode(NET_DVR_PLAYPAUSE))
			{
				ui->playBtn->setIcon(QIcon(":/Homework/Resources/play.png")); 
				m_isPlaying = false;
			}
		}
		else
		{
			//播放
			if (_SetPlayBackControlCode(NET_DVR_PLAYNORMAL))
			{
				ui->playBtn->setIcon(QIcon(":/Homework/Resources/pause2.png"));
				m_isPlaying = true;
			}
		}

		return;
	}
	else if (m_playType == PLAYBYNAME)
	{
		int row = ui->tableWidget->currentRow();
		QTableWidgetItem *item = ui->tableWidget->item(row, 0);
		if (item == nullptr)
		{
			return;
		}
		ui->playBtn->setIcon(QIcon(":/Homework/Resources/pause2.png"));
		_Playback(item->text());
	}
	else if (m_playType == PLAYBYTIME)
	{
		_PlayByTime(); 
		ui->playBtn->setIcon(QIcon(":/Homework/Resources/pause2.png"));
	}

	//打开声音
	if (_SetPlayBackControlCode(NET_DVR_PLAYSTARTAUDIO) == false)
	{
		m_mainWindow->ui->statusbar->showMessage("打开声音失败");
	}
		
}


void RemoteFileWidget::onSlowPlay()
{
	_SetPlayBackControlCode(NET_DVR_PLAYSLOW);
}

void RemoteFileWidget::onFastPlay()
{
	_SetPlayBackControlCode(NET_DVR_PLAYFAST);
}

/******************私有函数*********************/
void RemoteFileWidget::_Init()
{
	m_timer = new QTimer(this);
	connect(m_timer, &QTimer::timeout, this, [this](){
		int value = 0;
		_SetPlayBackControlCode(NET_DVR_PLAYGETPOS, value);
		ui->processSlider->setValue(value);
	});

	ui->startDate->setDate(QDate::currentDate());
	ui->endDate->setDate(QDate::currentDate());
	ui->startTime->setTime(QTime::fromString("00:00"));
	ui->endTime->setTime(QTime::fromString("23:59"));

	
	if (m_playType == PLAYBYTIME)
	{
		ui->processSlider->hide();
		ui->label_2->hide();
		ui->label_4->hide();
		ui->videoType->hide();
		ui->fileAtrribute->hide();
		ui->tableWidget->setEnabled(false);
		ui->findBtn->setEnabled(false);
	}

	ui->tableWidget->horizontalHeader()->setStretchLastSection(true);		//最后一列自动填充
	ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->tableWidget->setAlternatingRowColors(true);							//奇数偶数行不同背景色
	ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);	//选中整行,每次只允许选中一行
	ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

	//关联槽
	connect(ui->findBtn, &QPushButton::clicked, this, &RemoteFileWidget::onFindFileClicked);
	connect(ui->playBtn, &QPushButton::clicked, this, &RemoteFileWidget::onPlayback);
	connect(ui->stopBtn, &QPushButton::clicked, this, &RemoteFileWidget::_StopPlayBack);
	connect(ui->fastPlayBtn, &QPushButton::clicked, this, &RemoteFileWidget::onFastPlay);
	connect(ui->slowPlayBtn, &QPushButton::clicked, this, &RemoteFileWidget::onSlowPlay);
	connect(ui->voiceSlider, &QSlider::valueChanged, this, &RemoteFileWidget::onVoiceChange);
}

int RemoteFileWidget::_GetCurrentChannelNumer()
{
	// 获取当前选中的item对应的通道号
	auto item = m_mainWindow->ui->treeWidget->currentItem();
	auto iter = m_mainWindow->m_channels.find(item);
	//auto iter = m_mainWindow->m_channels.find(m_mainWindow->m_currentItem);
	if (iter == m_mainWindow->m_channels.end())
	{
		QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("请选择一个通道"), QMessageBox::Ok);
		return -1;
	}
	int channelNumber = (*iter).getChannelNum();
	m_userId = (*iter).getDevice()->getLoginID();
	return channelNumber;
}

void RemoteFileWidget::_AddRow(const QString &fileName, int fileSize)
{
	QTableWidgetItem *item = new QTableWidgetItem(fileName);
	QTableWidgetItem *sizeItem = new QTableWidgetItem(QString::number(fileSize));

	int row = ui->tableWidget->rowCount();
	ui->tableWidget->setRowCount(row + 1);
	ui->tableWidget->setItem(row, 0, item);
	ui->tableWidget->setItem(row, 1, sizeItem);
}

void RemoteFileWidget::_Playback(const QString &fileName)
{
	//停止当前正在播放的录像
	//_StopPlayBack();

	QByteArray byteArray = fileName.toLatin1();

	m_playHandle = NET_DVR_PlayBackByName(m_userId, byteArray.data(), (HWND)ui->playFrame->winId());

	//开始播放
	if (_SetPlayBackControlCode(NET_DVR_PLAYSTART) == false)
	{
		m_mainWindow->ui->statusbar->showMessage("播放录像失败");
		_StopPlayBack();
	}
	m_isPlaying = true;

	//开启定时器1s触发一次
	m_timer->start(1000);

}


void RemoteFileWidget::_StopPlayBack()
{
	NET_DVR_StopPlayBack(m_playHandle);
	m_timer->stop();
	m_playHandle = -1;
	ui->playFrame->hide();
	ui->playFrame->show();
	ui->playBtn->setIcon(QIcon(":/Homework/Resources/play.png"));
}


bool RemoteFileWidget::_SetPlayBackControlCode(unsigned int controlCode)
{
	return NET_DVR_PlayBackControl_V40(m_playHandle, controlCode, nullptr, 0, nullptr, nullptr);
}

bool RemoteFileWidget::_SetPlayBackControlCode(unsigned int controlCode, int &value)
{
	return NET_DVR_PlayBackControl_V40(m_playHandle, controlCode, nullptr, 0, &value, nullptr);
}

void RemoteFileWidget::_FindFileByName(int channelNumber)
{
	QDate startDate = ui->startDate->date();
	QTime startTime = ui->startTime->time();
	QDate endDate = ui->endDate->date();
	QTime endTime = ui->endTime->time();

	NET_DVR_FILECOND_V40 fileCond = { 0 };
	fileCond.lChannel = channelNumber;
	fileCond.dwFileType = 0xFF;
	fileCond.dwIsLocked = 0xFF;
	fileCond.dwUseCardNo = 0;
	fileCond.struStartTime.dwYear = startDate.year(); //开始时间
	fileCond.struStartTime.dwMonth = startDate.month();
	fileCond.struStartTime.dwDay = startDate.day();
	fileCond.struStartTime.dwHour = startTime.hour();
	fileCond.struStartTime.dwMinute = startTime.minute();
	fileCond.struStartTime.dwSecond = startTime.second();
	fileCond.struStopTime.dwYear = endDate.year(); //结束时间
	fileCond.struStopTime.dwMonth = endDate.month();
	fileCond.struStopTime.dwDay = endDate.day();
	fileCond.struStopTime.dwHour = endTime.hour();
	fileCond.struStopTime.dwMinute = endTime.minute();
	fileCond.struStopTime.dwSecond = endTime.second();


	LONG findHandle = NET_DVR_FindFile_V40(m_userId, &fileCond);
	if (findHandle == -1)
	{
		//no file
		return;
	}

	NET_DVR_FINDDATA_V40  fileData;
	while (true)
	{
		int result = NET_DVR_FindNextFile_V40(findHandle, &fileData);
		if (result == NET_DVR_ISFINDING) //正在查找文件
		{
			continue;
		}
		else if (result == NET_DVR_FILE_SUCCESS) //获得文件信息
		{
			_AddRow(fileData.sFileName, fileData.dwFileSize);
			break;
		}
		else
		{
			break;
		}
	}
	if (findHandle >= 0)
	{
		NET_DVR_FindClose_V30(findHandle);
	}
}


void RemoteFileWidget::_PlayByTime()
{
	int channelNumber = _GetCurrentChannelNumer();
	if (channelNumber == -1)
	{
		return;
	}

	QDate startDate = ui->startDate->date();
	QTime startTime = ui->startTime->time();
	QDate endDate = ui->endDate->date();
	QTime endTime = ui->endTime->time();

	NET_DVR_VOD_PARA struVodPara = { 0 };
	struVodPara.dwSize = sizeof(struVodPara);
	struVodPara.struIDInfo.dwChannel = channelNumber; //通道号
	struVodPara.hWnd = (HWND)ui->playFrame->winId(); //回放窗口
	struVodPara.struBeginTime.dwYear = startDate.year(); //开始时间
	struVodPara.struBeginTime.dwMonth = startDate.month();
	struVodPara.struBeginTime.dwDay = startDate.day();
	struVodPara.struBeginTime.dwHour = startTime.hour();
	struVodPara.struBeginTime.dwMinute = startTime.minute();
	struVodPara.struBeginTime.dwSecond = startTime.second();
	struVodPara.struEndTime.dwYear = endDate.year(); //结束时间
	struVodPara.struEndTime.dwMonth = endDate.month();
	struVodPara.struEndTime.dwDay = endDate.day();
	struVodPara.struEndTime.dwHour = endTime.hour();
	struVodPara.struEndTime.dwMinute = endTime.minute();
	struVodPara.struEndTime.dwSecond = endTime.second();

	m_playHandle = NET_DVR_PlayBackByTime_V40(m_userId, &struVodPara);
	if (m_playHandle < 0)
	{
		m_mainWindow->ui->statusbar->showMessage("播放失败");
		return;
	}

	//开始播放
	if (_SetPlayBackControlCode(NET_DVR_PLAYSTART) == false)
	{
		m_mainWindow->ui->statusbar->showMessage("播放录像失败");
		_StopPlayBack();
	}
	m_isPlaying = true;
}