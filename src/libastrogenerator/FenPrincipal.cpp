#include "FenPrincipal.h"
#include "ActionsFenetre.h"
#include "Calculastro.h"
#include "Carteciel.h"
#include "Constantes.h"
#include "Diaporama.h"
#include "FenCreerSoiree.h"
#include "FenInfosCreation.h"
#include "FenPreferences.h"
#include "FenetreBDD.h"
#include "InterfaceCreation.h"
#include "Objet.h"
#include "Soiree.h"

#include <QApplication>
#include <QCloseEvent>
#include <QCoreApplication>
#include <QDesktopServices>
#include <QDir>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QSignalMapper>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QStatusBar>
#include <QToolBar>

FenPrincipal::FenPrincipal()
{
	resize(1024, 768);
	if (!QDir::setCurrent(QCoreApplication::applicationDirPath() + "/")) {
		QMessageBox::critical(this, tr("Résolution des liens"), tr("Problème lors de la résolution des liens, le programme ne peut pas fonctionner correctement."));
		qApp->quit();
	}

	creerActions();
	creerMenu();

	// On ajoute les soirées récentes dans la BDD
	actionSoireesRecentes();

	// On crée la barre d'outils
	QToolBar *toolBar = addToolBar(tr("Toolbar", "Peu important, nom d'un widget"));
	toolBar->addAction(listeActions->getActionCreerSoiree());
	toolBar->addAction(listeActions->getActionFabriquerSoiree());
	toolBar->addAction(listeActions->getActionOuvrirSoiree());
	toolBar->addAction(listeActions->getActionEnregistrer());
	toolBar->addAction(listeActions->getActionImprimer());
	toolBar->addAction(listeActions->getActionFermer());
	toolBar->addAction(listeActions->getActionDiaporama());
	// toolBar->addAction(listeActions->getActionConnexion());

	// On crée le widget central
	tabOnglets = new QTabWidget;
	tabOnglets->setTabsClosable(true);
	tabOnglets->setVisible(false);

	// On crée le widget de la page d'accueil
	widgetAccueil = new QWidget;
	QGridLayout *layoutGrille = new QGridLayout;

	QPushButton *groupeGenererSoiree = new QPushButton;
	groupeGenererSoiree->setFixedSize(LARGEUR_BLOC_PAGE_ACCUEIL, HAUTEUR_BLOC_PAGE_ACCUEIL);
	QLabel *imageGenerer = new QLabel(groupeGenererSoiree);
	imageGenerer->setPixmap(QPixmap(":/icons/generate.png"));
	imageGenerer->setAlignment(Qt::AlignCenter);
	QLabel *texteGenerer = new QLabel(tr("Générer une soirée"), groupeGenererSoiree);
	texteGenerer->setFont(QFont("Verdana", 15));
	texteGenerer->setAlignment(Qt::AlignCenter);
	QLabel *descGenerer = new QLabel(tr("Rentrez plusieurs informations telles que votre latitude, longitude, niveau en astronomie ainsi que l'heure et la date d'observation et notre générateur vous créera automatiquement une liste d'objets à observer pour la soirée."), groupeGenererSoiree);
	descGenerer->setWordWrap(true);
	descGenerer->setAlignment(Qt::AlignJustify);
	QVBoxLayout *layoutGenerer = new QVBoxLayout(groupeGenererSoiree);
	layoutGenerer->addWidget(texteGenerer);
	layoutGenerer->addWidget(imageGenerer);
	layoutGenerer->addWidget(descGenerer);

	QPushButton *groupeCreerSoiree = new QPushButton;
	groupeCreerSoiree->setFixedSize(LARGEUR_BLOC_PAGE_ACCUEIL, HAUTEUR_BLOC_PAGE_ACCUEIL);
	QLabel *imageCreer = new QLabel(groupeCreerSoiree);
	imageCreer->setPixmap(QPixmap(":/icons/creer-soiree.png"));
	imageCreer->setAlignment(Qt::AlignCenter);
	QLabel *texteCreer = new QLabel(tr("Créer une soirée"), groupeCreerSoiree);
	texteCreer->setFont(QFont("Verdana", 15));
	texteCreer->setAlignment(Qt::AlignCenter);
	QLabel *descCreer = new QLabel(tr("Créez facilement votre soirée de toute pièce sans la générer. Les objets qu'il est possible d'observer sont accessibles grâce à une grande base de données. Outil réservé aux astronomes confirmés."), groupeCreerSoiree);
	descCreer->setWordWrap(true);
	descCreer->setAlignment(Qt::AlignJustify);
	QVBoxLayout *layoutCreer = new QVBoxLayout(groupeCreerSoiree);
	layoutCreer->addWidget(texteCreer);
	layoutCreer->addWidget(imageCreer);
	layoutCreer->addWidget(descCreer);

	QPushButton *groupeOuvrirSoiree = new QPushButton;
	groupeOuvrirSoiree->setFixedSize(LARGEUR_BLOC_PAGE_ACCUEIL, HAUTEUR_BLOC_PAGE_ACCUEIL);
	QLabel *imageOuvrir = new QLabel(groupeOuvrirSoiree);
	imageOuvrir->setPixmap(QPixmap(":/icons/ouvrir.png"));
	imageOuvrir->setAlignment(Qt::AlignCenter);
	QLabel *texteOuvrir = new QLabel(tr("Ouvrir une soirée"), groupeOuvrirSoiree);
	texteOuvrir->setFont(QFont("Verdana", 15));
	texteOuvrir->setAlignment(Qt::AlignCenter);
	QLabel *descOuvrir = new QLabel(tr("Ouvrez un fichier de soirée. Les fichiers de soirée sont de type SOA (Soiree Observation Astronomie). Vous pouvez les trouver en générant votre soirée en ligne ou en enregistrant votre soirée."), groupeOuvrirSoiree);
	descOuvrir->setWordWrap(true);
	descOuvrir->setAlignment(Qt::AlignJustify);
	QVBoxLayout *layoutOuvrir = new QVBoxLayout(groupeOuvrirSoiree);
	layoutOuvrir->addWidget(texteOuvrir);
	layoutOuvrir->addWidget(imageOuvrir);
	layoutOuvrir->addWidget(descOuvrir);

	QPushButton *groupePreferenceGenerateur = new QPushButton;
	groupePreferenceGenerateur->setFixedSize(LARGEUR_BLOC_PAGE_ACCUEIL, HAUTEUR_BLOC_PAGE_ACCUEIL);
	QLabel *imagePreferences = new QLabel(groupePreferenceGenerateur);
	imagePreferences->setPixmap(QPixmap(":/icons/preferences.png"));
	imagePreferences->setAlignment(Qt::AlignCenter);
	QLabel *textePreferences = new QLabel(tr("Personnaliser"), groupePreferenceGenerateur);
	textePreferences->setFont(QFont("Verdana", 15));
	textePreferences->setAlignment(Qt::AlignCenter);
	QLabel *descPreferences = new QLabel(tr("Vous pouvez personnaliser le générateur pour que les résultats qu'il vous fournit soient plus proches de vos attentes."), groupePreferenceGenerateur);
	descPreferences->setWordWrap(true);
	descPreferences->setAlignment(Qt::AlignJustify);
	QVBoxLayout *layoutPreferences = new QVBoxLayout(groupePreferenceGenerateur);
	layoutPreferences->addWidget(textePreferences);
	layoutPreferences->addWidget(imagePreferences);
	layoutPreferences->addWidget(descPreferences);

	layoutGrille->addWidget(groupeGenererSoiree, 0, 0);
	layoutGrille->addWidget(groupeCreerSoiree, 0, 1);
	layoutGrille->addWidget(groupeOuvrirSoiree, 1, 0);
	layoutGrille->addWidget(groupePreferenceGenerateur, 1, 1);
	widgetAccueil->setLayout(layoutGrille);

	QWidget *widgetCentral = new QWidget;
	QHBoxLayout *layoutCentral = new QHBoxLayout;
	layoutCentral->addWidget(tabOnglets);
	layoutCentral->addWidget(widgetAccueil);
	widgetCentral->setLayout(layoutCentral);
	setCentralWidget(widgetCentral);

	m_user = new QSettings(NOM_EQUIPE, NOM_PROGRAMME);

	FenCreerSoiree *fenCreation = new FenCreerSoiree(this);
	FenetreBDD *fenBDD = new FenetreBDD(this);
	FenInfosCreation *fenInfos = new FenInfosCreation(this);
	FenPreferences *fenPreferences = new FenPreferences(this);

	// on fait les différentes connexions
	connect(groupeOuvrirSoiree, &QPushButton::clicked, this, [this]() { ouvrirSoa(); });
	connect(groupeGenererSoiree, &QPushButton::clicked, fenCreation, &FenCreerSoiree::exec);
	connect(groupeCreerSoiree, &QPushButton::clicked, fenInfos, &FenInfosCreation::exec);
	connect(groupePreferenceGenerateur, &QPushButton::clicked, fenPreferences, &FenPreferences::exec);

	connect(fenPreferences, &FenPreferences::telescopeChange, fenCreation, &FenCreerSoiree::changerTelescope);
	connect(fenPreferences, &FenPreferences::nouveauTelescope, fenCreation, &FenCreerSoiree::actualiserTelescope);
	connect(fenPreferences, &FenPreferences::villeChange, fenCreation, &FenCreerSoiree::changerVille);

	QSignalMapper *mapper = new QSignalMapper;
	connect(mapper, &QSignalMapper::mappedInt, fenPreferences, &FenPreferences::ouvrir);

	mapper->setMapping(listeActions->getActionPreferencesTelescope(), 2);
	connect(listeActions->getActionPreferencesTelescope(), &QAction::triggered, mapper, qOverload<>(&QSignalMapper::map));
	mapper->setMapping(listeActions->getActionPreferencesOculaires(), 3);
	connect(listeActions->getActionPreferencesOculaires(), &QAction::triggered, mapper, qOverload<>(&QSignalMapper::map));
	mapper->setMapping(listeActions->getActionPreferencesCarteCiel(), 5);
	connect(listeActions->getActionPreferencesCarteCiel(), &QAction::triggered, mapper, qOverload<>(&QSignalMapper::map));
	mapper->setMapping(listeActions->getActionPreferencesLocalisation(), 1);
	connect(listeActions->getActionPreferencesLocalisation(), &QAction::triggered, mapper, qOverload<>(&QSignalMapper::map));
	mapper->setMapping(listeActions->getActionPersonaliserGenerateur(), 4);
	connect(listeActions->getActionPersonaliserGenerateur(), &QAction::triggered, mapper, qOverload<>(&QSignalMapper::map));

	connect(listeActions->getActionCreerSoiree(), &QAction::triggered, fenCreation, &FenCreerSoiree::exec);
	connect(listeActions->getActionFabriquerSoiree(), &QAction::triggered, fenInfos, &FenInfosCreation::exec);
	connect(listeActions->getActionOuvrirSoiree(), &QAction::triggered, this, [this]() { ouvrirSoa(); });
	connect(tabOnglets, &QTabWidget::tabCloseRequested, this, qOverload<int>(&FenPrincipal::fermerOnglet));
	connect(listeActions->getActionFermer(), &QAction::triggered, this, qOverload<>(&FenPrincipal::fermerOnglet));

	connect(listeActions->getActionQuitter(), &QAction::triggered, this, &FenPrincipal::quitterApplication);
	connect(listeActions->getActionObjetsRemarquables(), &QAction::triggered, this, &FenPrincipal::ouvrirObjetsRemarquables);
	connect(listeActions->getActionAide(), &QAction::triggered, this, &FenPrincipal::aide);
	connect(listeActions->getActionAPropos(), &QAction::triggered, this, &FenPrincipal::aPropos);
	connect(listeActions->getActionCDS(), &QAction::triggered, this, &FenPrincipal::ouvrirCDS);
	connect(listeActions->getActionBDD(), &QAction::triggered, fenBDD, &FenetreBDD::exec);

	// IMPORTANT !!!!!!
	connect(tabOnglets, &QTabWidget::currentChanged, this, qOverload<int>(&FenPrincipal::initialiserOngletActif));

	// Informations sur la fenêtre
	setWindowTitle(tr("Générateur de soirées d'observation - astroGenerator"));
	setMinimumHeight(HAUTEUR_MIN_FENETRE);
	setMinimumWidth(LARGEUR_MIN_FENETRE);
	setWindowIcon(QIcon(":/icons/logo-astrogenerator.png"));

	// On grise les actions qu'on ne peut pas faire à l'ouverture du programme
	listeActions->griserActionMonterObjet(true);
	listeActions->griserActionDescendreObjet(true);
	listeActions->griserActionSupprimerObjet(true);
	listeActions->griserActionChangerDureeObjet(true);
	listeActions->griserActionAjouterObjet(true);
	listeActions->griserActionAjouterPlanete(true);
	listeActions->griserActionEnregistrer(true);
	listeActions->griserActionToutFermer(true);
	listeActions->griserActionFermer(true);
	listeActions->griserActionCarteCiel(true);
	listeActions->griserCarteCouleurFond(true);
	listeActions->griserCarteCouleurEtoiles(true);
	listeActions->griserCarteCouleurLegende(true);
	listeActions->griserCarteCouleurConstellation(true);
	listeActions->griserCarteCouleurObjet(true);
	listeActions->griserActionImprimer(true);
	listeActions->griserActionExporterXML(true);
	listeActions->griserActionExporterPDF(true);
	listeActions->griserActionExporterJSON(true);
	listeActions->griserActionDiaporama(true);
	listeActions->griserActionPartager(true);

	// Si on demande à ouvrir un ou plusieurs fichiers à l'ouverture du programme
	QStringList liste = QCoreApplication::arguments();
	if (liste.size() > 1) {
		for (int i(1); i < liste.size(); i++) {
			if (liste.at(i).right(4) == ".soa")
				ouvrirSoa(liste.at(i));
		}
	}
}
void FenPrincipal::creerActions()
{
	listeActions = new ActionsFenetre(this);
}
void FenPrincipal::creerMenu()
{
	// MENU PRINCIPAL DU LOGICIEL
	QMenu *menuFichier = menuBar()->addMenu(tr("&Fichier"));
	menuFichier->addAction(listeActions->getActionCreerSoiree());
	menuFichier->addAction(listeActions->getActionFabriquerSoiree());
	menuFichier->addAction(listeActions->getActionOuvrirSoiree());
	menuFichier->addAction(listeActions->getActionEnregistrer());
	menuFichier->addSeparator();
	menuSoireesRecentes = menuFichier->addMenu(tr("Soirées récentes")); // Est rempli dans la fonction actionSoireesRecentes()
	menuFichier->addSeparator();
	menuFichier->addAction(listeActions->getActionImprimer());
	menuFichier->addSeparator();
	menuFichier->addAction(listeActions->getActionFermer());
	menuFichier->addAction(listeActions->getActionToutFermer());
	menuFichier->addAction(listeActions->getActionQuitter());

	QMenu *menuEdition = menuBar()->addMenu(tr("&Edition"));
	menuEdition->addAction(listeActions->getActionMonterObjet());
	menuEdition->addAction(listeActions->getActionDescendreObjet());
	menuEdition->addAction(listeActions->getActionSupprimerObjet());
	menuEdition->addAction(listeActions->getActionChangerDureeObjet());
	menuEdition->addAction(listeActions->getActionAjouterObjet());
	menuEdition->addAction(listeActions->getActionAjouterPlanete());
	menuEdition->addSeparator();
	QMenu *exporter = menuEdition->addMenu(tr("Exporter la soirée"));
	exporter->addAction(listeActions->getActionExporterXML());
	exporter->addAction(listeActions->getActionExporterPDF());
	// exporter->addAction(listeActions->getActionExporterJSON());
	// menuEdition->addAction(listeActions->getActionPartager());

	QMenu *menuAffichage = menuBar()->addMenu(tr("&Affichage"));
	menuAffichage->addAction(listeActions->getActionDiaporama());

	QMenu *carteCielMenu = menuAffichage->addMenu(tr("Carte du ciel"));
	carteCielMenu->addAction(listeActions->getActionCarteCiel());
	carteCielMenu->addAction(listeActions->getCarteCouleurFond());
	carteCielMenu->addAction(listeActions->getCarteCouleurEtoiles());
	carteCielMenu->addAction(listeActions->getCarteCouleurLegende());
	carteCielMenu->addAction(listeActions->getCarteCouleurConstellation());
	carteCielMenu->addAction(listeActions->getCarteCouleurObjet());

	QMenu *menuPreferences = menuBar()->addMenu(tr("&Préférences"));
	menuPreferences->addAction(listeActions->getActionPreferencesLocalisation());
	menuPreferences->addAction(listeActions->getActionPreferencesTelescope());
	menuPreferences->addAction(listeActions->getActionPreferencesOculaires());
	menuPreferences->addAction(listeActions->getActionPersonaliserGenerateur());
	menuPreferences->addAction(listeActions->getActionPreferencesCarteCiel());

	QMenu *menuObjets = menuBar()->addMenu(tr("&Objets"));
	menuObjets->addAction(listeActions->getActionBDD());
	menuObjets->addAction(listeActions->getActionCDS());
	menuObjets->addAction(listeActions->getActionObjetsRemarquables());

	QMenu *menuAide = menuBar()->addMenu(tr("Aide"));
	menuAide->addAction(listeActions->getActionAPropos());
	menuAide->addAction(listeActions->getActionAide());
}
void FenPrincipal::nouvelOngletSoiree(Soiree &soiree)
{
	QApplication::setOverrideCursor(Qt::WaitCursor); // changer de curseur
	if (!tabOnglets->isVisible()) {
		tabOnglets->setVisible(true);
		widgetAccueil->setVisible(false);
	} // Essayer de remplacer ça par une fonction

	// On crée un objet de type InterfaceLecture
	InterfaceLecture *interface = new InterfaceLecture(&soiree, listeActions, this);

	// On l'ajoute à la liste des onglets et on crée un nouvel onglet
	m_listeInterface.push_back(interface);
	int newIndex = tabOnglets->addTab(interface, tr("Soirée du ", "Cette chaîne est suivie de la date de la soirée") + interface->getSoiree()->getDebut().toString(tr("dd/MM/yyyy", "Format de la date")));
	tabOnglets->setCurrentIndex(newIndex);

	barreStatut->showMessage(tr("Nouvel onglet de soirée ajouté avec succès"), 2000);

	initialiserOngletActif();

	connect(interface, &InterfaceLecture::fermer, this, qOverload<Interface *>(&FenPrincipal::fermerOnglet));
	connect(interface, &InterfaceLecture::afficher, this, [this](const QString &message) { afficherMessage(message); });

	// On dégrise les actions
	listeActions->griserActionMonterObjet(false);
	listeActions->griserActionDescendreObjet(false);
	listeActions->griserActionSupprimerObjet(false);
	listeActions->griserActionChangerDureeObjet(false);
	listeActions->griserActionAjouterObjet(false);
	listeActions->griserActionAjouterPlanete(false);
	listeActions->griserActionEnregistrer(false);
	listeActions->griserActionToutFermer(false);
	listeActions->griserActionFermer(false);
	listeActions->griserActionCarteCiel(false);
	listeActions->griserCarteCouleurFond(false);
	listeActions->griserCarteCouleurEtoiles(false);
	listeActions->griserCarteCouleurLegende(false);
	listeActions->griserCarteCouleurConstellation(false);
	listeActions->griserCarteCouleurObjet(false);
	listeActions->griserActionImprimer(false);
	listeActions->griserActionExporterXML(false);
	listeActions->griserActionExporterPDF(false);
	listeActions->griserActionExporterJSON(false);
	listeActions->griserActionDiaporama(false);
	listeActions->griserActionPartager(false);

	QApplication::restoreOverrideCursor(); // restaurer le curseur
}
bool FenPrincipal::fermerOnglet(int index)
{
	if (tabOnglets->count() > 0 && index >= 0 && index < tabOnglets->count()) {
		bool continuer(true);
		if (m_listeInterface.at(index)->getSoiree()->shouldBeSaved()) // Si la soirée a été modifiée
		{
			QMessageBox msgBox;
			msgBox.setText(tr("La soirée a été modifiée."));
			msgBox.setDetailedText(tr("Soirée modifiée"));
			msgBox.setInformativeText(tr("Voulez-vous sauver les changements ?"));
			msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
			msgBox.setDefaultButton(QMessageBox::Save);
			int ret = msgBox.exec();

			switch (ret) {
			case QMessageBox::Save:
				continuer = m_listeInterface.at(index)->getSoiree()->enregistrerSoiree();
				break;
			case QMessageBox::Discard:
				continuer = true;
				break;
			case QMessageBox::Cancel:
				continuer = false;
				break;
			default:
				continuer = false; // Ne devrait jamais être éxécuté
				break;
			}
		}
		if (continuer) {
			tabOnglets->removeTab(index);
			Interface *interface = m_listeInterface.at(index);
			m_listeInterface.remove(index);
			delete interface;

			barreStatut->showMessage(tr("Onglet fermé avec succès"), 2000);

			initialiserOngletActif();

			if (tabOnglets->count() == 0) // On regarde le nombre d'onglets
			{
				tabOnglets->setVisible(false);
				widgetAccueil->setVisible(true);

				listeActions->griserActionMonterObjet(true);
				listeActions->griserActionDescendreObjet(true);
				listeActions->griserActionSupprimerObjet(true);
				listeActions->griserActionChangerDureeObjet(true);
				listeActions->griserActionAjouterObjet(true);
				listeActions->griserActionAjouterPlanete(true);
				listeActions->griserActionEnregistrer(true);
				listeActions->griserActionToutFermer(true);
				listeActions->griserActionFermer(true);
				listeActions->griserActionCarteCiel(true);
				listeActions->griserCarteCouleurFond(true);
				listeActions->griserCarteCouleurEtoiles(true);
				listeActions->griserCarteCouleurLegende(true);
				listeActions->griserCarteCouleurConstellation(true);
				listeActions->griserCarteCouleurObjet(true);
				listeActions->griserActionImprimer(true);
				listeActions->griserActionExporterXML(true);
				listeActions->griserActionExporterPDF(true);
				listeActions->griserActionExporterJSON(true);
				listeActions->griserActionDiaporama(true);
				listeActions->griserActionPartager(true);
			}
			return true;
		}
	}
	return false;
}
bool FenPrincipal::fermerOnglet()
{
	return fermerOnglet(tabOnglets->currentIndex());
}
bool FenPrincipal::fermerOnglet(Interface *interface)
{
	int index = tabOnglets->indexOf(interface);
	if (index > -1)
		return fermerOnglet(index);

	return false;
}
void FenPrincipal::ouvrirSoa(QString fileSoa)
{
	if (fileSoa == "")
		fileSoa = QFileDialog::getOpenFileName(nullptr, tr("Ouvrir fichier de soirée"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), "Soirée Observation Astronomie (*.soa)");

	if (fileSoa != "") {
		Soiree *soiree = Soiree::soaToSoiree(fileSoa);
		nouvelOngletSoiree(*soiree);
	}
}
bool FenPrincipal::quitterApplication()
{
	bool accept(true);
	if (tabOnglets->count() == 0)
		qApp->quit();
	else {
		for (int i(0); i < tabOnglets->count(); i++) {
			if (!fermerOnglet(i)) // Si on n'a pas fermé l'onglet, on ne peut pas quitter
				accept = false;
		}
	}
	return accept;
}
void FenPrincipal::closeEvent(QCloseEvent *event)
{
	if (quitterApplication())
		event->accept();
	else
		event->ignore();
}

