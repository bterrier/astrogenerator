#include "eyepiecesettingwidget.h"
#include "ui_eyepiecesettingwidget.h"

class EyepieceItem : public QListWidgetItem
{
public:
	EyepieceItem(int value, QListWidget *parent = nullptr) :
	    QListWidgetItem(QStringLiteral("%1 mm").arg(value), parent)
	{
		setData(Qt::UserRole, value);
	}

	bool operator<(const QListWidgetItem &other) const override
	{
		return data(Qt::UserRole).toInt() < other.data(Qt::UserRole).toInt();
	}
};

EyepieceSettingWidget::EyepieceSettingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EyepieceSettingWidget)
{
	ui->setupUi(this);

	connect(ui->pbAdd, &QPushButton::clicked, this, [this]() {
		int value = ui->spinBox->value();
		add(value);
	});

	connect(ui->pbRemove, &QPushButton::clicked, this, [this]() {
		const auto items = ui->listWidget->selectedItems();
		for (const auto &item : items) {
			delete item;
		}
	});
}

EyepieceSettingWidget::~EyepieceSettingWidget()
{
	delete ui;
}

QString EyepieceSettingWidget::settingString() const
{
	QStringList list;
	for (int i = 0; i < ui->listWidget->count(); ++i) {
		int current = ui->listWidget->item(i)->data(Qt::UserRole).toInt();
		list.append(QString::number(current));
	}

	return list.join('|');
}

void EyepieceSettingWidget::add(int focalLength)
{
	if (focalLength <= 0) {
		return;
	}

	for (int i = 0; i < ui->listWidget->count(); ++i) {
		int current = ui->listWidget->item(i)->data(Qt::UserRole).toInt();
		if (current == focalLength)
			return;
	}

	new EyepieceItem(focalLength, ui->listWidget);
	ui->listWidget->sortItems();
}

void EyepieceSettingWidget::clear()
{
	ui->listWidget->clear();
}
