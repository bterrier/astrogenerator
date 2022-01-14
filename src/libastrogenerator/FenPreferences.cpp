#include "FenPreferences.h"
#include "Constantes.h"

#include <QColorDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlQuery>

FenPreferences::FenPreferences(FenPrincipal *parent) :
    QDialog(parent)
{
	m_parent = parent;

	setWindowTitle(tr("Préférences"));
	// setMaximumWidth(600);
	setMinimumWidth(600);

	QHBoxLayout *layoutPrincipal = new QHBoxLayout;
	//        layoutPrincipal->setAlignment(Qt::AlignLeft);

	m_menu = new QListWidget;
	m_menu->setMaximumWidth(150);
	m_menu->setMinimumWidth(150);
	m_menu->setSelectionRectVisible(false);

	QStringList listeMenu;
	listeMenu << tr("Localisation") << tr("Télescope") << tr("Oculaires") << tr("Générateur") << tr("Carte du ciel");
	m_menu->addItems(listeMenu);
	layoutPrincipal->addWidget(m_menu, 0, Qt::AlignLeft);

	connect(m_menu, &QListWidget::currentRowChanged, this, &FenPreferences::changerOnglet);

	// On crée toutes les pages

	// PAGE LOCALISATION

	QGroupBox *groupBoxLocalisation = new QGroupBox(tr("Localisation"));
	QGroupBox *groupBoxCoordonnees = new QGroupBox(tr("Coordonnées"));

	QFormLayout *layoutFormLocalisation = new QFormLayout;
	QFormLayout *layoutFormCoordonnees = new QFormLayout;

	m_listePays = new QComboBox;

	QSqlQuery *requete = new QSqlQuery;
	requete->exec("SELECT pays FROM villes_monde GROUP BY pays ORDER BY pays");
	while (requete->next())
		m_listePays->addItem(requete->value(0).toString());

	layoutFormLocalisation->addRow("&Pays", m_listePays);

	m_listeDept = new QSpinBox;
	m_listeDept->setMinimum(1);
	m_listeDept->setMaximum(95);

	m_listeVilles = new QComboBox;

	layoutFormLocalisation->addRow(tr("&Département"), m_listeDept);
	layoutFormLocalisation->addRow(tr("&Ville"), m_listeVilles);

	connect(m_listePays, qOverload<int>(&QComboBox::currentIndexChanged), this, [this]() { changerVilles(m_listePays->currentText()); });
	connect(m_listeDept, qOverload<int>(&QSpinBox::valueChanged), this, [this]() { changerVilles(); });
	connect(m_listeVilles, qOverload<int>(&QComboBox::currentIndexChanged), this, [this]() { changerCoordonnees(m_listePays->currentText()); });

	groupBoxLocalisation->setLayout(layoutFormLocalisation);

	m_latitude = new QDoubleSpinBox;
	m_latitude->setDecimals(3);
	m_latitude->setMaximum(90);
	m_latitude->setMinimum(-90);
	m_longitude = new QDoubleSpinBox;
	m_longitude->setDecimals(3);
	m_longitude->setMaximum(180);
	m_longitude->setMinimum(-180);

	layoutFormCoordonnees->addRow(tr("L&atitude"), m_latitude);
	layoutFormCoordonnees->addRow(tr("L&ongitude"), m_longitude);

	groupBoxCoordonnees->setLayout(layoutFormCoordonnees);

	QWidget *page_localisation = new QWidget;
	page_localisation->setFixedWidth(350);
	QVBoxLayout *layoutLocalisation = new QVBoxLayout;
	layoutLocalisation->addWidget(groupBoxLocalisation);
	layoutLocalisation->addWidget(groupBoxCoordonnees);
	page_localisation->setLayout(layoutLocalisation);

	m_listePages.push_back(page_localisation);

	layoutPrincipal->addWidget(page_localisation);

	// PAGE TELESCOPE
	QGroupBox *groupBoxTelescopeListe = new QGroupBox(tr("Liste des télescopes"));
	QGroupBox *groupBoxTelescopeAdd = new QGroupBox(tr("Ajouter des télescopes"));

	QFormLayout *layoutFormTelescopeListe = new QFormLayout;
	QFormLayout *layoutFormTelescopeAdd = new QFormLayout;

	m_listeTelescope = new QComboBox;
	requete->exec("SELECT nom FROM telescope ORDER BY nom");
	while (requete->next())
		m_listeTelescope->addItem(requete->value(0).toString());

	m_diametre = new QSpinBox;
	m_diametre->setMaximum(DIAMETRE_MAXIMUM);
	m_diametre->setValue(DIAMETRE_DEFAULT);
	m_diametre->setMinimum(DIAMETRE_MINIMUM);
	m_focale = new QSpinBox;
	m_focale->setMinimum(FOCALE_MINIMUM);
	m_focale->setValue(FOCALE_DEFAULT);
	m_focale->setMaximum(FOCALE_MAXIMUM);

	m_nomTelescope = new QLineEdit;
	m_boutonAjouterTelescope = new QPushButton(tr("Ajouter le télescope"));
	connect(m_boutonAjouterTelescope, &QPushButton::clicked, this, &FenPreferences::ajouterTelescope);

	m_marque = new QComboBox;
	QStringList listeMarque;
	requete->exec("SELECT marque FROM telescope GROUP BY marque ORDER BY marque");
	while (requete->next())
		listeMarque << requete->value(0).toString();
	m_marque->addItems(listeMarque);

	layoutFormTelescopeListe->addRow(tr("Liste des &télescopes"), m_listeTelescope);
	groupBoxTelescopeListe->setLayout(layoutFormTelescopeListe);

	layoutFormTelescopeAdd->addRow(tr("Nom du télescope (unique)"), m_nomTelescope);
	layoutFormTelescopeAdd->addRow(tr("Diamètre (mm)"), m_diametre);
	layoutFormTelescopeAdd->addRow(tr("Focale (mm)"), m_focale);
	layoutFormTelescopeAdd->addRow(tr("Marque"), m_marque);
	layoutFormTelescopeAdd->addWidget(m_boutonAjouterTelescope);
	groupBoxTelescopeAdd->setLayout(layoutFormTelescopeAdd);

	QVBoxLayout *layoutTelescope = new QVBoxLayout;
	layoutTelescope->addWidget(groupBoxTelescopeListe);
	layoutTelescope->addWidget(groupBoxTelescopeAdd);

	QWidget *page_telescope = new QWidget;
	page_telescope->setFixedWidth(350);
	page_telescope->setLayout(layoutTelescope);
	page_telescope->setVisible(false);
	m_listePages.push_back(page_telescope);
	layoutPrincipal->addWidget(page_telescope);

	// PAGE OCULAIRES
	QGroupBox *groupBoxOculaireListe = new QGroupBox(tr("Liste des oculaires"));
	QHBoxLayout *layoutOculaires = new QHBoxLayout;
	QVBoxLayout *layoutV1 = new QVBoxLayout;
	QVBoxLayout *layoutV2 = new QVBoxLayout;
	QVBoxLayout *layoutV3 = new QVBoxLayout;

	for (int i(0); i < 24; i++) {
		QCheckBox *b = new QCheckBox(QString::number(i + 6) + tr(" mm", "Symbole des millimètres (laisser l'espace avant)"));
		m_listeOculaires.push_back(b);

		if (i < 8)
			layoutV1->addWidget(b);
		else if (i < 16)
			layoutV2->addWidget(b);
		else
			layoutV3->addWidget(b);
	}
	layoutOculaires->addLayout(layoutV1);
	layoutOculaires->addLayout(layoutV2);
	layoutOculaires->addLayout(layoutV3);

	QHBoxLayout *layoutObligatoire = new QHBoxLayout;
	groupBoxOculaireListe->setLayout(layoutOculaires);
	layoutObligatoire->addWidget(groupBoxOculaireListe);

	QWidget *page_oculaires = new QWidget;
	page_oculaires->setFixedWidth(350);
	page_oculaires->setLayout(layoutObligatoire);
	page_oculaires->setVisible(false);
	m_listePages.push_back(page_oculaires);
	layoutPrincipal->addWidget(page_oculaires);

	// PAGE GENERATEUR

	QGroupBox *groupBoxGenerateurParam = new QGroupBox(tr("Paramètres d'observation des objets"));
	QGroupBox *groupBoxGenerateurPriorite = new QGroupBox(tr("Priorité des objets"));

	QFormLayout *layoutGenerateurParam = new QFormLayout;
	QFormLayout *layoutGenerateurPriorite = new QFormLayout;

	QHBoxLayout *layoutHauteurMinimum = new QHBoxLayout;
	QHBoxLayout *layoutPauseMinimum = new QHBoxLayout;
	QHBoxLayout *layoutNoteAmasGlobulaire = new QHBoxLayout;
	QHBoxLayout *layoutNoteAmasNebuleuse = new QHBoxLayout;
	QHBoxLayout *layoutNoteAmasOuvert = new QHBoxLayout;
	QHBoxLayout *layoutNoteNebuleusePlanetaire = new QHBoxLayout;
	QHBoxLayout *layoutNoteNebuleuseReflection = new QHBoxLayout;
	QHBoxLayout *layoutNoteEtoileDouble = new QHBoxLayout;
	QHBoxLayout *layoutNoteEtoileTriple = new QHBoxLayout;
	QHBoxLayout *layoutNoteGalaxie = new QHBoxLayout;

	m_labelHauteurMinimum = new QLabel;
	m_labelPauseMinimum = new QLabel;
	m_labelNoteAmasGlobulaire = new QLabel;
	m_labelNoteAmasNebuleuse = new QLabel;
	m_labelNoteNebuleusePlanetaire = new QLabel;
	m_labelNoteNebuleuseReflection = new QLabel;
	m_labelNoteEtoileDouble = new QLabel;
	m_labelNoteEtoileTriple = new QLabel;
	m_labelNoteGalaxie = new QLabel;
	m_labelNoteAmasOuvert = new QLabel;

	m_pauseMinimum = new QSlider(Qt::Horizontal);
	m_pauseMinimum->setMinimum(PAUSE_MINIMUM_OBJET);
	m_pauseMinimum->setMaximum(PAUSE_MAXIMUM_OBJET);
	m_hauteurMinimum = new QSlider(Qt::Horizontal);
	m_hauteurMinimum->setMinimum(0);
	m_hauteurMinimum->setMaximum(70);
	m_noteAmasGlobulaire = new QSlider(Qt::Horizontal);
	m_noteAmasGlobulaire->setMinimum(0);
	m_noteAmasGlobulaire->setMaximum(15);
	m_noteAmasNebuleuse = new QSlider(Qt::Horizontal);
	m_noteAmasNebuleuse->setMinimum(0);
	m_noteAmasNebuleuse->setMaximum(15);
	m_noteNebuleusePlanetaire = new QSlider(Qt::Horizontal);
	m_noteNebuleusePlanetaire->setMinimum(0);
	m_noteNebuleusePlanetaire->setMaximum(15);
	m_noteNebuleuseReflection = new QSlider(Qt::Horizontal);
	m_noteNebuleuseReflection->setMinimum(0);
	m_noteNebuleuseReflection->setMaximum(15);
	m_noteEtoileDouble = new QSlider(Qt::Horizontal);
	m_noteEtoileDouble->setMinimum(0);
	m_noteEtoileDouble->setMaximum(15);
	m_noteEtoileTriple = new QSlider(Qt::Horizontal);
	m_noteEtoileTriple->setMinimum(0);
	m_noteEtoileTriple->setMaximum(15);
	m_noteGalaxie = new QSlider(Qt::Horizontal);
	m_noteGalaxie->setMinimum(0);
	m_noteGalaxie->setMaximum(15);
	m_noteAmasOuvert = new QSlider(Qt::Horizontal);
	m_noteAmasOuvert->setMinimum(0);
	m_noteAmasOuvert->setMaximum(15);

	connect(m_hauteurMinimum, &QSlider::valueChanged, m_labelHauteurMinimum, qOverload<int>(&QLabel::setNum));
	connect(m_pauseMinimum, &QSlider::valueChanged, m_labelPauseMinimum, qOverload<int>(&QLabel::setNum));
	connect(m_noteAmasGlobulaire, &QSlider::valueChanged, m_labelNoteAmasGlobulaire, qOverload<int>(&QLabel::setNum));
	connect(m_noteAmasNebuleuse, &QSlider::valueChanged, m_labelNoteAmasNebuleuse, qOverload<int>(&QLabel::setNum));
	connect(m_noteNebuleuseReflection, &QSlider::valueChanged, m_labelNoteNebuleuseReflection, qOverload<int>(&QLabel::setNum));
	connect(m_noteNebuleusePlanetaire, &QSlider::valueChanged, m_labelNoteNebuleusePlanetaire, qOverload<int>(&QLabel::setNum));
	connect(m_noteEtoileDouble, &QSlider::valueChanged, m_labelNoteEtoileDouble, qOverload<int>(&QLabel::setNum));
	connect(m_noteEtoileTriple, &QSlider::valueChanged, m_labelNoteEtoileTriple, qOverload<int>(&QLabel::setNum));
	connect(m_noteAmasOuvert, &QSlider::valueChanged, m_labelNoteAmasOuvert, qOverload<int>(&QLabel::setNum));
	connect(m_noteGalaxie, &QSlider::valueChanged, m_labelNoteGalaxie, qOverload<int>(&QLabel::setNum));

	layoutHauteurMinimum->addWidget(m_hauteurMinimum);
	layoutHauteurMinimum->addWidget(m_labelHauteurMinimum);
	layoutHauteurMinimum->setAlignment(Qt::AlignLeft);

	layoutPauseMinimum->addWidget(m_pauseMinimum);
	layoutPauseMinimum->addWidget(m_labelPauseMinimum);
	layoutPauseMinimum->setAlignment(Qt::AlignLeft);

	layoutNoteAmasGlobulaire->addWidget(m_noteAmasGlobulaire);
	layoutNoteAmasGlobulaire->addWidget(m_labelNoteAmasGlobulaire);
	layoutNoteAmasGlobulaire->setAlignment(Qt::AlignLeft);

	layoutNoteAmasNebuleuse->addWidget(m_noteAmasNebuleuse);
	layoutNoteAmasNebuleuse->addWidget(m_labelNoteAmasNebuleuse);
	layoutNoteAmasNebuleuse->setAlignment(Qt::AlignLeft);

	layoutNoteAmasOuvert->addWidget(m_noteAmasOuvert);
	layoutNoteAmasOuvert->addWidget(m_labelNoteAmasOuvert);
	layoutNoteAmasOuvert->setAlignment(Qt::AlignLeft);

	layoutNoteNebuleusePlanetaire->addWidget(m_noteNebuleusePlanetaire);
	layoutNoteNebuleusePlanetaire->addWidget(m_labelNoteNebuleusePlanetaire);
	layoutNoteNebuleusePlanetaire->setAlignment(Qt::AlignLeft);

	layoutNoteNebuleuseReflection->addWidget(m_noteNebuleuseReflection);
	layoutNoteNebuleuseReflection->addWidget(m_labelNoteNebuleuseReflection);
	layoutNoteNebuleuseReflection->setAlignment(Qt::AlignLeft);

	layoutNoteEtoileDouble->addWidget(m_noteEtoileDouble);
	layoutNoteEtoileDouble->addWidget(m_labelNoteEtoileDouble);
	layoutNoteEtoileDouble->setAlignment(Qt::AlignLeft);

	layoutNoteEtoileTriple->addWidget(m_noteEtoileTriple);
	layoutNoteEtoileTriple->addWidget(m_labelNoteEtoileTriple);
	layoutNoteEtoileTriple->setAlignment(Qt::AlignLeft);

	layoutNoteGalaxie->addWidget(m_noteGalaxie);
	layoutNoteGalaxie->addWidget(m_labelNoteGalaxie);
	layoutNoteGalaxie->setAlignment(Qt::AlignLeft);

	layoutGenerateurParam->addRow(tr("Hauteur minimum (en °)"), layoutHauteurMinimum);
	layoutGenerateurParam->addRow(tr("Pause minimum entre 2 objets (min)"), layoutPauseMinimum);

	layoutGenerateurPriorite->addRow(tr("Amas Globulaire"), layoutNoteAmasGlobulaire);
	layoutGenerateurPriorite->addRow(tr("Amas+Nébuleuse"), layoutNoteAmasNebuleuse);
	layoutGenerateurPriorite->addRow(tr("Amas ouvert"), layoutNoteAmasOuvert);
	layoutGenerateurPriorite->addRow(tr("Nébuleuse planétaire"), layoutNoteNebuleusePlanetaire);
	layoutGenerateurPriorite->addRow(tr("Nébuleuse en réflection"), layoutNoteNebuleuseReflection);
	layoutGenerateurPriorite->addRow(tr("Etoile double"), layoutNoteEtoileDouble);
	layoutGenerateurPriorite->addRow(tr("Etoile triple"), layoutNoteEtoileTriple);
	layoutGenerateurPriorite->addRow(tr("Galaxie"), layoutNoteGalaxie);

	groupBoxGenerateurParam->setLayout(layoutGenerateurParam);
	groupBoxGenerateurPriorite->setLayout(layoutGenerateurPriorite);

	QVBoxLayout *layoutGenerateur = new QVBoxLayout;
	layoutGenerateur->addWidget(groupBoxGenerateurParam);
	layoutGenerateur->addWidget(groupBoxGenerateurPriorite);

	QWidget *page_generateur = new QWidget;
	page_generateur->setFixedWidth(350);
	page_generateur->setLayout(layoutGenerateur);
	page_generateur->setVisible(false);
	m_listePages.push_back(page_generateur);
	layoutPrincipal->addWidget(page_generateur);

	// PAGE CARTE CIEL

	QGroupBox *groupBoxCouleurCarte = new QGroupBox(tr("Couleurs par défaut de la carte du ciel"));
	QFormLayout *layoutFormCouleurCarte = new QFormLayout;

	m_boutonCouleurFond = new QPushButton(tr("Changer"));
	m_boutonCouleurLegende = new QPushButton(tr("Changer"));
	m_boutonCouleurObjet = new QPushButton(tr("Changer"));
	m_boutonCouleurEtoile = new QPushButton(tr("Changer"));
	m_boutonCouleurConstellation = new QPushButton(tr("Changer"));

	connect(m_boutonCouleurFond, &QPushButton::clicked, this, &FenPreferences::changerCouleurFond);
	connect(m_boutonCouleurLegende, &QPushButton::clicked, this, &FenPreferences::changerCouleurLegende);
	connect(m_boutonCouleurObjet, &QPushButton::clicked, this, &FenPreferences::changerCouleurObjet);
	connect(m_boutonCouleurEtoile, &QPushButton::clicked, this, &FenPreferences::changerCouleurEtoile);
	connect(m_boutonCouleurConstellation, &QPushButton::clicked, this, &FenPreferences::changerCouleurConstellation);

	m_labelCouleurFond = new QLabel;
	m_labelCouleurLegende = new QLabel;
	m_labelCouleurObjet = new QLabel;
	m_labelCouleurEtoile = new QLabel;
	m_labelCouleurConstellation = new QLabel;

	QHBoxLayout *layoutCouleurFond = new QHBoxLayout;
	layoutCouleurFond->addWidget(m_boutonCouleurFond);
	layoutCouleurFond->addWidget(m_labelCouleurFond);
	QHBoxLayout *layoutCouleurLegende = new QHBoxLayout;
	layoutCouleurLegende->addWidget(m_boutonCouleurLegende);
	layoutCouleurLegende->addWidget(m_labelCouleurLegende);
	QHBoxLayout *layoutCouleurObjet = new QHBoxLayout;
	layoutCouleurObjet->addWidget(m_boutonCouleurObjet);
	layoutCouleurObjet->addWidget(m_labelCouleurObjet);
	QHBoxLayout *layoutCouleurConstellation = new QHBoxLayout;
	layoutCouleurConstellation->addWidget(m_boutonCouleurConstellation);
	layoutCouleurConstellation->addWidget(m_labelCouleurConstellation);
	QHBoxLayout *layoutCouleurEtoile = new QHBoxLayout;
	layoutCouleurEtoile->addWidget(m_boutonCouleurEtoile);
	layoutCouleurEtoile->addWidget(m_labelCouleurEtoile);

	layoutFormCouleurCarte->addRow(tr("Couleur du fond"), layoutCouleurFond);
	layoutFormCouleurCarte->addRow(tr("Couleur des légendes"), layoutCouleurLegende);
	layoutFormCouleurCarte->addRow(tr("Couleur des objets"), layoutCouleurObjet);
	layoutFormCouleurCarte->addRow(tr("Couleur des constellations"), layoutCouleurConstellation);
	layoutFormCouleurCarte->addRow(tr("Couleur des étoiles"), layoutCouleurEtoile);

	groupBoxCouleurCarte->setLayout(layoutFormCouleurCarte);
	QVBoxLayout *layoutOk = new QVBoxLayout;
	layoutOk->addWidget(groupBoxCouleurCarte);

	QWidget *page_carteciel = new QWidget;
	page_carteciel->setFixedWidth(350);
	page_carteciel->setLayout(layoutOk);
	page_carteciel->setVisible(false);
	m_listePages.push_back(page_carteciel);
	layoutPrincipal->addWidget(page_carteciel);

	// LAYOUT DES BOUTONS
	QVBoxLayout *layoutPrincipalBoutons = new QVBoxLayout;
	layoutPrincipalBoutons->setAlignment(Qt::AlignTop);
	m_boutonAnnuler = new QPushButton(tr("Annuler"));
	m_boutonValider = new QPushButton(tr("Valider"));
	m_boutonReinitialiser = new QPushButton(tr("Réinitialiser"));

	layoutPrincipalBoutons->addWidget(m_boutonValider);
	layoutPrincipalBoutons->addWidget(m_boutonAnnuler);
	layoutPrincipalBoutons->addWidget(m_boutonReinitialiser);

	layoutPrincipal->addLayout(layoutPrincipalBoutons);

	connect(m_boutonValider, &QPushButton::clicked, this, &FenPreferences::valider);
	connect(m_boutonAnnuler, &QPushButton::clicked, this, &FenPreferences::fermer);
	connect(m_boutonReinitialiser, &QPushButton::clicked, this, &FenPreferences::reinitialiser);

	initialiserValeur(); // Initialise tout
	setLayout(layoutPrincipal);
}
void FenPreferences::changerOnglet(int onglet)
{
	for (int i(0); i < m_listePages.count(); i++) {
		m_listePages.at(i)->setVisible(false);
		m_menu->item(i)->setBackground(QColor(255, 255, 255));
	}
	m_listePages.at(onglet)->setVisible(true);
	m_menu->item(onglet)->setBackground(QColor(220, 220, 220));
}
void FenPreferences::changerVilles(QString pays)
{
	if (pays != "France") {
		QSqlQuery *requeteVilles = new QSqlQuery;
		requeteVilles->prepare("SELECT nom FROM villes_monde WHERE pays = :pays ORDER BY nom");
		requeteVilles->bindValue(":pays", pays);
		requeteVilles->exec();
		m_listeVilles->clear();
		while (requeteVilles->next()) {
			m_listeVilles->addItem(requeteVilles->value(0).toString());
		}
		m_listeDept->setDisabled(true);
	} else {
		m_listeDept->setDisabled(false);
		QSqlQuery *requeteVilles = new QSqlQuery("SELECT nom FROM villes_france WHERE departement = " + QString::number(m_listeDept->value()) + " ORDER BY nom");
		m_listeVilles->clear();
		while (requeteVilles->next()) {
			m_listeVilles->addItem(requeteVilles->value(0).toString());
		}
	}
}

