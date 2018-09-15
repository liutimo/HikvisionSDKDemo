#pragma execution_character_set("utf-8")  
#include "MainWindow.h"
#include "Utils.h"
#include "ui_MainWindow.h"
#include "SettingDialog.h"
#include "AddDeviceDialog.h"
#include "DeviceInfoDialog.h"
#include "ChannelInfoDialog.h"
#include "CapturePictureDialog.h"
#include "DeviceInfo.h"
#include "Database.h"

#include <QDebug>
#include <QTreeWidget>
#include <QMouseEvent>
#include <QMessageBox>
#include <QDateTime>
#include <QScrollBar>

MainWindow *g_mainWindow = nullptr;			//ȫ��ָ��

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent), ui(new Ui::MainWindow), m_capturePictureDialog(nullptr),
m_addDeviceDialog(nullptr), m_channelMaxed(false), m_videoLabelMenu(nullptr),
m_currentItem(nullptr), m_deviceInfoDialog(nullptr)
{
	ui->setupUi(this);
	m_selectedLabel = ui->videoLabel1;
	_InitMenu();
	_InitVideoLabel();

	/*״̬��*/
	connect(ui->addDeviceAction, &QAction::triggered, this, &MainWindow::onShowAddDeviceDialog);
	connect(ui->recordAction, &QAction::triggered, this, &MainWindow::onRecord);
	connect(ui->settingAction, &QAction::triggered, this, [this](){
		SettingDialog *settingDialog = new SettingDialog(this);
		settingDialog->setModal(true);
		settingDialog->show();
	});

	_InitTreeWidget();

	ui->tableWidget->setColumnWidth(0, 150);
	ui->tableWidget->setColumnWidth(1, 350);
	ui->tableWidget->setColumnWidth(2, 150);
	ui->tableWidget->horizontalScrollBar()->hide();
	ui->tableWidget->verticalHeader()->hide();
	
	//���һ���Զ����
	ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
	
	//����ż���в�ͬ����ɫ
	ui->tableWidget->setAlternatingRowColors(true);
	
	//ѡ������,ÿ��ֻ����ѡ��һ��
	ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

	g_mainWindow = this;
}


MainWindow::~MainWindow()
{
	delete ui;

	if (m_addDeviceDialog != nullptr)
	{
		delete m_addDeviceDialog;
	}

	if (m_capturePictureDialog != nullptr)
	{
		delete m_capturePictureDialog;
	}

	if (m_deviceInfoDialog != nullptr)
	{
		delete m_deviceInfoDialog;
	}

	if (m_channelInfoDialog != nullptr)
	{
		delete m_channelInfoDialog;
	}
}


void MainWindow::addLog(const char *msg)
{
	QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
	QString alarmInfo = QString(msg);
	QString deviceInfo = "��ʱ�Ȳ���";

	QTableWidgetItem *item1 = new QTableWidgetItem(time);
	QTableWidgetItem *item2 = new QTableWidgetItem(alarmInfo);
	QTableWidgetItem *item3 = new QTableWidgetItem(deviceInfo);

	int row = ui->tableWidget->rowCount();

	ui->tableWidget->setRowCount(row + 1);

	ui->tableWidget->setItem(row, 0, item1);
	ui->tableWidget->setItem(row, 1, item2);
	ui->tableWidget->setItem(row, 2, item3);

	//ui->tableWidget->setCurrentCell(row, 0);
	ui->tableWidget->scrollToBottom();
}

