#pragma once
#include <QWidget>

namespace Ui {
	class Form;
}

class PlayBackWidget :
	public QWidget
{
public:
	explicit PlayBackWidget(QWidget *parent = nullptr);
	~PlayBackWidget();

protected:
	void paintEvent(QPaintEvent *event);

private:
	Ui::Form *ui;
};

