#pragma once
#include <QDialog>

namespace Ui 
{
	class SettingDialog;
}


class SettingDialog :
	public QDialog
{
public:
	explicit SettingDialog(QWidget *parent = nullptr);
	~SettingDialog();

private:
	Ui::SettingDialog *ui;
};