void MainWindow::_InitMenu()
{
	m_deviceMenu = new QMenu(ui->treeWidget);

	QAction *loginAction = m_deviceMenu->addAction(QStringLiteral("��¼"));
	QAction *logoutAction = m_deviceMenu->addAction(QStringLiteral("ע��"));
	QAction *delAction = m_deviceMenu->addAction(QStringLiteral("�Ƴ�"));
	m_deviceMenu->addSeparator();
	QAction *attrAction = m_deviceMenu->addAction(QStringLiteral("����"));

	connect(loginAction, &QAction::triggered, this, &MainWindow::onLogin);
	connect(logoutAction, &QAction::triggered, this, &MainWindow::onLogout);
	connect(delAction, &QAction::triggered, this, &MainWindow::onDelete);
	connect(attrAction, &QAction::triggered, this, &MainWindow::onShowAttribute);

	m_channelMenu = new QMenu(ui->treeWidget);
	QAction *capturePictureAction = m_channelMenu->addAction(QStringLiteral("ץͼ"));
	QAction *channelAttrAction = m_channelMenu->addAction(QStringLiteral("ͨ������"));
	
	connect(capturePictureAction, &QAction::triggered, this, &MainWindow::onCapturePicture);
	connect(channelAttrAction, &QAction::triggered, this, &MainWindow::onShowChannelAttribute);


	/*�ò˵���Ҫ���ݲ�ͬ��ͨ����̬�仯�����Ҳ��Ҫ��̬����*/
	/*m_videoLabelMenu = new QMenu(this);

	QAction *closePreview = m_videoLabelMenu->addAction("Ԥ��");
	QAction *record = m_videoLabelMenu->addAction("¼��");
*/

	QAction *record1 = new QAction("��ʼ¼��");
	QAction *record2 = new QAction("��ʼ¼��");
	QAction *record3 = new QAction("��ʼ¼��");
	QAction *record4 = new QAction("��ʼ¼��");

	m_videoActions.insert(ui->videoLabel1, record1);
	m_videoActions.insert(ui->videoLabel2, record2);
	m_videoActions.insert(ui->videoLabel3, record3);
	m_videoActions.insert(ui->videoLabel4, record4);

	connect(record1, &QAction::triggered, this, &MainWindow::onRecord);
	connect(record2, &QAction::triggered, this, &MainWindow::onRecord);
	connect(record3, &QAction::triggered, this, &MainWindow::onRecord);
	connect(record4, &QAction::triggered, this, &MainWindow::onRecord);
}

void MainWindow::_InitVideoLabel()
{
	ui->videoLabel1->installEventFilter(this); 
	ui->videoLabel2->installEventFilter(this); 
	ui->videoLabel3->installEventFilter(this); 
	ui->videoLabel4->installEventFilter(this);
	m_selectedLabel->setStyleSheet(m_selectedStyle);
}


void MainWindow::_InitTreeWidget()
{
	/*�豸��*/
	ui->treeWidget->setHeaderHidden(true);
	ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui->treeWidget, &QTreeWidget::customContextMenuRequested, this, &MainWindow::onShowContextMenu);
	connect(ui->treeWidget, &QTreeWidget::itemDoubleClicked, this, &MainWindow::onPlay);

	QVector<DeviceInfo> devices = Database::getDevices();

	for (DeviceInfo &device : devices)
	{
		QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget, QStringList(device.getDeviceName()));
		m_devices.insert(item, device);

		/*for (ChannelInfo &channel : device.getChannels())
		{
			QTreeWidgetItem *child = new QTreeWidgetItem(item, QStringList(channel.getChannelName()));
			item->addChild(child);
		}*/
	}
}

void MainWindow::_HideAllVideoLabel()
{
	ui->videoLabel1->setVisible(false); 
	ui->videoLabel2->setVisible(false);
	ui->videoLabel3->setVisible(false);
	ui->videoLabel4->setVisible(false);
}

void MainWindow::_ShowAllVideoLabel()
{
	ui->videoLabel1->setVisible(true);
	ui->videoLabel2->setVisible(true);
	ui->videoLabel3->setVisible(true);
	ui->videoLabel4->setVisible(true);
}

bool MainWindow::_DoLogin(DeviceInfo &device)
{
	int errnum;
	return _DoLogin(device, errnum);
}

bool MainWindow::_DoLogin(DeviceInfo &device, int &errnum)
{
	NET_DVR_DEVICEINFO_V30 deviceInfo;
	memset(&deviceInfo, 0, sizeof(NET_DVR_DEVICEINFO_V30));

	/*
	����ΪʲôAPI���������ó�const char *!!!!
	*/

	QByteArray byteArray1 = device.getIP().toUtf8();
	char *ipTmp = byteArray1.data();

	QByteArray byteArray2 = device.getUserName().toUtf8();
	char *userNameTmp = byteArray2.data();

	QByteArray byteArray3 = device.getPasswd().toUtf8();
	char *passwordTmp = byteArray3.data();


	int loginId = NET_DVR_Login_V30(ipTmp, device.getPort(), userNameTmp, passwordTmp, &deviceInfo);

	if (loginId == -1)
	{
		errnum = NET_DVR_GetLastError();
		return FALSE;
	}

	device.setDeviceInfo(deviceInfo);
	device.setLoginId(loginId);
	device.setWorkable(true);

	/*��ȡ������Ϣ*/
	NET_DVR_SETUPALARM_PARAM struAlarmParam = { 0 };
	struAlarmParam.dwSize = sizeof(struAlarmParam);
	struAlarmParam.byAlarmInfoType = 0;
	
	LONG alarmHandle = NET_DVR_SetupAlarmChan_V41(loginId, &struAlarmParam);
	device.setAlarmHandle(alarmHandle);

	return TRUE;
}

