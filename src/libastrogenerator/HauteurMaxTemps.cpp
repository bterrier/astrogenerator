#include "HauteurMaxTemps.h"

#include <QMessageBox>

HauteurMaxTemps::HauteurMaxTemps() {}
HauteurMaxTemps::HauteurMaxTemps(QDateTime temps, double hauteurMax)
{
    if (temps.isValid() && !temps.isNull())
        m_temps = temps;
    else {
        QMessageBox::critical(nullptr, QObject::tr("Date"), QObject::tr("Erreur de date dans HauteurMaxTemps. Contacter le developpeur."));
        m_temps = QDateTime();
    }
    if (hauteurMax < 90 && hauteurMax > -90)
        m_hauteurMax = hauteurMax;
    else {
        QMessageBox::critical(nullptr, QObject::tr("Hauteur maximum"), QObject::tr("La hauteur maximum doit être comprise entre -90° et 90°"));
        m_hauteurMax = 0;
    }
}
double HauteurMaxTemps::hauteurMax() const
{
    return m_hauteurMax;
}
QDateTime HauteurMaxTemps::temps() const
{
    return m_temps;
}
void HauteurMaxTemps::setHauteurMax(double hauteur)
{
    if (hauteur < 90 && hauteur > -90)
        m_hauteurMax = hauteur;
    else
        QMessageBox::critical(nullptr, QObject::tr("Hauteur maximum"), QObject::tr("La hauteur maximum doit être comprise entre -90° et 90°"));
}
void HauteurMaxTemps::setDateTime(QDateTime date)
{
    if (date.isValid())
        m_temps = date;
    else
        QMessageBox::critical(nullptr, QObject::tr("Date"), QObject::tr("Erreur de date dans HauteurMaxTemps. Contacter le developpeur."));
}
