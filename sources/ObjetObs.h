#ifndef OBJETOBS_H
#define OBJETOBS_H

#include <QDateTime>

#include "Objet.h"

// ATTENTION CLASSE ABSTRAITE !!!!

class ObjetObs : public Objet
{
    Q_OBJECT

public:
    ObjetObs(QDateTime debut, QDateTime fin);

    QDateTime getDebut() const;
    QDateTime getFin() const;

    void setFin(QDateTime const& fin);
    void setDebut(QDateTime const& debut);

    bool estEgal(ObjetObs const& objet) const;

protected:
    QDateTime m_debutObs;
    QDateTime m_finObs;
};
bool operator==(ObjetObs const& a, ObjetObs const& b);
bool operator!=(ObjetObs const& a, ObjetObs const& b);

#endif // OBJETOBS_H