QTreeWidgetItem * MainWindow::_GetItemOfVideoLabel()
{
	for (QWidget* w : m_videoLabels)
	{
		if (w == m_selectedLabel)
		{
			return m_videoLabels.key(w);
			break;
		}
	}
	return nullptr;
}


LONG MainWindow::_StartPreview(LONG loginId, LONG channelNum, HWND wid)
{
	NET_DVR_PREVIEWINFO previewInfo;
	memset(&previewInfo, 0, sizeof(previewInfo));

	previewInfo.lChannel = channelNum;
	previewInfo.dwStreamType = 0;
	previewInfo.dwLinkMode = 0;
	previewInfo.hPlayWnd = wid;
	previewInfo.bBlocked = 1;

	LONG realHandle = NET_DVR_RealPlay_V40(loginId, &previewInfo, nullptr, nullptr);
	return realHandle;
}
bool MainWindow::_StopPreview(LONG realHandle)
{
	return NET_DVR_StopRealPlay(realHandle);
}


/*************************�麯��*********************************/
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
	QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

	if (mouseEvent->type() == QEvent::MouseButtonDblClick && mouseEvent->button() == Qt::LeftButton)
	{
		QWidget *label = static_cast<QWidget*>(watched);

		if (m_channelMaxed)
		{
			_ShowAllVideoLabel();
		}
		else
		{
			_HideAllVideoLabel();
			label->setVisible(true);
		}
		m_channelMaxed = !m_channelMaxed;
	}
	
	if (mouseEvent->type() == QEvent::MouseButtonPress)
	{
		QWidget *label = static_cast<QWidget*>(watched);
		label->setStyleSheet(m_selectedStyle);

		if (m_selectedLabel != nullptr);
			m_selectedLabel->setStyleSheet(m_normalStyle);
		m_selectedLabel = label;

		if (mouseEvent->button() == Qt::RightButton)
		{
			if (m_videoLabelMenu != nullptr)
			{
				delete m_videoLabelMenu;
				m_videoLabelMenu = nullptr;
			}

			m_videoLabelMenu = new QMenu();
			QWidget *label = static_cast<QWidget*>(watched);
			QAction *action = *m_videoActions.find(label);
			m_videoLabelMenu->addAction(action);

			QTreeWidgetItem *item = _GetItemOfVideoLabel();
			action->setEnabled((item != nullptr));

			m_videoLabelMenu->exec(label->mapToGlobal(mouseEvent->pos()));
		}
	}
	return QObject::eventFilter(watched, event);
}
/************************˽�в�*****************************/
void MainWindow::onShowAddDeviceDialog()
{
	if (m_addDeviceDialog != nullptr)
	{
		delete m_addDeviceDialog;
		m_addDeviceDialog = nullptr;
	}
	m_addDeviceDialog = new AddDeviceDialog(this);
	m_addDeviceDialog->setModal(true);
	m_addDeviceDialog->show();

	connect(m_addDeviceDialog, &AddDeviceDialog::addDevice, this, [this](DeviceInfo device) {
		QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget, QStringList(device.getDeviceName()));
		item->setIcon(0, QIcon(":/Homework/Resources/offline.png"));

		int deviceId = Database::addDevice(device);
		device.setDeviceId(deviceId);
		m_devices.insert(item, device);
	});
}

void MainWindow::onShowContextMenu(const QPoint &pos)
{
	QTreeWidgetItem *item = ui->treeWidget->itemAt(pos);
	if (item != nullptr)
	{
		m_currentItem = item;
		if (item->parent() == nullptr)
		{
			//�豸�ڵ�
			m_deviceMenu->exec(ui->treeWidget->mapToGlobal(pos));
		}
		else
		{
			//ͨ���ڵ�
			m_channelMenu->exec(ui->treeWidget->mapToGlobal(pos));
		}
	}
}

