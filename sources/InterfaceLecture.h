#ifndef INTERFACELECTURE_H
#define INTERFACELECTURE_H

#include <QtGui>
#include "ActionsFenetre.h"
#include "Soiree.h"
#include "Carteciel.h"
#include "Interface.h"

class InterfaceLecture : public Interface
{
    Q_OBJECT
    public:
        InterfaceLecture(Soiree *soiree, ActionsFenetre *listeActionsParam, QWidget *parent = 0);

    signals:

    public slots:
        void monterObjet();
        void descendreObjet();
        void supprimerObjet();
        void modifierObjet();
        void ajouterObjet();
        void ajouterPlanete();

        void setCouleurFond();
        void setCouleurEtoiles();
        void setCouleurLegende();
        void setCouleurObjet();
        void setCouleurConstellations();

        void enregistrerSoiree();
        void carteCiel();
        void afficherInfosObjet(QModelIndex cells);
        void diaporama();
        void toXML();
        void imprimer();
        void toPDF();
        void infosSoiree();
        void griserActions();

        Soiree *getSoiree();

    private:
        QStandardItemModel *m_modele;
        QTableView *m_vue;
        Soiree *m_soiree;
        Carteciel *m_carte;
    
};

#endif // INTERFACELECTURE_H
