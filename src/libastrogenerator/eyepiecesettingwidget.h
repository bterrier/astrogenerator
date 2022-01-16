#ifndef EYEPIECESETTINGWIDGET_H
#define EYEPIECESETTINGWIDGET_H

#include <QWidget>

namespace Ui
{
class EyepieceSettingWidget;
}

class EyepieceSettingWidget : public QWidget
{
	Q_OBJECT

public:
	explicit EyepieceSettingWidget(QWidget *parent = nullptr);
	~EyepieceSettingWidget();

	QString settingString() const;
	void add(int focalLength);
	void clear();

private:
	Ui::EyepieceSettingWidget *ui;
};

#endif // EYEPIECESETTINGWIDGET_H
