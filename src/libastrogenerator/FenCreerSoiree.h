#ifndef FENCREERSOIREE_H
#define FENCREERSOIREE_H

#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QFormLayout>
#include <QLabel>
#include <QProgressBar>

#include "FenPrincipal.h"

class FenCreerSoiree : public QDialog
{
	Q_OBJECT

public:
	explicit FenCreerSoiree(QWidget *parent = nullptr);
	QString creerTexteConstellation(); // Crée un texte en fonction des constellations selectionnées

signals:
	void nouvelOngletSoiree(Soiree *soiree);

public slots:
	void genererSoiree();
	void actualiserVilles(QString pays = "France");
	void changerVille(QString pays, QString ville, int dept);
	void actualiserCoordonnees(QString const &nom);
	void toutCocherConstellation();
	void toutDecocherConstellation();
	void updateTexteConstellation();
	void changerTelescope(QString const &telescope);
	void actualiserTelescope();

private:
	QComboBox *m_pays;
	QSpinBox *m_departement;
	QComboBox *m_villes;
	QDoubleSpinBox *m_latitude;
	QDoubleSpinBox *m_longitude;
	QDateEdit *m_date;
	QTimeEdit *m_heure;
	QSpinBox *m_duree;
	QSpinBox *m_dureeObjet;
	QComboBox *m_niveau;
	QCheckBox *m_planetes;
	QComboBox *m_telescope;
	QPushButton *m_submit;
	QProgressBar *m_progress;
	QPushButton *m_close;
	QFormLayout *layoutForm;
	QLabel *m_texteConstellation;

	QDialog *m_fenetreConstellation;
	QVector<QCheckBox *> m_checkBoxConstellation;

	QLineEdit *nom_telescope;
	QSpinBox *diametre;
	QSpinBox *focale;
	QComboBox *marquesList;
};

#endif // FENCREERSOIREE_H
