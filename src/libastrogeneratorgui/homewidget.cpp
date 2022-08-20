#include "homewidget.h"
#include "ui_homewidget.h"

HomeWidget::HomeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeWidget)
{
	ui->setupUi(this);

	connect(ui->pbCreate, &QPushButton::clicked, this, &HomeWidget::createClicked);
	connect(ui->pbGenerate, &QPushButton::clicked, this, &HomeWidget::generateClicked);
	connect(ui->pbOpen, &QPushButton::clicked, this, &HomeWidget::openClicked);
	connect(ui->pbSettings, &QPushButton::clicked, this, &HomeWidget::settingsClicked);
}

HomeWidget::~HomeWidget()
{
	delete ui;
}
