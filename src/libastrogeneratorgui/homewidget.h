#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QWidget>

namespace Ui
{
class HomeWidget;
}

class HomeWidget : public QWidget
{
	Q_OBJECT

public:
	explicit HomeWidget(QWidget *parent = nullptr);
	~HomeWidget() override;

signals:
	void generateClicked();
	void createClicked();
	void openClicked();
	void settingsClicked();

private:
	Ui::HomeWidget *ui;
};

#endif // HOMEWIDGET_H