void MainWindow::onLogin()
{
	DeviceInfo &deviceInfo = *m_devices.find(m_currentItem);

	int errnum = 0;

	if (!_DoLogin(deviceInfo, errnum))
	{
		QString errmsg = "��¼ʧ��! ������:" + QString::number(errnum);
		QMessageBox::information(this, QStringLiteral("��ʾ"), errmsg, QMessageBox::Ok);
		return;
	}
	m_currentItem->setIcon(0, QIcon(":/Homework/Resources/online.png"));
	if (deviceInfo.isFirstLogin())
	{
		//��ȡͨ����Ϣ
		int channlCnt = deviceInfo.getDeviceInfo().byChanNum;
		for (int i = 1; i <= channlCnt; ++i)
		{
			ChannelInfo channelInfo;
			channelInfo.setChannelNum(i);
			channelInfo.setChannelName("ͨ��" + QString::number(i));
			channelInfo.setDevice(&deviceInfo);

			//��treeWidget�������Ŀ
			QTreeWidgetItem *item = new QTreeWidgetItem(m_currentItem, QStringList(channelInfo.getChannelName()));
			item->setIcon(0, QIcon(":/Homework/Resources/pause.png"));
			m_currentItem->addChild(item);

			m_channels.insert(item, channelInfo);

			//�������ݿ�
			//Database::addChannel(channelInfo);
		}
		deviceInfo.setFirstLogin(false);
	}
	deviceInfo.setWorkable();

	//test����¼��
	int ret = NET_DVR_PlayBackByName(deviceInfo.getLoginID(), "C:\mpeg4record\2018-09-11\10.18.99.36_01_0_20180911_172856_1.mp4", (HWND)ui->videoLabel1->winId());
	if (ret == -1)
		ui->statusbar->showMessage(NET_DVR_GetErrorMsg());
	else
		ui->statusbar->showMessage("??");
}

void MainWindow::onLogout()
{
	//1. �ж��Ƿ������ڲ��ŵ�ͨ����
	DeviceInfo &deviceInfo = *m_devices.find(m_currentItem);

	for (auto iter = m_channels.cbegin(); iter != m_channels.end(); ++iter)
	{
		if (iter->getRealhandle() != -1 && iter->getDevice() == &deviceInfo)
		{
			QMessageBox::information(this, QStringLiteral("��ʾ"), QStringLiteral("���豸��ͨ������Ԥ����¼��"), QMessageBox::Ok);
			return;
		}
	}
	//2. ��������
	NET_DVR_CloseAlarmChan_V30(deviceInfo.getAlarmHandle());

	//3. ע���豸
	NET_DVR_Logout(deviceInfo.getLoginID());
	deviceInfo.setWorkable(false);

	//4.����itemͼ��
	m_currentItem->setIcon(0, QIcon(":/Homework/Resources/offline.png"));
}

void MainWindow::onDelete()
{
	auto ret = QMessageBox::information(this, QStringLiteral("��ʾ"), QStringLiteral("ȷ��ɾ�����豸��"), QMessageBox::Ok | QMessageBox::Cancel);
	if (ret == QMessageBox::Cancel)
	{
		return;
	}

	//1. �ж��Ƿ�ǰ�豸��ͨ������ʹ��
	auto iter = m_devices.find(m_currentItem);
	DeviceInfo &deviceInfo = *iter;
	for (auto iter = m_channels.cbegin(); iter != m_channels.cend(); ++iter)
	{
		if (iter->getRealhandle() != -1 && iter->getDevice() == &deviceInfo)
		{
			QMessageBox::information(this, QStringLiteral("��ʾ"), QStringLiteral("���豸��ͨ������Ԥ����¼��"), QMessageBox::Ok);
			return;
		}
	}

	//2. ɾ��ͨ��
	for (auto iter = m_channels.begin(); iter != m_channels.end();)
	{
		if (iter->getDevice() == &deviceInfo)
		{
			m_channels.erase(iter++);
		}
		else
		{
			++iter;
		}
	}

	//3. �����ݿ�ɾ��
	Database::delDevice(deviceInfo.getDeviceId());
	
	//4. ���ڴ�ɾ��
	m_devices.erase(iter);

	delete m_currentItem;
	m_currentItem = nullptr;
}

