#pragma once
#include  <QDialog>
namespace Ui {
	class CapturePicture;
}


class CapturePictureDialog :
	public QDialog
{
	Q_OBJECT
public:
	explicit CapturePictureDialog(QWidget *parent = nullptr);
	~CapturePictureDialog();

signals:
	void capture(int size, int quality);

private:
	Ui::CapturePicture *ui;
};

