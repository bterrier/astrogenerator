#include "timerwidget.h"

#include <QHBoxLayout>

TimerWidget::TimerWidget(QWidget *parent) :
    QWidget{parent},
    m_lcd(new QLCDNumber(this)),
    m_timer(this)
{
	m_lcd->display(0);
	m_lcd->setSegmentStyle(QLCDNumber::Filled);
	QPalette palette = m_lcd->palette();
	palette.setColor(QPalette::Light, QColor(255, 255, 255));
	m_lcd->setPalette(palette);

	QHBoxLayout *layout = new QHBoxLayout(this);
	setLayout(layout);
	layout->addWidget(m_lcd);

	m_timer.setSingleShot(false);
	m_timer.setInterval(100);
	connect(&m_timer, &QTimer::timeout, this, &TimerWidget::refresh);
}

void TimerWidget::start(int seconds)
{
	m_interval = seconds * 1000;
	m_elapsed.restart();
	m_timer.start();
}

static QString secToMinSec(qint64 seconds)
{
	const auto mins = seconds / 60;
	const auto secs = seconds % 60;

	return QStringLiteral("%1:%2")
	    .arg(mins, 2, 10, QChar('0'))
	    .arg(secs, 2, 10, QChar('0'));
}

void TimerWidget::refresh()
{
	const qint64 elapsed = m_elapsed.elapsed();
	const qint64 left = m_interval - elapsed;

	if (left <= 0) {
		m_lcd->display(secToMinSec(0));
		m_timer.stop();
		emit timeout();
	} else {
		m_lcd->display(secToMinSec(left / 1000));
	}
}
