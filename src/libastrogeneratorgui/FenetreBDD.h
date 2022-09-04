#ifndef FENETREBDD_H
#define FENETREBDD_H

#include <QCheckBox>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QStandardItemModel>
#include <QTableView>

class FenetreBDD : public QDialog
{
	Q_OBJECT
public:
	explicit FenetreBDD(QWidget *parent = nullptr);
	QString creerRequete() const;

public slots:
	void actualiserVue();
	void afficherObjet(QModelIndex index) const;
	void ouvrirCDS();

private:
	QStandardItemModel *m_modele;
	QTableView *m_vue;

	QLineEdit *champRecherche;

	QCheckBox *typeGalaxie;
	QCheckBox *typeAmasG;
	QCheckBox *typeNebuleuseR;
	QCheckBox *typeNebuleuseP;
	QCheckBox *typeAmasNebuleuse;
	QCheckBox *typeAmasOuvert;

	QCheckBox *difficulte0;
	QCheckBox *difficulte1;
	QCheckBox *difficulte2;
	QCheckBox *difficulte3;
	QCheckBox *difficulte4;

	QCheckBox *interet1;
	QCheckBox *interet2;
	QCheckBox *interet3;
	QCheckBox *interet4;

	QSpinBox *champMagnitude;

	QLabel *nombreResultat;
};

#endif // FENETREBDD_H