void FenPrincipal::ouvrirObjetsRemarquables()
{
	QDesktopServices::openUrl(QUrl(URL_OBJET_REMARQUABLE));
}
void FenPrincipal::ouvrirCDS()
{
	QDesktopServices::openUrl(QUrl(URL_CDS));
}
void FenPrincipal::nouvelOngletCreation(double latitude, double longitude, QDateTime heureDebut, uint diametre, uint focale)
{
	if (!tabOnglets->isVisible()) {
		tabOnglets->setVisible(true);
		widgetAccueil->setVisible(false);
	}

	InterfaceCreation *interface = new InterfaceCreation(latitude, longitude, heureDebut, diametre, focale, listeActions, this);
	m_listeInterface.push_back(interface);
	int newIndex = tabOnglets->addTab(interface, tr("Créer une soirée", "Titre d'un onglet de création de soirée"));
	tabOnglets->setCurrentIndex(newIndex);

	barreStatut->showMessage(tr("Nouvel onglet de création de soirée ajouté"), 2000);

	initialiserOngletActif();

	connect(interface, &InterfaceCreation::fermer, this, qOverload<Interface *>(&FenPrincipal::fermerOnglet));
	connect(interface, &InterfaceCreation::afficher, this, [this](const QString &message) { afficherMessage(message); });
}
void FenPrincipal::initialiserOngletActif(int index)
{
	if (m_listeInterface.count() > 0 && index > -1) {
		for (int i(0); i < m_listeInterface.count(); i++) {
			m_listeInterface.at(i)->setActive(false);
		}
		m_listeInterface.at(index)->setActive(true);
	}
}
void FenPrincipal::initialiserOngletActif()
{
	if (m_listeInterface.count() > 0 && tabOnglets->currentIndex() > -1) {
		for (int i(0); i < m_listeInterface.count(); i++) {
			m_listeInterface.at(i)->setActive(false);
		}
		m_listeInterface.at(tabOnglets->currentIndex())->setActive(true);
	}
}
QSettings *FenPrincipal::getUser() const
{
	return m_user;
}
void FenPrincipal::afficherMessage(QString message, int duree)
{
	barreStatut->showMessage(message, duree);
}
void FenPrincipal::actionSoireesRecentes()
{ // Initialise au début toutes les soirées de la BDD et les affiche dans le menu avec ajouterSoireeRecente()

	QSqlQuery requete("SELECT fichier, ID FROM soireesRecentes");
	QSqlQuery effacer;
	QString fichier;
	int id(0), i(0);
	while (requete.next()) {
		fichier = requete.value(0).toString();
		id = requete.value(1).toInt();

		if (fichier.right(4) == ".soa" && QFile::exists(fichier)) // SI le fichier existe et est correct
		{
			ajouterSoireeRecente(fichier); // On l'ajoute dans le menu
			i++;
		} else // Sinon on le supprime de la BDD
		{
			effacer.prepare("DELETE FROM soireesRecentes WHERE ID = :id");
			effacer.bindValue(":id", id);
			effacer.exec();
		}
	}
	if (i == 0)
		listeActions->griserActionSoireesRecentes(true);
}
void FenPrincipal::ajouterSoireeRecente(const QString &fichier)
{ // Ajoute une soirée récente en fonction du nom et le connecte au SIGNAL

	QAction *action = new QAction(fichier, this);
	QSignalMapper *mapper = new QSignalMapper;
	connect(mapper, &QSignalMapper::mappedString, this, &FenPrincipal::ouvrirSoa);

	mapper->setMapping(action, fichier);
	connect(action, &QAction::triggered, mapper, qOverload<>(&QSignalMapper::map));

	menuSoireesRecentes->addAction(action);
}
void FenPrincipal::aPropos()
{
	QString about;
	about = "<strong>Version : </strong>" VERSION_STRING "<br />";
	about += "Copyright © 2011-2013 - Univers-Astronomie.fr - Gallouedec Valentin<br />";
	about += "Copyright © 2021-2022 Terrier Benjamin<br />";
	about += tr("<strong>Sources&nbsp;: </strong><a href='%1'>%1</a><br />").arg(URL_GITHUB);
	about += "<strong>Site web : </strong><a href='" URL_UNIVERS_ASTRONOMIE "'>" URL_UNIVERS_ASTRONOMIE "</a><br />";
	about += "<strong>Outil en ligne : </strong><a href='" URL_GENERATEUR "'> " URL_GENERATEUR "</a><br />";
	about += "<strong>Langue ordinateur : </strong>" + QLocale::system().name();
	about += R"(
<p>astroGenerator is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version. </p>

<p>astroGenerator is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.</p>

<p>You should have received a copy of the GNU General Public License
along with astroGenerator.  If not, see <a href="https://www.gnu.org/licenses/">https://www.gnu.org/licenses/</a>.</p>
)";
	QMessageBox::information(this, "A propos d'Astrogenerator", about);
}

void FenPrincipal::aide()
{
	if (!QDesktopServices::openUrl(QUrl::fromLocalFile("doc.pdf"))) {
		QMessageBox::critical(this, tr("Erreur"), tr("Impossible d'ouvrir le fichier aide."));
	}
}
