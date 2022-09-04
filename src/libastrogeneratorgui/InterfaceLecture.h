#ifndef INTERFACELECTURE_H
#define INTERFACELECTURE_H

#include <QStandardItemModel>
#include <QTableView>

#include "ActionsFenetre.h"
#include "Carteciel.h"
#include "Interface.h"
#include "Soiree.h"

#include "nightmodel.h"

class InterfaceLecture : public Interface
{
	Q_OBJECT
public:
	InterfaceLecture(Soiree *soiree, ActionsFenetre *listeActionsParam, QWidget *parent = nullptr);

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

	// void enregistrerSoiree();
	void carteCiel();
	void afficherInfosObjet(QModelIndex cells);
	void diaporama();
	void toXML();
	void imprimer();
	void toPDF();
	void infosSoiree() override;
	void griserActions() override;

	Soiree *getSoiree() override;

private:
	NightModel *m_model;
	QTableView *m_vue;
	Soiree *m_soiree;
	Carteciel *m_carte;
};

#endif // INTERFACELECTURE_H
