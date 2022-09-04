#ifndef FENPREFERENCES_H
#define FENPREFERENCES_H

#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QLabel>
#include <QListWidget>

#include "FenPrincipal.h"
#include "eyepiecesettingwidget.h"

class FenPreferences : public QDialog
{
	Q_OBJECT
public:
	explicit FenPreferences(FenPrincipal *parent = nullptr);

signals:
	void telescopeChange(QString);
	void nouveauTelescope();
	void villeChange(QString, QString, int);

public slots:
	void changerOnglet(int onglet);
	void changerVilles(QString pays = "France");
	void changerCoordonnees(QString nom);
	void ajouterTelescope();

	void changerCouleurFond();
	void changerCouleurLegende();
	void changerCouleurEtoile();
	void changerCouleurConstellation();
	void changerCouleurObjet();

	void ouvrir(int page);

	void valider();
	void fermer();
	void initialiserValeur();
	void reinitialiser();

private:
	FenPrincipal *m_parent;
	QVector<QWidget *> m_listePages;
	QListWidget *m_menu;

	// PAGE LOCALISATION
	QComboBox *m_listePays;
	QSpinBox *m_listeDept;
	QComboBox *m_listeVilles;
	QDoubleSpinBox *m_latitude;
	QDoubleSpinBox *m_longitude;

	// PAGE TELESCOPE
	QComboBox *m_listeTelescope;
	QSpinBox *m_diametre;
	QSpinBox *m_focale;
	QLineEdit *m_nomTelescope;
	QComboBox *m_marque;
	QPushButton *m_boutonAjouterTelescope;

	// PAGE OCULAIRES
	EyepieceSettingWidget *m_eyepiecePage;

	// PAGE GENERATEUR
	QSlider *m_hauteurMinimum;
	QSlider *m_pauseMinimum;
	QSlider *m_noteAmasGlobulaire;
	QSlider *m_noteAmasNebuleuse;
	QSlider *m_noteNebuleusePlanetaire;
	QSlider *m_noteNebuleuseReflection;
	QSlider *m_noteEtoileDouble;
	QSlider *m_noteEtoileTriple;
	QSlider *m_noteGalaxie;
	QSlider *m_noteAmasOuvert;

	QLabel *m_labelPauseMinimum;
	QLabel *m_labelHauteurMinimum;
	QLabel *m_labelNoteAmasGlobulaire;
	QLabel *m_labelNoteAmasNebuleuse;
	QLabel *m_labelNoteNebuleusePlanetaire;
	QLabel *m_labelNoteNebuleuseReflection;
	QLabel *m_labelNoteEtoileDouble;
	QLabel *m_labelNoteEtoileTriple;
	QLabel *m_labelNoteGalaxie;
	QLabel *m_labelNoteAmasOuvert;

	// PAGE CARTE CIEL
	QPushButton *m_boutonCouleurFond;
	QPushButton *m_boutonCouleurLegende;
	QPushButton *m_boutonCouleurObjet;
	QPushButton *m_boutonCouleurEtoile;
	QPushButton *m_boutonCouleurConstellation;
	QLabel *m_labelCouleurFond;
	QLabel *m_labelCouleurLegende;
	QLabel *m_labelCouleurObjet;
	QLabel *m_labelCouleurEtoile;
	QLabel *m_labelCouleurConstellation;

	// BOUTONS
	QPushButton *m_boutonValider;
	QPushButton *m_boutonAnnuler;
	QPushButton *m_boutonReinitialiser;
};

#endif // FENPREFERENCES_H