void FenPreferences::changerCoordonnees(QString nom)
{
	if (m_listePays->currentText() == "France") {
		//! \todo Fix warning when nom === ""
		QSqlQuery *requeteCoordonnees = new QSqlQuery("SELECT latitude, longitude FROM villes_france WHERE nom = '" + nom + "' AND departement = " + QString::number(m_listeDept->value()));
		requeteCoordonnees->next();
		m_latitude->setValue(requeteCoordonnees->value(0).toDouble());
		m_longitude->setValue(requeteCoordonnees->value(1).toDouble());
	} else {
		QSqlQuery *requeteCoordonnees = new QSqlQuery;
		requeteCoordonnees->prepare("SELECT latitude, longitude FROM villes_monde WHERE nom = :nom AND pays = :pays");
		requeteCoordonnees->bindValue(":nom", nom);
		requeteCoordonnees->bindValue(":pays", m_listePays->currentText());
		requeteCoordonnees->exec();
		requeteCoordonnees->next();

		m_latitude->setValue(requeteCoordonnees->value(0).toDouble());
		m_longitude->setValue(requeteCoordonnees->value(1).toDouble());
	}
}
void FenPreferences::ajouterTelescope()
{
	QSqlQuery requete("SELECT COUNT(*) as nbr FROM telescope WHERE nom = :nom");
	requete.bindValue(":nom", m_nomTelescope->text());
	requete.exec();
	requete.next();

	if (requete.value(0).toInt() == 0) {
		requete.prepare("INSERT INTO telescope (nom,diametre,focale,marque) VALUES(:nom,:diametre,:focale,:marque)");
		requete.bindValue(":nom", m_nomTelescope->text());
		requete.bindValue(":diametre", m_diametre->value());
		requete.bindValue(":focale", m_focale->value());
		requete.bindValue(":marque", m_marque->currentText());
		if (requete.exec()) {
			requete.exec("SELECT nom FROM telescope ORDER BY nom");
			m_listeTelescope->clear();
			while (requete.next()) {
				m_listeTelescope->addItem(requete.value(0).toString());
			}
			m_listeTelescope->setCurrentIndex(m_listeTelescope->findText(m_nomTelescope->text()));
			emit nouveauTelescope();
			QMessageBox::information(this, tr("Télescope ajouté"), tr("Le télescope a été ajouté avec succès à la base de données."));
		}
	} else
		QMessageBox::critical(this, tr("Ajout impossible"), tr("Impossible d'ajouter le télescope car il est déjà dans la base de données."));
}
void FenPreferences::changerCouleurConstellation()
{
	QColor color = QColorDialog::getColor(QColor(m_labelCouleurConstellation->text()));
	m_labelCouleurConstellation->setText(color.name().toUpper());
	m_labelCouleurConstellation->setStyleSheet("color:" + color.name());
}
void FenPreferences::changerCouleurEtoile()
{
	QColor color = QColorDialog::getColor(QColor(m_labelCouleurEtoile->text()));
	m_labelCouleurEtoile->setText(color.name().toUpper());
	m_labelCouleurEtoile->setStyleSheet("color:" + color.name());
}
void FenPreferences::changerCouleurFond()
{
	QColor color = QColorDialog::getColor(QColor(m_labelCouleurFond->text()));
	m_labelCouleurFond->setText(color.name().toUpper());
	m_labelCouleurFond->setStyleSheet("color:" + color.name());
}
void FenPreferences::changerCouleurLegende()
{
	QColor color = QColorDialog::getColor(QColor(m_labelCouleurLegende->text()));
	m_labelCouleurLegende->setText(color.name().toUpper());
	m_labelCouleurLegende->setStyleSheet("color:" + color.name());
}
void FenPreferences::changerCouleurObjet()
{
	QColor color = QColorDialog::getColor(QColor(m_labelCouleurObjet->text()));
	m_labelCouleurObjet->setText(color.name().toUpper());
	m_labelCouleurObjet->setStyleSheet("color:" + color.name());
}
void FenPreferences::valider()
{
	QSettings *s = m_parent->getUser();

	s->setValue("localisation/pays", m_listePays->currentText());
	s->setValue("localisation/ville", m_listeVilles->currentText());
	s->setValue("localisation/latitude", m_latitude->value());
	s->setValue("localisation/longitude", m_longitude->value());

	emit villeChange(m_listePays->currentText(), m_listeVilles->currentText(), m_listeDept->value());

	QSqlQuery requete("SELECT diametre, focale, nom, marque FROM telescope WHERE nom = :nom");
	requete.bindValue(":nom", m_listeTelescope->currentText());
	requete.exec();
	requete.next();
	s->setValue("telescope/diametre", requete.value(0).toInt());
	s->setValue("telescope/focale", requete.value(1).toInt());
	s->setValue("telescope/marque", requete.value(3).toString());
	s->setValue("telescope/nom", requete.value(2).toString());

	emit telescopeChange(requete.value(2).toString());

	QString taille, oculaires;
	for (int i(0); i < m_listeOculaires.count(); i++) {
		if (m_listeOculaires.at(i)->isChecked()) {
			taille = m_listeOculaires.at(i)->text().left(m_listeOculaires.at(i)->text().count() - 3);
			oculaires += "|" + taille;
		}
	}
	oculaires = oculaires.right(oculaires.count() - 1);
	s->setValue("oculaires", oculaires);

	s->setValue("couleurCarte/fond", m_labelCouleurFond->text());
	s->setValue("couleurCarte/etoile", m_labelCouleurEtoile->text());
	s->setValue("couleurCarte/constellation", m_labelCouleurConstellation->text());
	s->setValue("couleurCarte/objet", m_labelCouleurObjet->text());
	s->setValue("couleurCarte/legende", m_labelCouleurLegende->text());

	s->setValue("generateur/hauteurMin", m_hauteurMinimum->value());
	s->setValue("generateur/pauseMin", m_pauseMinimum->value());
	s->setValue("generateur/amasGlobulaire", m_noteAmasGlobulaire->value());
	s->setValue("generateur/amasNebuleuse", m_noteAmasNebuleuse->value());
	s->setValue("generateur/amasOuvert", m_noteAmasOuvert->value());
	s->setValue("generateur/nebuleuseReflection", m_noteNebuleuseReflection->value());
	s->setValue("generateur/nebuleusePlanetaire", m_noteNebuleusePlanetaire->value());
	s->setValue("generateur/etoileTriple", m_noteEtoileTriple->value());
	s->setValue("generateur/etoileDouble", m_noteEtoileDouble->value());
	s->setValue("generateur/galaxie", m_noteGalaxie->value());

	this->close();
}
void FenPreferences::initialiserValeur()
{
	// INITIALISE TOUTES LES VALEURS EN FONCTION DU REGISTRE

	// PAGE LOCALISATION
	m_listeVilles->clear(); // on vide les villes
	QSqlQuery *requete = new QSqlQuery;

	// on met soit le pays de l'utilisateur, soit le défaut
	m_listePays->setCurrentIndex(m_listePays->findText(m_parent->getUser()->value("localisation/pays", PAYS_DEFAUT).toString()));

	if (m_listePays->currentText() != "France") {
		// On ajoute tous les items qui correspondent à ce pays
		m_listeDept->setDisabled(true);
		requete->prepare("SELECT nom FROM villes_monde WHERE pays = :pays ORDER BY nom");
		requete->bindValue(":pays", m_listePays->currentText());
		requete->exec();
		QStringList listeVilles;
		while (requete->next())
			listeVilles << requete->value(0).toString();
		m_listeVilles->addItems(listeVilles);

		// On met la ville par défaut
		m_listeVilles->setCurrentIndex(m_listeVilles->findText(m_parent->getUser()->value("localisation/ville", VILLE_DEFAUT).toString()));
	} else {
		// On trouve le département de la ville
		int dept;
		requete->prepare("SELECT departement FROM villes_france WHERE nom = :nom");
		requete->bindValue(":nom", m_parent->getUser()->value("localisation/ville", VILLE_DEFAUT).toString());
		requete->exec();
		requete->next();
		dept = requete->value(0).toInt();
		m_listeDept->setValue(dept);

		// On ajoute toutes les villes de ce département
		requete->prepare("SELECT nom FROM villes_france WHERE departement = :dept ORDER BY nom");
		requete->bindValue(":dept", dept);
		requete->exec();
		while (requete->next())
			m_listeVilles->addItem(requete->value(0).toString());

		// On met la ville par défaut
		m_listeVilles->setCurrentIndex(m_listeVilles->findText(m_parent->getUser()->value("localisation/ville", VILLE_DEFAUT).toString()));
	}
	if (m_listePays->currentText() == "France") {
		requete->prepare("SELECT latitude, longitude FROM villes_france WHERE departement = :dept AND nom = :nom");
		requete->bindValue(":dept", m_listeDept->value());
		requete->bindValue(":nom", m_listeVilles->currentText());
		requete->exec();
		requete->next();

		m_latitude->setValue(requete->value(0).toDouble());
		m_longitude->setValue(requete->value(1).toDouble());
	} else {
		requete->prepare("SELECT latitude, longitude FROM villes_monde WHERE pays = :pays AND nom = :nom");
		requete->bindValue(":pays", m_listePays->currentText());
		requete->bindValue(":nom", m_listeVilles->currentText());
		requete->exec();
		requete->next();
		m_latitude->setValue(requete->value(0).toDouble());
		m_longitude->setValue(requete->value(1).toDouble());
	}

	// PAGE TELESCOPE
	m_listeTelescope->setCurrentIndex(m_listeTelescope->findText(m_parent->getUser()->value("telescope/nom", TELESCOPE_DEFAUT).toString()));

	// PAGE OCULAIRES
	QStringList oculaires = m_parent->getUser()->value("oculaires", OCULAIREES_DEFAUT).toString().split("|");
	for (int i(0); i < m_listeOculaires.count(); i++) {
		if (oculaires.indexOf(QString::number(i + 6)) != -1)
			m_listeOculaires.at(i)->setChecked(true);
		else
			m_listeOculaires.at(i)->setChecked(false);
	}

	// PAGE GENERATEUR
	m_labelHauteurMinimum->setText(m_parent->getUser()->value("generateur/hauteurMin", HAUTEUR_MIN_OBJET).toString());
	m_hauteurMinimum->setValue(m_parent->getUser()->value("generateur/hauteurMin", HAUTEUR_MIN_OBJET).toInt());

	m_labelPauseMinimum->setText(m_parent->getUser()->value("generateur/pauseMin", TEMPS_ESPACE).toString());
	m_pauseMinimum->setValue(m_parent->getUser()->value("generateur/pauseMin", TEMPS_ESPACE).toInt());

	m_labelNoteAmasGlobulaire->setText(m_parent->getUser()->value("generateur/amasGlobulaire", NOTE_AMAS_GLOBULAIRE).toString());
	m_noteAmasGlobulaire->setValue(m_parent->getUser()->value("generateur/amasGlobulaire", NOTE_AMAS_GLOBULAIRE).toInt());

	m_labelNoteAmasNebuleuse->setText(m_parent->getUser()->value("generateur/amasNebuleuse", NOTE_AMAS_NEBULEUSE).toString());
	m_noteAmasNebuleuse->setValue(m_parent->getUser()->value("generateur/amasNebuleuse", NOTE_AMAS_NEBULEUSE).toInt());

	m_labelNoteNebuleusePlanetaire->setText(m_parent->getUser()->value("generateur/nebuleusePlanetaire", NOTE_NEBULEUSE_PLANETAIRE).toString());
	m_noteNebuleusePlanetaire->setValue(m_parent->getUser()->value("generateur/nebuleusePlanetaire", NOTE_NEBULEUSE_PLANETAIRE).toInt());

	m_labelNoteNebuleuseReflection->setText(m_parent->getUser()->value("generateur/nebuleuseReflection", NOTE_NEBULEUSE_REFLECTION).toString());
	m_noteNebuleuseReflection->setValue(m_parent->getUser()->value("generateur/nebuleuseReflection", NOTE_NEBULEUSE_REFLECTION).toInt());

	m_labelNoteEtoileDouble->setText(m_parent->getUser()->value("generateur/etoileDouble", NOTE_ETOILE_DOUBLE).toString());
	m_noteEtoileDouble->setValue(m_parent->getUser()->value("generateur/etoileDouble", NOTE_ETOILE_DOUBLE).toInt());

	m_labelNoteEtoileTriple->setText(m_parent->getUser()->value("generateur/etoileTriple", NOTE_ETOILE_TRIPLE).toString());
	m_noteEtoileTriple->setValue(m_parent->getUser()->value("generateur/etoileTriple", NOTE_ETOILE_TRIPLE).toInt());

	m_labelNoteGalaxie->setText(m_parent->getUser()->value("generateur/galaxie", NOTE_GALAXIE).toString());
	m_noteGalaxie->setValue(m_parent->getUser()->value("generateur/galaxie", NOTE_GALAXIE).toInt());

	m_labelNoteAmasOuvert->setText(m_parent->getUser()->value("generateur/amasOuvert", NOTE_AMAS_OUVERT).toString());
	m_noteAmasOuvert->setValue(m_parent->getUser()->value("generateur/amasOuvert", NOTE_AMAS_OUVERT).toInt());

	// PAGE CARTE CIEL
	QString cFond, cLegende, cObjet, cEtoile, cConstellation;
	cFond = m_parent->getUser()->value("couleurCarte/fond", COULEUR_FOND).toString();
	cLegende = m_parent->getUser()->value("couleurCarte/legende", COULEUR_LEGENDE).toString();
	cObjet = m_parent->getUser()->value("couleurCarte/objet", COULEUR_OBJET).toString();
	cConstellation = m_parent->getUser()->value("couleurCarte/constellation", COULEUR_CONSTELLATION).toString();
	cEtoile = m_parent->getUser()->value("couleurCarte/etoile", COULEUR_ETOILE).toString();

	m_labelCouleurFond->setText(cFond.toUpper());
	m_labelCouleurFond->setStyleSheet("color:" + cFond);
	m_labelCouleurLegende->setText(cLegende.toUpper());
	m_labelCouleurLegende->setStyleSheet("color:" + cLegende);
	m_labelCouleurObjet->setText(cObjet.toUpper());
	m_labelCouleurObjet->setStyleSheet("color:" + cObjet);
	m_labelCouleurEtoile->setText(cEtoile.toUpper());
	m_labelCouleurEtoile->setStyleSheet("color:" + cEtoile);
	m_labelCouleurConstellation->setText(cConstellation.toUpper());
	m_labelCouleurConstellation->setStyleSheet("color:" + cConstellation);
}
void FenPreferences::fermer()
{
	initialiserValeur();
	this->close();
}
void FenPreferences::reinitialiser()
{
	// REMETTRE POUR TOUS LES ITEMS LES VALEURS PAR DEFAUT
	QSqlQuery requete;
	m_listeVilles->clear();
	m_listePays->setCurrentIndex(m_listePays->findText(PAYS_DEFAUT));
	if (PAYS_DEFAUT != QStringLiteral("France")) {
		m_listeDept->setDisabled(true);
		requete.prepare("SELECT nom FROM villes_monde WHERE pays = :pays ORDER BY nom");
		requete.bindValue(":pays", PAYS_DEFAUT);
		requete.exec();
		while (requete.next()) {
			m_listeVilles->addItem(requete.value(0).toString());
		}
		m_listeVilles->setCurrentIndex(m_listeVilles->findText(VILLE_DEFAUT));

		requete.prepare("SELECT latitude, longitude FROM villes_monde WHERE pays = :pays AND nom = :nom");
		requete.bindValue(":pays", PAYS_DEFAUT);
		requete.bindValue(":nom", VILLE_DEFAUT);
		requete.exec();
		requete.next();
		m_latitude->setValue(requete.value(0).toDouble());
		m_longitude->setValue(requete.value(1).toDouble());
	} else {
		m_listeDept->setDisabled(false);
		requete.prepare("SELECT departement, latitude, longitude FROM villes_france WHERE nom = :nom");
		requete.bindValue(":nom", VILLE_DEFAUT);
		requete.exec();
		requete.next();
		int dept = requete.value(0).toInt();
		double latitude = requete.value(1).toDouble();
		double longitude = requete.value(2).toDouble();

		requete.prepare("SELECT nom FROM villes_france WHERE departement = :dept ORDER BY nom");
		requete.bindValue(":dept", dept);
		requete.exec();
		while (requete.next()) {
			m_listeVilles->addItem(requete.value(0).toString());
		}
		m_listeVilles->setCurrentIndex(m_listeVilles->findText(VILLE_DEFAUT));
		m_listeDept->setValue(dept);
		m_latitude->setValue(latitude);
		m_longitude->setValue(longitude);
	}

	m_listeTelescope->setCurrentIndex(m_listeTelescope->findText(TELESCOPE_DEFAUT));

	// PAGE OCULAIRES
	QString oculairesParse(OCULAIREES_DEFAUT);
	QStringList oculaires = oculairesParse.split("|");
	for (int i(0); i < m_listeOculaires.count(); i++) {
		if (oculaires.indexOf(QString::number(i + 6)) != -1)
			m_listeOculaires.at(i)->setChecked(true);
		else
			m_listeOculaires.at(i)->setChecked(false);
	}

	// PAGE GENERATEUR
	m_labelHauteurMinimum->setText(QString::number(HAUTEUR_MIN_OBJET));
	m_hauteurMinimum->setValue(HAUTEUR_MIN_OBJET);

	m_labelPauseMinimum->setText(QString::number(TEMPS_ESPACE));
	m_pauseMinimum->setValue(TEMPS_ESPACE);

	m_labelNoteAmasGlobulaire->setText(QString::number(NOTE_AMAS_GLOBULAIRE));
	m_noteAmasGlobulaire->setValue(NOTE_AMAS_GLOBULAIRE);

	m_labelNoteAmasNebuleuse->setText(QString::number(NOTE_AMAS_NEBULEUSE));
	m_noteAmasNebuleuse->setValue(NOTE_AMAS_NEBULEUSE);

	m_labelNoteNebuleusePlanetaire->setText(QString::number(NOTE_NEBULEUSE_PLANETAIRE));
	m_noteNebuleusePlanetaire->setValue(NOTE_NEBULEUSE_PLANETAIRE);

	m_labelNoteNebuleuseReflection->setText(QString::number(NOTE_NEBULEUSE_REFLECTION));
	m_noteNebuleuseReflection->setValue(NOTE_NEBULEUSE_REFLECTION);

	m_labelNoteEtoileDouble->setText(QString::number(NOTE_ETOILE_DOUBLE));
	m_noteEtoileDouble->setValue(NOTE_ETOILE_DOUBLE);

	m_labelNoteEtoileTriple->setText(QString::number(NOTE_ETOILE_TRIPLE));
	m_noteEtoileTriple->setValue(NOTE_ETOILE_TRIPLE);

	m_labelNoteGalaxie->setText(QString::number(NOTE_GALAXIE));
	m_noteGalaxie->setValue(NOTE_GALAXIE);

	m_labelNoteAmasOuvert->setText(QString::number(NOTE_AMAS_OUVERT));
	m_noteAmasOuvert->setValue(NOTE_AMAS_OUVERT);

	// PAGE CARTE CIEL
	QString cFond, cLegende, cObjet, cEtoile, cConstellation;
	cFond = COULEUR_FOND;
	cLegende = COULEUR_LEGENDE;
	cObjet = COULEUR_OBJET;
	cConstellation = COULEUR_CONSTELLATION;
	cEtoile = COULEUR_ETOILE;

	m_labelCouleurFond->setText(cFond.toUpper());
	m_labelCouleurFond->setStyleSheet("color:" + cFond);
	m_labelCouleurLegende->setText(cLegende.toUpper());
	m_labelCouleurLegende->setStyleSheet("color:" + cLegende);
	m_labelCouleurObjet->setText(cObjet.toUpper());
	m_labelCouleurObjet->setStyleSheet("color:" + cObjet);
	m_labelCouleurEtoile->setText(cEtoile.toUpper());
	m_labelCouleurEtoile->setStyleSheet("color:" + cEtoile);
	m_labelCouleurConstellation->setText(cConstellation.toUpper());
	m_labelCouleurConstellation->setStyleSheet("color:" + cConstellation);
}
void FenPreferences::ouvrir(int page)
{
	if (page > 0 && page <= m_listePages.size()) {
		changerOnglet(page - 1);
		exec();
	}
}