void MainWindow::onShowAttribute()
{
	DeviceInfo &deviceInfo = *m_devices.find(m_currentItem);
	
	if (m_deviceInfoDialog != nullptr)
	{
		delete m_deviceInfoDialog;
		m_deviceInfoDialog = nullptr;
	}
	
	m_deviceInfoDialog = new DeviceInfoDialog(deviceInfo, this);
	m_deviceInfoDialog->setModal(true);
	m_deviceInfoDialog->show();

	connect(m_deviceInfoDialog, &DeviceInfoDialog::modifyDeviceInfo, this, &MainWindow::onModifyDeviceInfo);
}

void MainWindow::onPlay(QTreeWidgetItem *item, int column)
{
	m_currentItem = item;
	auto iter = m_channels.find(m_currentItem);
	if (iter == m_channels.end())
	{
		return;
	}

	ChannelInfo &channelInfo = *iter;

	//δ����
	if (channelInfo.getRealhandle() == -1)
	{
		//[1] �жϵ�ǰѡ�еĴ����Ƿ������ڲ��ŵ�ͨ��
		QTreeWidgetItem *item = _GetItemOfVideoLabel();
		if (item != nullptr)
		{
			ChannelInfo &channelInfo = *m_channels.find(item);

			//ֹͣ����ʧ�ܣ�ֱ�ӷ��أ�
			if (!_StopPreview(channelInfo.getRealhandle()))
			{
				return;
			}
			channelInfo.setRealhandle(-1);
			item->setIcon(0, QIcon(":/Homework/Resources/pause.png"));

			m_videoLabels.remove(item);
		}

		LONG realHandle = -1;
		if ((realHandle = _StartPreview(channelInfo.getDevice()->getLoginID(), channelInfo.getChannelNum(), (HWND)m_selectedLabel->winId())) == -1)
		{
			QString msg = "���Ŵ���! ������:" + QString::number(NET_DVR_GetLastError());
			QMessageBox::information(this, QStringLiteral("��ʾ"), msg, QMessageBox::Ok);
			return;
		}

		channelInfo.setRealhandle(realHandle);
		m_videoLabels.insert(m_currentItem, m_selectedLabel);
		m_currentItem->setIcon(0, QIcon(":/Homework/Resources/play.png"));
	}
	else
	{
		//�ò�������ˢ��label����Ȼֹͣ���ź󣬻���ʾֹͣǰ���һ֡����
		auto iter = m_videoLabels.find(m_currentItem);
		if (iter != m_videoLabels.end())
		{
			(*iter)->hide();
			(*iter)->show();
			m_videoLabels.erase(iter);
		}

		if (!_StopPreview(channelInfo.getRealhandle()))
		{
			QString msg = "ֹͣ���Ŵ���! ������:" + QString::number(NET_DVR_GetLastError());
			QMessageBox::information(this, QStringLiteral("��ʾ"), msg, QMessageBox::Ok);
			return;
		}

		channelInfo.setRealhandle(-1);
		m_currentItem->setIcon(0, QIcon(":/Homework/Resources/pause.png"));
	}
}

void MainWindow::onCapturePicture()
{
	if (m_capturePictureDialog != nullptr)
	{
		delete m_capturePictureDialog;
		m_capturePictureDialog = nullptr;
	}
	m_capturePictureDialog = new CapturePictureDialog();
	m_capturePictureDialog->setModal(true);
	m_capturePictureDialog->show();

	connect(m_capturePictureDialog, &CapturePictureDialog::capture, this, [this](int size, int quality){
		NET_DVR_JPEGPARA jpgeParama;
		jpgeParama.wPicSize = size;
		jpgeParama.wPicQuality = quality;

		//NET_DVR_CaptureJPEGPicture
		ChannelInfo &channelInfo = *m_channels.find(m_currentItem);
		int loginId = channelInfo.getDevice()->getLoginID();
		int channelNum = channelInfo.getChannelNum();
		QString fileName = Utils::getSettingValue("ImageStorgeLocation") + "/1.jpg";
		QByteArray array = fileName.toUtf8();

		bool ret = NET_DVR_CaptureJPEGPicture(loginId, channelNum, &jpgeParama, array.data());
		QString msg = ret ? "�ɹ�" : "ʧ��";
		QMessageBox::information(this, QStringLiteral("��ʾ"), msg, QMessageBox::Ok);
	});
}

