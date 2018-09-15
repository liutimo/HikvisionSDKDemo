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
	void onShowAddDeviceDialog();					//�豸��Ӵ���
	void onShowContextMenu(const QPoint &pos);		//treeWidget�Ҽ��˵�
	void onLogin();									//��¼����
	void onLogout();								//ע������
	void onDelete();								//�Ƴ��豸
	void onShowAttribute();							//�豸���Դ���
	void onCapturePicture();						//ץ��ͼƬ����
	void onPlay(QTreeWidgetItem *item, int column);	//����item������ͨ��
	void onShowChannelAttribute();					//ͨ�����Դ���
	void onModifyDeviceInfo(DeviceInfo deviceInfo);	//�޸��豸��Ϣ
	void onRecord();								//��ʼ¼��



protected:
	bool eventFilter(QObject *watched, QEvent *event);

private:
	
	void _InitMenu();
	void _HideAllVideoLabel();
	void _ShowAllVideoLabel();
	void _InitVideoLabel();
	void _InitTreeWidget();
	bool _DoLogin(DeviceInfo &device, int &errnum);
	bool _DoLogin(DeviceInfo &device);			//�ð汾�����Ĵ���ֵ
	
	
	QTreeWidgetItem *_GetItemOfVideoLabel();	//��ȡ�뵱ǰѡ��videoLabel��Ӧ��QTreeWidgetItem
	
	LONG _StartPreview(LONG loginId, LONG channelNum, HWND wid);
	bool _StopPreview(LONG realHandle);

	Ui::MainWindow *ui;
	AddDeviceDialog *m_addDeviceDialog;
	CapturePictureDialog	*m_capturePictureDialog;
	DeviceInfoDialog		*m_deviceInfoDialog;
	ChannelInfoDialog		*m_channelInfoDialog;
	QMenu		   *m_channelMenu;			//ͨ���˵�
	QMenu		   *m_deviceMenu;			//�豸�˵�
	QMenu		   *m_videoLabelMenu;		//���Ŵ��ڲ˵�

	QTreeWidgetItem *m_currentItem;			//��¼�˵���ʾʱ��Ӧ��item

	bool	m_channelMaxed;					//�Ƿ���һ��ͨ��������󻯲���
	QMap<QTreeWidgetItem*, DeviceInfo> m_devices;
	QMap<QTreeWidgetItem*, ChannelInfo> m_channels;
	QMap<QTreeWidgetItem*, QWidget*> m_videoLabels;   //��ǰ���ڲ��ŵĴ���

	QMap<QWidget *, QAction*> m_videoActions;

	//QSS
	QString m_selectedStyle = "image:url(:/Homework/Resources/logo.png);border:1px solid red; ";
	QString m_normalStyle = "image:url(:/Homework/Resources/logo.png);border:1px solid gray;";

	QWidget *m_selectedLabel;
};


bool _AlarmInfoCallBack(LONG lCommand, NET_DVR_ALARMER *pAlarmer,
	char *pAlarmInfo, DWORD dwBufLen, void *pUser);					//������Ϣ�ص�����