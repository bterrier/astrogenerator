#ifndef OBJETPLANETE_H
#define OBJETPLANETE_H

#include <QDateTime>
#include <QString>

#include "Objet.h"
#include "ObjetObs.h"

class ObjetPlaneteObs : public ObjetObs
{
    Q_OBJECT

public:
    ObjetPlaneteObs(QString nomPlanete, QDateTime debut, QDateTime fin);
    QString nomComplet(bool abrege = false) const;

    double illumination() const;
    double distance() const;

protected:
    double m_illumination;
    double m_distance;
};

#endif // OBJETPLANETE_H
