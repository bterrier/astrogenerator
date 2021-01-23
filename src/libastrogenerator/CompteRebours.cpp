#include "CompteRebours.h"

#include <cmath>

#include <QTimer>
#include <QVBoxLayout>

CompteRebours::CompteRebours()
{
    m_lcd = new QLCDNumber;
    m_lcd->display(0);
    m_lcd->setSegmentStyle(QLCDNumber::Filled);
    QPalette palette = m_lcd->palette();
    palette.setColor(QPalette::Light, QColor(255, 255, 255));
    m_lcd->setPalette(palette);
    timer = new QTimer;

    connect(timer, &QTimer::timeout, this, &CompteRebours::enlever);
    connect(this, &CompteRebours::fin, timer, &QTimer::stop);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(m_lcd);
    setLayout(layout);
}
void CompteRebours::enlever()
{
    secondeRestant--;
    m_lcd->display(secToMinSec(secondeRestant));
    if (secondeRestant == 0)
        emit fin();
}
void CompteRebours::commencer(int secondes)
{
    m_lcd->display(secToMinSec(secondes));
    secondeRestant = secondes;
    timer->start(1000); // On le repète toutes les secondes
}
QString CompteRebours::secToMinSec(int sec) const
{
    int minute = int(floor(sec / 60));
    int secNew = sec - minute * 60;

    QString resultat;

    if (minute < 10)
        resultat = "0" + QString::number(minute);
    else
        resultat = QString::number(minute);

    resultat += ":";

    if (secNew < 10)
        resultat += "0" + QString::number(secNew);
    else
        resultat += QString::number(secNew);

    return resultat;
}
