#pragma once

#include <QMainWindow>
#include <QMap>
#include <HCNetSDK.h>
#include "DeviceInfo.h"
namespace Ui
{
	class MainWindow;
}
QT_BEGIN_NAMESPACE
class AddDeviceDialog;
class QMenu;
class QWidget;
class QTreeWidgetItem;
class DeviceInfo;
class ChannelInfo;
class CapturePictureDialog;
class DeviceInfoDialog;
class ChannelInfoDialog;
QT_END_NAMESPACE

class MainWindow :
	public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();
	void addLog(const char *msg);

private slots:
	void onShowAddDeviceDialog();					//设备添加窗口
	void onShowContextMenu(const QPoint &pos);		//treeWidget右键菜单
	void onLogin();									//登录操作
	void onLogout();								//注销操作
	void onDelete();								//移除设备
	void onShowAttribute();							//设备属性窗口
	void onCapturePicture();						//抓捕图片窗口
	void onPlay(QTreeWidgetItem *item, int column);	//播放item关联的通道
	void onShowChannelAttribute();					//通道属性窗口
	void onModifyDeviceInfo(DeviceInfo deviceInfo);	//修改设备信息
	void onRecord();								//开始录像



protected:
	bool eventFilter(QObject *watched, QEvent *event);

private:
	
	void _InitMenu();
	void _HideAllVideoLabel();
	void _ShowAllVideoLabel();
	void _InitVideoLabel();
	void _InitTreeWidget();
	bool _DoLogin(DeviceInfo &device, int &errnum);
	bool _DoLogin(DeviceInfo &device);			//该版本不关心错误值
	
	
	QTreeWidgetItem *_GetItemOfVideoLabel();	//获取与当前选中videoLabel对应的QTreeWidgetItem
	
	LONG _StartPreview(LONG loginId, LONG channelNum, HWND wid);
	bool _StopPreview(LONG realHandle);

	Ui::MainWindow *ui;
	AddDeviceDialog *m_addDeviceDialog;
	CapturePictureDialog	*m_capturePictureDialog;
	DeviceInfoDialog		*m_deviceInfoDialog;
	ChannelInfoDialog		*m_channelInfoDialog;
	QMenu		   *m_channelMenu;			//通道菜单
	QMenu		   *m_deviceMenu;			//设备菜单
	QMenu		   *m_videoLabelMenu;		//播放窗口菜单

	QTreeWidgetItem *m_currentItem;			//记录菜单显示时对应的item

	bool	m_channelMaxed;					//是否有一个通道处于最大化播放
	QMap<QTreeWidgetItem*, DeviceInfo> m_devices;
	QMap<QTreeWidgetItem*, ChannelInfo> m_channels;
	QMap<QTreeWidgetItem*, QWidget*> m_videoLabels;   //当前正在播放的窗口

	QMap<QWidget *, QAction*> m_videoActions;

	//QSS
	QString m_selectedStyle = "image:url(:/Homework/Resources/logo.png);border:1px solid red; ";
	QString m_normalStyle = "image:url(:/Homework/Resources/logo.png);border:1px solid gray;";

	QWidget *m_selectedLabel;
};


bool _AlarmInfoCallBack(LONG lCommand, NET_DVR_ALARMER *pAlarmer,
	char *pAlarmInfo, DWORD dwBufLen, void *pUser);					//报警信息回调函数