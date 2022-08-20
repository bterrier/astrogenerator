#include "ObjetObs.h"

ObjetObs::ObjetObs(QDateTime debut, QDateTime fin) :
    Objet()
{
	if (debut.isValid() && !debut.isNull() && fin.isValid() && !fin.isNull()) {
		m_debutObs = debut;
		m_finObs = fin;
	}
}
QDateTime ObjetObs::getDebut() const
{
	return m_debutObs;
}
QDateTime ObjetObs::getFin() const
{
	return m_finObs;
}
void ObjetObs::setDebut(QDateTime const &debut)
{
	if (debut.isValid() && !debut.isNull())
		m_debutObs = debut;
	else
		qCritical() << "La date donnée est invalide dans ObjetObs.";
}
void ObjetObs::setFin(QDateTime const &fin)
{
	if (fin.isValid() && !fin.isNull())
		m_finObs = fin;
	else
		qCritical() << "La date donnée est invalide dans ObjetObs.";
}
bool ObjetObs::estEgal(const ObjetObs &objet) const
{
	if (objet.getDebut() == m_debutObs
	    && objet.getFin() == m_finObs
	    && objet.ref() == m_ref) {
		return true;
	} else
		return false;
}
bool operator==(ObjetObs const &a, ObjetObs const &b)
{
	return a.estEgal(b);
}
bool operator!=(ObjetObs const &a, ObjetObs const &b)
{
	if (a.estEgal(b))
		return false;
	else
		return true;
}
