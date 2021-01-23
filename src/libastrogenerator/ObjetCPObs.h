#ifndef OBJETCPOBS_H
#define OBJETCPOBS_H

#include "ObjetCP.h"
#include "ObjetObs.h"

class ObjetCPObs : public ObjetObs
{
    Q_OBJECT

public:
    ObjetCPObs(ObjetCP *objet, QDateTime debut, QDateTime fin);
    ObjetCPObs(QString ref, QDateTime debut, QDateTime fin);
    void initialiser(QString ref);
    int ngc() const;
    int messier() const;
    QString nomComplet(bool abrege = false) const;

protected:
    int m_ngc;
    int m_messier;
};

#endif // OBJETCP_HOBS
