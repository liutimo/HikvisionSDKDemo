#include "SettingDialog.h"
#include "ui_SettingDialog.h"
#include <QFileDialog>
#include <QSettings>
#include "Utils.h"
SettingDialog::SettingDialog(QWidget *parent) : QDialog(parent),
ui(new Ui::SettingDialog)
{
	ui->setupUi(this);
	ui->lineEdit1->setText(Utils::getSettingValue("ImageStorgeLocation"));
	ui->lineEdit2->setText(Utils::getSettingValue("VideoStorgeLocation"));

	connect(ui->openBtn1, &QToolButton::clicked, this, [this](){
		QString directory = QFileDialog::getExistingDirectory(this);
		if (!directory.isEmpty())
		{
			ui->lineEdit1->setText(directory.remove("file:///"));
			ui->lineEdit1->setCursorPosition(0);
			Utils::setSettingValue("ImageStorgeLocation", ui->lineEdit1->text());
		}
	});

	connect(ui->openBtn2, &QToolButton::clicked, this, [this](){
		QString directory = QFileDialog::getExistingDirectory(this);
		if (!directory.isEmpty())
		{
			ui->lineEdit2->setText(directory.remove("file:///"));
			ui->lineEdit2->setCursorPosition(1);
			Utils::setSettingValue("VideoStorgeLocation", ui->lineEdit2->text());
		}
	});
}


SettingDialog::~SettingDialog()
{
	delete ui;
}
