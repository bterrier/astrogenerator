#ifndef CARTECIEL_H
#define CARTECIEL_H

#include <QGraphicsScene>

#include "ObjetObs.h"
#include "Soiree.h"

class Carteciel : public QGraphicsScene
{
	Q_OBJECT

public:
	explicit Carteciel(Soiree *soiree);
	static double hmsToDegree(QString hms);
	static double dmsToDegree(QString dms);

	void dessinerCarte();
	QColor planeteToColor(QString planeteRef);

	// Accesseurs
	void setLatitude(double latitude);
	void setLongitude(double longitude);
	void setTemps(QDateTime temps);
	void setPlanning(QVector<ObjetObs *> planning);

public slots:
	void setCouleurFond();
	void setCouleurLegende();
	void setCouleurObjet();
	void setCouleurEtoile();
	void setCouleurConstellation();

signals:
	void majCarte();

private:
	Q_DISABLE_COPY(Carteciel)

	QSettings *m_user;
	QColor m_couleurFond;
	QColor m_couleurLegende;
	QColor m_couleurObjet;
	QColor m_couleurEtoile;
	QColor m_couleurConstellation;
	QVector<ObjetObs *> m_planning;
	QDateTime m_temps;
	double m_latitude;
	double m_longitude;
};

#endif // CARTECIEL_H
