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


	void _FindLocalFile();			//�ú���֮���������д洢��Ƶ�ļ����ļ���
	void _AddRow(const LocalFile&);					//���һ��

	Ui::MainPlayBackWidget *ui;

	
};

