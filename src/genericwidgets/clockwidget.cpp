#include "clockwidget.h"

#include <QTime>

ClockWidget::ClockWidget(QWidget *parent) :
    QLCDNumber{parent},
    m_timer(new QTimer(this))
{
	m_timer->setSingleShot(false);
	connect(m_timer, &QTimer::timeout, this, &ClockWidget::refresh);
	m_timer->start(1'000);
	refresh();
}

void ClockWidget::refresh()
{
	const QTime time = QTime::currentTime();
	const QString text = time.toString("hh:mm");
	display(text);
}
