#ifndef FENPRINCIPAL_H
#define FENPRINCIPAL_H

#include <QMainWindow>

#include "ActionsFenetre.h"
#include "Carteciel.h"
#include "Interface.h"
#include "InterfaceCreation.h"
#include "InterfaceLecture.h"
#include "Soiree.h"

class HomeWidget;

class FenPrincipal : public QMainWindow
{
	Q_OBJECT

public:
	FenPrincipal();
	void creerActions();
	void creerMenu();

	QSettings *getUser() const; // renvoit m_user
	void actionSoireesRecentes();
	void ajouterSoireeRecente(QString const &fichier);

public slots:
	void nouvelOngletSoiree(Soiree *soiree);
	void nouvelOngletCreation(double latitude, double longitude, QDateTime heureDebut, uint diametre, uint focale);
	bool fermerOnglet(int index);
	bool fermerOnglet(); // Pareil que effacerOnglet() mais pour l'onglet actif
	bool fermerOnglet(Interface *interface);
	void ouvrirSoa(QString fileSoa = QString());
	bool quitterApplication();
	void ouvrirObjetsRemarquables();
	void ouvrirCDS();
	void aPropos();
	void aide();

	void initialiserOngletActif(int index);
	void initialiserOngletActif();

	void afficherMessage(QString message, int duree = 2000);

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void saveAll();
	bool save(Soiree *night);

private:
	// Liste des actions dans une classe (pour être accessible partout
	ActionsFenetre *listeActions;

	// La classe Utilisateur qui stocke toutes les informations variables de l'utilisateur
	QSettings *m_user;

	// Pour gérer les soirées récentes
	QMenu *menuSoireesRecentes;

	QTabWidget *tabOnglets;
	HomeWidget *m_homeWidget = nullptr;
	QVector<Interface *> m_listeInterface;
};

#endif // FENPRINCIPAL_H
