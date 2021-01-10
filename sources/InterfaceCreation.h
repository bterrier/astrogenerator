#ifndef INTERFACECREATION_H
#define INTERFACECREATION_H

#include <QtGui>
#include "Soiree.h"
#include "ActionsFenetre.h"
#include "FenPrincipal.h"
#include "Interface.h"

class InterfaceCreation : public Interface
{
    Q_OBJECT

    public:
        InterfaceCreation(double latitude, double longitude, QDateTime heureDebut, uint diametre, uint focale, ActionsFenetre *listeAction, QWidget *parent);
        // Soiree toSoiree(); // Transforme l'objet InterfaceCreation en Soiree
        QStandardItemModel *rechercheToModele(QString recherche, bool ok = false);
        bool tempsEstPris(QDateTime temps, int duree) const;

    public slots:
        void nouvelleRecherche(QString texte);
        void ajoutObjet();
        void fenetrePlanete();
        void ajouterPlanete();
        void raffraichirListeObjet();
        void enleveObjet();
        void getDate();

        void monterObjet();
        void descendreObjet();
        void modifierObjet();
        void afficherInfosObjet(QModelIndex cells);

        void enregistrerSoiree();
        void infosSoiree();
        void griserActions();
        Soiree* getSoiree();


    private:
        Soiree *m_soiree; // liste des objets � observer

        QStandardItemModel *m_modeleRecherche; // Contient les objets recherch�s
        QListView *m_vueRecherche; // la vue associ�e

        QStandardItemModel *m_modele;
        QTableView *m_vue;
        QListView *m_vuePlanete;
        QStandardItemModel *m_modelePlanete;

        // Champ quand on demande � cr�er une soir�e
        QDoubleSpinBox *m_champLatitude;
        QDoubleSpinBox *m_champLongitude;
        QSpinBox *m_champDiametre;
        QSpinBox *m_champFocale;
        QDateEdit *m_champDate;
        QTimeEdit *m_champHeureDebut;

        // Champ quand on ajoute un objet
        QDateTimeEdit *champDate;
        QSpinBox *champDureeObjet;

        // Actions sp�cifiques
        QAction *ajouterObjet;
        QAction *enleverObjet;
};

#endif // INTERFACECREATION_H
