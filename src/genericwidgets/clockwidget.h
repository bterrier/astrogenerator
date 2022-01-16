#ifndef CLOCKWIDGET_H
#define CLOCKWIDGET_H

#include <QLCDNumber>
#include <QTimer>

class ClockWidget : public QLCDNumber
{
	Q_OBJECT
public:
	explicit ClockWidget(QWidget *parent = nullptr);

public slots:
	void refresh();

signals:
private:
	QTimer *m_timer;
};

#endif // CLOCKWIDGET_H
