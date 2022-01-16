#ifndef TIMERWIDGET_H
#define TIMERWIDGET_H

#include <QElapsedTimer>
#include <QTimer>

#include <QLCDNumber>
#include <QWidget>

class TimerWidget : public QWidget
{
	Q_OBJECT
public:
	explicit TimerWidget(QWidget *parent = nullptr);

signals:
	void timeout();

public slots:
	void start(int seconds = 10);

private slots:
	void refresh();

private:
	QLCDNumber *m_lcd;
	QElapsedTimer m_elapsed;
	QTimer m_timer;
	qint64 m_interval = 0;
};

#endif // TIMERWIDGET_H
