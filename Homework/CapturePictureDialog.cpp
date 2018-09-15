#include "CapturePictureDialog.h"
#include "ui_CapturePictureDialog.h"

CapturePictureDialog::CapturePictureDialog(QWidget *parent) : QDialog(parent),
ui(new Ui::CapturePicture)
{
	ui->setupUi(this);
	connect(ui->btnClose, &QPushButton::clicked, this, &QDialog::close);
	connect(ui->btnCapture, &QPushButton::clicked, this, [this]() {
		int size = ui->sizeComboBox->currentIndex();
		int quality = ui->qualityComboBox->currentIndex();
		emit capture(size, quality);
		close();
	});
}


CapturePictureDialog::~CapturePictureDialog()
{
	delete ui;
}
