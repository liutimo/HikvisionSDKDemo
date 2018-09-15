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
	//最后一列自动填充
	ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
	//ui->tableWidget->setVerticalHeader()->hide();
	ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//奇数偶数行不同背景色
	ui->tableWidget->setAlternatingRowColors(true);

	//选中整行,每次只允许选中一行
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