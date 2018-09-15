#include "PlayBackWidget.h"
#include "ui_PlayBackWidget.h"
#include <QPainter>

PlayBackWidget::PlayBackWidget(QWidget *parent) :
QWidget(parent), ui(new Ui::Form)
{
	ui->setupUi(this);
}


PlayBackWidget::~PlayBackWidget()
{
	delete ui;
}

void PlayBackWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setPen(Qt::red);

	painter.drawRect(rect());
}