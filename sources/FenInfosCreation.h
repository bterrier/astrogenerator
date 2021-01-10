#ifndef FENINFOSCREATION_H
#define FENINFOSCREATION_H

#include <QComboBox>
#include <QDialog>
#include <QFormLayout>
#include "FenPrincipal.h"

/*
    Cette classe crée une fenêtre qui demande des infos préalables pour la création manuelle de la soirée
    Les informations rentrées ici seront ensuite envoyées à la fonction nouvelOngletCreation() de la classe
    FenPrincipal qui se chargera d'ouvrir un nouvel onglet.

    Cette classe est équivalente à FenCreerSoiree pour la génération de soirée

    Pour lancer cette fenêtre, il faut utiliser son slot exec()
*/


class FenInfosCreation : public QDialog
{
    Q_OBJECT
    public:
        FenInfosCreation(FenPrincipal *parent = 0);

    public slots:
        void actualiserCoordonnees(QString nom);
        void actualiserVilles(QString pays = "France");
        void creer();

    private :
        FenPrincipal *m_parent;

        QComboBox *m_pays;
        QSpinBox *m_departement;
        QComboBox *m_villes;
        QDoubleSpinBox *m_latitude;
        QDoubleSpinBox *m_longitude;
        QDateEdit *m_date;
        QTimeEdit *m_heure;
        QComboBox *m_telescope;
        QPushButton *m_submit;
        QPushButton *m_close;
        QFormLayout *layoutForm;
    
};

#endif // FENINFOSCREATION_H
