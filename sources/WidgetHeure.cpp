#include "WidgetHeure.h"

WidgetHeure::WidgetHeure(QWidget *parent) :
    QLCDNumber(parent),
    m_timer(this)
{
    setDigitCount(5);
    refresh();

    connect(&m_timer, &QTimer::timeout, this, &WidgetHeure::refresh);
    m_timer.start(10'000);
}
void WidgetHeure::refresh()
{
    const QTime time = QTime::currentTime();
    const QString text = time.toString("hh:mm");
    display(text);
}
