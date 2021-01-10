#ifndef FENINFOSCREATION_H
#define FENINFOSCREATION_H

#include <QComboBox>
#include <QDialog>
#include <QFormLayout>
#include "FenPrincipal.h"

/*
    Cette classe cr�e une fen�tre qui demande des infos pr�alables pour la cr�ation manuelle de la soir�e
    Les informations rentr�es ici seront ensuite envoy�es � la fonction nouvelOngletCreation() de la classe
    FenPrincipal qui se chargera d'ouvrir un nouvel onglet.

    Cette classe est �quivalente � FenCreerSoiree pour la g�n�ration de soir�e

    Pour lancer cette fen�tre, il faut utiliser son slot exec()
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
