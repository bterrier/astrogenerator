#ifndef INTERFACECREATION_H
#define INTERFACECREATION_H

#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QListView>
#include <QSpinBox>
#include <QTableView>
#include <QTimeEdit>

#include "ActionsFenetre.h"
#include "FenPrincipal.h"
#include "Interface.h"
#include "Soiree.h"

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
	Soiree *getSoiree();

private:
	Soiree *m_soiree; // liste des objets à observer

	QStandardItemModel *m_modeleRecherche; // Contient les objets recherchés
	QListView *m_vueRecherche; // la vue associée

	QStandardItemModel *m_modele;
	QTableView *m_vue;
	QListView *m_vuePlanete;
	QStandardItemModel *m_modelePlanete;

	// Champ quand on demande à créer une soirée
	QDoubleSpinBox *m_champLatitude;
	QDoubleSpinBox *m_champLongitude;
	QSpinBox *m_champDiametre;
	QSpinBox *m_champFocale;
	QDateEdit *m_champDate;
	QTimeEdit *m_champHeureDebut;

	// Champ quand on ajoute un objet
	QDateTimeEdit *champDate;
	QSpinBox *champDureeObjet;

	// Actions spécifiques
	QAction *ajouterObjet;
	QAction *enleverObjet;
};

#endif // INTERFACECREATION_H
