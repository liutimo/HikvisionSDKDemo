#pragma once
#include <QWidget>
namespace Ui 
{
	class RemoteFileWidget;
}

QT_BEGIN_NAMESPACE
class MainWindow;
class QTimer;
QT_END_NAMESPACE

class RemoteFileWidget :
	public QWidget
{
	Q_OBJECT
public:
	typedef enum
	{
		PLAYBYTIME = 1,
		PLAYBYNAME
	}PlayBackRemoteFileType;

	explicit RemoteFileWidget(PlayBackRemoteFileType type = PLAYBYNAME ,QWidget *parent = nullptr);
	~RemoteFileWidget();
	void setMainWindow(MainWindow *main);

private slots:
	void onFindFileClicked();
	void onPlayback();
	void onSlowPlay();
	void onVoiceChange(int value);
	void onFastPlay();

private:
	void _Init(); 
	int _GetCurrentChannelNumer();
	void _AddRow(const QString &fileName, int fileSize);
	void _Playback(const QString &fileName);
	void _StopPlayBack();
	bool _SetPlayBackControlCode(unsigned int controlCode);
	bool _SetPlayBackControlCode(unsigned int controlCode, int &value);
	void _FindFileByName(int channelNumber);

	void _PlayByTime();
	

	Ui::RemoteFileWidget *ui;
	MainWindow *m_mainWindow;		//哎，代码已经乱成一锅粥

	int m_userId;
	int m_playHandle;		//暂时只支持一个视频文件的播放
	bool m_isPlaying;		//记录视频播放状态
	QTimer *m_timer;		//计时器记录播放进度

	PlayBackRemoteFileType m_playType;
};

