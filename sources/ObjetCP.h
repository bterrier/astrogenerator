#ifndef OBJETCP_H
#define OBJETCP_H

#include "Objet.h"

class ObjetCP : public Objet
{
    Q_OBJECT

public:
    ObjetCP();
    explicit ObjetCP(QString ref);
    int ngc() const;
    int messier() const;
    QString ref() const;
    QString nomComplet(bool) const override;

protected:
    int m_ngc;
    int m_messier;
    QString m_refBdd;
};

#endif // OBJETCP_H
