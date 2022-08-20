#include "HauteurMaxTemps.h"

#include <QDebug>

HauteurMaxTemps::HauteurMaxTemps(const QDateTime &time, double hauteurMax) :
    m_temps(time),
    m_hauteurMax(hauteurMax)
{
	if (!time.isValid()) {
		qCritical() << "Erreur de date dans HauteurMaxTemps. Contacter le developpeur.";
		m_temps = QDateTime();
	}

	if (hauteurMax > 90 || hauteurMax < -90) {
		qCritical() << "La hauteur maximum doit être comprise entre -90° et 90°";
		m_hauteurMax = 0;
	}
}

void HauteurMaxTemps::setHauteurMax(double hauteur)
{
	if (hauteur < 90 && hauteur > -90)
		m_hauteurMax = hauteur;
	else
		qCritical() << "La hauteur maximum doit être comprise entre -90° et 90°";
}

void HauteurMaxTemps::setDateTime(const QDateTime &date)
{
	if (date.isValid())
		m_temps = date;
	else
		qCritical() << "Erreur de date dans HauteurMaxTemps. Contacter le developpeur.";
}
