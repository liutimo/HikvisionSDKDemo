#include "MainPlayBackWidget.h"
#include "ui_MainPlayBackWidget.h"
#include "PlayBackWidget.h"
MainPlayBackWidget::MainPlayBackWidget(QWidget *parent) : QWidget(parent),
ui(new Ui::MainPlayBackWidget)
{
	ui->setupUi(this);

	PlayBackWidget *p1 = new PlayBackWidget();
	PlayBackWidget *p2 = new PlayBackWidget();
	PlayBackWidget *p3 = new PlayBackWidget();
	PlayBackWidget *p4 = new PlayBackWidget();

	ui->gridLayout->addWidget(p1, 0, 0, 1, 1);
	ui->gridLayout->addWidget(p2, 0, 1, 1, 1);
	ui->gridLayout->addWidget(p3, 1, 0, 1, 1);
	ui->gridLayout->addWidget(p4, 1, 1, 1, 1);

	ui->tableWidget->setRowCount(4);

}


MainPlayBackWidget::~MainPlayBackWidget()
{
	delete ui;
}
