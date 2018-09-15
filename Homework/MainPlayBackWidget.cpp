#include "MainPlayBackWidget.h"
#include "ui_MainPlayBackWidget.h"
#include "PlayBackWidget.h"

#include <HCNetSDK.h>
#include <plaympeg4.h>
#include "Utils.h"
#include <QFileInfo>
#include <QDir>

MainPlayBackWidget::MainPlayBackWidget(QWidget *parent) : QWidget(parent),
ui(new Ui::MainPlayBackWidget)
{
	ui->setupUi(this);
	//���һ���Զ����
	ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
	//ui->tableWidget->setVerticalHeader()->hide();
	ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//����ż���в�ͬ����ɫ
	ui->tableWidget->setAlternatingRowColors(true);

	//ѡ������,ÿ��ֻ����ѡ��һ��
	ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	_FindLocalFile();

	connect(ui->playBtn, &QPushButton::clicked, this, &MainPlayBackWidget::onPlay);
}


MainPlayBackWidget::~MainPlayBackWidget()
{
	delete ui;
}


void MainPlayBackWidget::onPlay()
{
	int row = ui->tableWidget->currentRow();
	QTableWidgetItem *item = ui->tableWidget->item(row, 0);
	if (item == nullptr)
	{
		return;
	}

	QString fileName = Utils::getSettingValue("VideoStorgeLocation") + item->text();


}

void MainPlayBackWidget::_FindLocalFile()
{
	QString filePath = Utils::getSettingValue("VideoStorgeLocation");

	QDir dir(filePath);
	dir.setNameFilters(QStringList("*.mp4"));
	
	QFileInfoList list = dir.entryInfoList();
	for (const QFileInfo &fileInfo : list)
	{
		LocalFile file;
		file.fileName = fileInfo.fileName();
		file.fileSize = fileInfo.size();

		_AddRow(file);
	}



}

void MainPlayBackWidget::_AddRow(const LocalFile &file)
{
	QTableWidgetItem *item = new QTableWidgetItem(file.fileName);
	QTableWidgetItem *sizeItem = new QTableWidgetItem(QString::number(file.fileSize));
	
	int row = ui->tableWidget->rowCount();
	ui->tableWidget->setRowCount(row + 1);
	ui->tableWidget->setItem(row, 0, item);
	ui->tableWidget->setItem(row, 1, sizeItem);

}