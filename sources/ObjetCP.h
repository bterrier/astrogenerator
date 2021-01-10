#ifndef OBJETCP_H
#define OBJETCP_H

#include <QtGui>
#include "Objet.h"

class ObjetCP : public Objet
{
    public:
        ObjetCP();
        ObjetCP(QString ref);
        int ngc() const;
        int messier() const;
        QString ref() const;
        QString nomComplet() const;

    protected:
      int m_ngc;
      int m_messier;
      QString m_refBdd;
};

#endif // OBJETCP_H
