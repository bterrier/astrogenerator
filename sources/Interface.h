#ifndef INTERFACE_H
#define INTERFACE_H

#include <QtGui>
#include "ActionsFenetre.h"
#include "Soiree.h"

class Interface : public QWidget
{
    Q_OBJECT
    public:
        Interface(ActionsFenetre *listeActions, QWidget *parent = 0);
        bool isActive();
        void setActive(bool active);

    signals:
        void fermer(Interface *interface);
        void afficher(QString);

    public slots:
        void slotFermer();
        virtual void infosSoiree();
        virtual void griserActions();
        virtual Soiree* getSoiree() = 0;


    protected: // On y aura aussi accès depuis les élements filles
        ActionsFenetre *m_listeActions;
        bool m_active;
    
};

#endif // INTERFACE_H
