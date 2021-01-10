#ifndef FENPRINCIPAL_H
#define FENPRINCIPAL_H

#include <QMainWindow>
#include <QWebView>
#include <QWebFrame>
#include "Carteciel.h"
#include "Soiree.h"
#include "Interface.h"
#include "InterfaceLecture.h"
#include "InterfaceCreation.h"
#include "ActionsFenetre.h"

class FenPrincipal : public QMainWindow
{
    Q_OBJECT

    public:
        FenPrincipal();
        void creerActions();
        void creerMenu();

        QSettings *getUser() const; // renvoit m_user
        void actionSoireesRecentes();
        void ajouterSoireeRecente(QString const& fichier);



    public slots:
        void nouvelOngletSoiree(Soiree& soiree);
        void nouvelOngletCreation(double latitude, double longitude, QDateTime heureDebut, uint diametre, uint focale);
        bool fermerOnglet(int index);
        bool fermerOnglet(); // Pareil que effacerOnglet() mais pour l'onglet actif
        bool fermerOnglet(Interface *interface);
        void ouvrirSoa(QString fileSoa = QString());
        bool quitterApplication();
        void ouvrirUniversAstronomie();
        void ouvrirObjetsRemarquables();
        void ouvrirCDS();
        void aPropos();
        void aide();

        // Verifie � la demande de l'utilisateur
        void miseAJour1(); // Cr�e le module de recherche de mise � jour et l'active
        void miseAJour2(bool ok); // Traite le r�sultat de la mise � jour
        // V�rifie au demarrage
        void searchNewVersion(bool ok);
        static bool existsNewVersion(QString ligne);

        void initialiserOngletActif(int index);
        void initialiserOngletActif();

        void afficherMessage(QString message, int duree = 2000);


    protected:
        void closeEvent(QCloseEvent * event);

    private:
        // Liste des actions dans une classe (pour �tre accessible partout
        ActionsFenetre *listeActions;

        // Pour les mises � jour (module qui accedera au fichier)
        QWebView *m_view;
        QWebView *m_view2;

        // La classe Utilisateur qui stocke toutes les informations variables de l'utilisateur
        QSettings *m_user;

        // Pour g�rer les soir�es r�centes
        QMenu *menuSoireesRecentes;

        QTabWidget *tabOnglets;
        QWidget *widgetAccueil;
        QVector<Interface *> m_listeInterface;
        QStatusBar *barreStatut;

};

#endif // FENPRINCIPAL_H
