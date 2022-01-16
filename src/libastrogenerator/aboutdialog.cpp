#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include <QLocale>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
	ui->setupUi(this);
	ui->labelVersion->setText(VERSION_STRING);
	ui->labelLocale->setText(QLocale::system().name());
}

AboutDialog::~AboutDialog()
{
	delete ui;
}