void MainWindow::onShowChannelAttribute()
{
	ChannelInfo &channlnfo = *m_channels.find(m_currentItem);

	if (m_channelInfoDialog != nullptr)
	{
		delete m_channelInfoDialog;
		m_channelInfoDialog = nullptr;
	}

	m_channelInfoDialog = new ChannelInfoDialog(channlnfo, this);
	m_channelInfoDialog->setModal(true);
	m_channelInfoDialog->show();

	connect(m_channelInfoDialog, &ChannelInfoDialog::modifyChannelInfo, this, [this](ChannelInfo channleInfo) {
		ChannelInfo &channel = *m_channels.find(m_currentItem);
		channel.setChannelName(channleInfo.getChannelName());

		//�����ڵ���
		m_currentItem->setText(0, channel.getChannelName());
	});
}


void MainWindow::onModifyDeviceInfo(DeviceInfo deviceInfo)
{
	DeviceInfo &device = *m_devices.find(m_currentItem);
	device.setDeviceName(deviceInfo.getDeviceName());
	device.setIP(deviceInfo.getIP());
	device.setPort(deviceInfo.getPort());
	device.setUserName(deviceInfo.getUserName());
	device.setPasswd(deviceInfo.getPasswd());

	//�����ڵ���
	m_currentItem->setText(0, device.getDeviceName());
	
	//��Ϣ�����󣬲��������´ε�¼ʱ�Ż�ʹ��������
	
}


void MainWindow::onRecord()
{
	QTreeWidgetItem *item = _GetItemOfVideoLabel(); 
	ChannelInfo &channelInfo = *m_channels.find(item);

	QAction *action = static_cast<QAction*>(sender());
	if (action->text() == "��ʼ¼��")
	{
		action->setText("ֹͣ¼��");
		QString fileName = Utils::getSettingValue("VideoStorgeLocation") + Utils::getVideoFileName(channelInfo);
		/*�ڱ��ػ��������ʹ��toUtf8��ᵼ����������*/
		QByteArray array = fileName.toLocal8Bit();
		if (NET_DVR_SaveRealData(channelInfo.getRealhandle(), array.data()))
		{
			ui->statusbar->showMessage("����¼��");
		}
		else
		{
			QString errmsg = "¼��ʧ��! ������룺" + QString::number(NET_DVR_GetLastError());
			ui->statusbar->showMessage(errmsg);
		}
	}
	else
	{
		action->setText("��ʼ¼��");
		if (NET_DVR_StopSaveRealData(channelInfo.getRealhandle()))
		{
			ui->statusbar->showMessage("¼���ѱ���!!", 5000);
		}
		else
		{
			QString errmsg = "����¼��ʧ��! ������룺" + QString::number(NET_DVR_GetLastError());
			ui->statusbar->showMessage(errmsg);
		}
	}
	/*
	
	*/
}


bool _AlarmInfoCallBack(LONG lCommand, NET_DVR_ALARMER *pAlarmer, char *pAlarmInfo, DWORD dwBufLen, void *pUser)
{
	if (lCommand != COMM_ALARM)
		return false;

	NET_DVR_ALARMINFO alarmInfo;
	memset(&alarmInfo, 0, sizeof(NET_DVR_ALARMINFO));
	memcpy(&alarmInfo, pAlarmInfo, sizeof(NET_DVR_ALARMINFO));

	switch (alarmInfo.dwAlarmType)
	{
		case 0://signal alarm
			g_mainWindow->addLog("signal alarm");
			break;
		case 1://hard disk full alarm
			g_mainWindow->addLog("hard disk full alarm");
			break;
		case 2://video loss alarm
			g_mainWindow->addLog("video loss alarm");
			break;
		case 3://motion detection alarm
			g_mainWindow->addLog("motion detection alarm");
			break;
		case 4://hard disk unformatted alarm
			g_mainWindow->addLog("hard disk unformatted alarm");
			break;
		case 5://hard disk alarm
			g_mainWindow->addLog("hard disk alarm");
			break;
		case 6://mask alarm
			g_mainWindow->addLog("mask alarm");
			break;
		case 7:	//type unmatched
			g_mainWindow->addLog("type unmatched");
			break;
		case 8: //"Illegal access"

			g_mainWindow->addLog("Illegal access");
			break;
		case 9: //Video signal abnormalities
			g_mainWindow->addLog("Video signal abnormalities");
			break;
		case 10: //Video abnorma
			g_mainWindow->addLog("Video abnorma");
			break;
		case 11: //"Intelligent Scene Change"
			g_mainWindow->addLog("Intelligent Scene Change");
			break;
		default: // "Unknown alarm type:"
			g_mainWindow->addLog("Unknown alarm type");
			break;
	}

	return true;
}