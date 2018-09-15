#pragma once
#include <QWidget>

namespace Ui 
{
	class MainPlayBackWidget;
}

class MainPlayBackWidget :
	public QWidget
{
public:
	explicit MainPlayBackWidget(QWidget *parent = nullptr);
	~MainPlayBackWidget();

private:
	Ui::MainPlayBackWidget *ui;
};

