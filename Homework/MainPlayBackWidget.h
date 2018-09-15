#pragma once
#include <QWidget>

namespace Ui 
{
	class MainPlayBackWidget;
}

class MainPlayBackWidget :
	public QWidget
{
	Q_OBJECT
public:
	explicit MainPlayBackWidget(QWidget *parent = nullptr);
	~MainPlayBackWidget();

private slots:
	void onPlay();

private:
	typedef struct
	{
		QString fileName;
		int		fileSize;
	}LocalFile;


	void _FindLocalFile();			//该函数之查找设置中存储视频文件的文件夹
	void _AddRow(const LocalFile&);					//添加一行

	Ui::MainPlayBackWidget *ui;

	
};

