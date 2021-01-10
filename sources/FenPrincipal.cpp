#include "FenPrincipal.h"
#include "Objet.h"
#include "Calculastro.h"
#include "Soiree.h"
#include "Carteciel.h"
#include "FenCreerSoiree.h"
#include "Constantes.h"
#include "Diaporama.h"
#include "CompteRebours.h"
#include "WidgetHeure.h"
#include "InterfaceCreation.h"
#include "ActionsFenetre.h"
#include "FenetreBDD.h"
#include "FenInfosCreation.h"
#include "FenPreferences.h"

#include <QCloseEvent>
#include <QMessageBox>
#include <QDesktopServices>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QDir>
#include <QCoreApplication>
#include <QStatusBar>
#include <QToolBar>
#include <QPushButton>
#include <QSignalMapper>
#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <QFileDialog>

FenPrincipal::FenPrincipal()
{
    if(!QDir::setCurrent(QCoreApplication::applicationDirPath()+"/"))
    {
        QMessageBox::critical(this,tr("R�solution des liens"),tr("Probl�me lors de la r�solution des liens, le programme ne peut pas fonctionner correctement."));
        qApp->quit();
    }
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost");
    db.setDatabaseName("dbastrogenerator");
    db.setUserName("univers2");
    db.setPassword("iwxldmkdgpf");
    bool ok = db.open();
    if(!ok)
    {
        QMessageBox::critical(this,tr("Erreur"),tr("Base de donn�es impossible � ouvrir :")+ db.lastError().text());
        qApp->quit();
    }

    creerActions();
    creerMenu();

    // On ajoute les soir�es r�centes dans la BDD
    actionSoireesRecentes();

    // On cr�e le widget pour les acc�s aux mises � jour
    m_view = new QWebView;
        connect(m_view,SIGNAL(loadFinished(bool)),this,SLOT(miseAJour2(bool)));
    m_view2 = new QWebView;
        connect(m_view2,SIGNAL(loadFinished(bool)),this,SLOT(searchNewVersion(bool)));
        m_view2->load(QUrl(FICHIER_VERSION));

    // On cr�e la barre d'outils
        QToolBar *toolBar = addToolBar(tr("Toolbar","Peu important, nom d'un widget"));
    toolBar->addAction(listeActions->getActionCreerSoiree());
    toolBar->addAction(listeActions->getActionFabriquerSoiree());
    toolBar->addAction(listeActions->getActionOuvrirSoiree());
    toolBar->addAction(listeActions->getActionEnregistrer());
    toolBar->addAction(listeActions->getActionImprimer());
    toolBar->addAction(listeActions->getActionFermer());
    toolBar->addAction(listeActions->getActionDiaporama());
    // toolBar->addAction(listeActions->getActionConnexion());

    // On cr�e le widget central
    tabOnglets = new QTabWidget;
    tabOnglets->setTabsClosable(true);
    tabOnglets->setVisible(false);

    // On cr�e le widget de la page d'accueil
    widgetAccueil = new QWidget;
    QGridLayout *layoutGrille = new QGridLayout;

    QPushButton *groupeGenererSoiree = new QPushButton;
        groupeGenererSoiree->setFixedSize(LARGEUR_BLOC_PAGE_ACCUEIL,HAUTEUR_BLOC_PAGE_ACCUEIL);
        QLabel *imageGenerer = new QLabel(groupeGenererSoiree);
        imageGenerer->setPixmap(QPixmap("images/generate.png"));
        imageGenerer->setAlignment(Qt::AlignCenter);
        QLabel *texteGenerer = new QLabel(tr("G�n�rer une soir�e"),groupeGenererSoiree);
            texteGenerer->setFont(QFont("Verdana",15));
            texteGenerer->setAlignment(Qt::AlignCenter);
        QLabel *descGenerer = new QLabel(tr("Rentrez plusieurs informations telles que votre latitude, longitude, niveau en astronomie ainsi que l'heure et la date d'observation et notre g�n�rateur vous cr�era automatiquement une liste d'objets � observer pour la soir�e."),groupeGenererSoiree);
            descGenerer->setWordWrap(true);
            descGenerer->setAlignment(Qt::AlignJustify);
        QVBoxLayout *layoutGenerer = new QVBoxLayout(groupeGenererSoiree);
        layoutGenerer->addWidget(texteGenerer);
        layoutGenerer->addWidget(imageGenerer);
        layoutGenerer->addWidget(descGenerer);

    QPushButton *groupeCreerSoiree = new QPushButton;
        groupeCreerSoiree->setFixedSize(LARGEUR_BLOC_PAGE_ACCUEIL,HAUTEUR_BLOC_PAGE_ACCUEIL);
        QLabel *imageCreer = new QLabel(groupeCreerSoiree);
        imageCreer->setPixmap(QPixmap("images/creer-soiree.png"));
        imageCreer->setAlignment(Qt::AlignCenter);
        QLabel *texteCreer = new QLabel(tr("Cr�er une soir�e"),groupeCreerSoiree);
            texteCreer->setFont(QFont("Verdana",15));
            texteCreer->setAlignment(Qt::AlignCenter);
        QLabel *descCreer = new QLabel(tr("Cr�ez facilement votre soir�e de toute pi�ce sans la g�n�rer. Les objets qu'il est possible d'observer sont accessibles gr�ce � une grande base de donn�es. Outil r�serv� aux astronomes confirm�s."),groupeCreerSoiree);
            descCreer->setWordWrap(true);
            descCreer->setAlignment(Qt::AlignJustify);
        QVBoxLayout *layoutCreer = new QVBoxLayout(groupeCreerSoiree);
        layoutCreer->addWidget(texteCreer);
        layoutCreer->addWidget(imageCreer);
        layoutCreer->addWidget(descCreer);

    QPushButton *groupeOuvrirSoiree = new QPushButton;
        groupeOuvrirSoiree->setFixedSize(LARGEUR_BLOC_PAGE_ACCUEIL,HAUTEUR_BLOC_PAGE_ACCUEIL);
        QLabel *imageOuvrir = new QLabel(groupeOuvrirSoiree);
        imageOuvrir->setPixmap(QPixmap("images/ouvrir.png"));
        imageOuvrir->setAlignment(Qt::AlignCenter);
        QLabel *texteOuvrir = new QLabel(tr("Ouvrir une soir�e"),groupeOuvrirSoiree);
            texteOuvrir->setFont(QFont("Verdana",15));
            texteOuvrir->setAlignment(Qt::AlignCenter);
        QLabel *descOuvrir = new QLabel(tr("Ouvrez un fichier de soir�e. Les fichiers de soir�e sont de type SOA (Soiree Observation Astronomie). Vous pouvez les trouver en g�n�rant votre soir�e en ligne ou en enregistrant votre soir�e."),groupeOuvrirSoiree);
            descOuvrir->setWordWrap(true);
            descOuvrir->setAlignment(Qt::AlignJustify);
        QVBoxLayout *layoutOuvrir = new QVBoxLayout(groupeOuvrirSoiree);
        layoutOuvrir->addWidget(texteOuvrir);
        layoutOuvrir->addWidget(imageOuvrir);
        layoutOuvrir->addWidget(descOuvrir);

    QPushButton *groupePreferenceGenerateur = new QPushButton;
        groupePreferenceGenerateur->setFixedSize(LARGEUR_BLOC_PAGE_ACCUEIL,HAUTEUR_BLOC_PAGE_ACCUEIL);
        QLabel *imagePreferences = new QLabel(groupePreferenceGenerateur);
        imagePreferences->setPixmap(QPixmap("images/preferences.png"));
        imagePreferences->setAlignment(Qt::AlignCenter);
        QLabel *textePreferences = new QLabel(tr("Personnaliser"),groupePreferenceGenerateur);
            textePreferences->setFont(QFont("Verdana",15));
            textePreferences->setAlignment(Qt::AlignCenter);
        QLabel *descPreferences = new QLabel(tr("Vous pouvez personnaliser le g�n�rateur pour que les r�sultats qu'il vous fournit soient plus proches de vos attentes."),groupePreferenceGenerateur);
            descPreferences->setWordWrap(true);
            descPreferences->setAlignment(Qt::AlignJustify);
        QVBoxLayout *layoutPreferences = new QVBoxLayout(groupePreferenceGenerateur);
        layoutPreferences->addWidget(textePreferences);
        layoutPreferences->addWidget(imagePreferences);
        layoutPreferences->addWidget(descPreferences);

    layoutGrille->addWidget(groupeGenererSoiree,0,0);
    layoutGrille->addWidget(groupeCreerSoiree,0,1);
    layoutGrille->addWidget(groupeOuvrirSoiree,1,0);
    layoutGrille->addWidget(groupePreferenceGenerateur,1,1);
    widgetAccueil->setLayout(layoutGrille);

    QWidget *widgetCentral = new QWidget;
        QHBoxLayout *layoutCentral = new QHBoxLayout;
        layoutCentral->addWidget(tabOnglets);
        layoutCentral->addWidget(widgetAccueil);
        widgetCentral->setLayout(layoutCentral);
    setCentralWidget(widgetCentral);

    m_user = new QSettings(NOM_EQUIPE,NOM_PROGRAMME);


    FenCreerSoiree *fenCreation = new FenCreerSoiree(this);
    FenetreBDD *fenBDD = new FenetreBDD(this);
    FenInfosCreation *fenInfos = new FenInfosCreation(this);
    FenPreferences *fenPreferences = new FenPreferences(this);

    // on fait les diff�rentes connexions
    connect(groupeOuvrirSoiree,SIGNAL(clicked()),this,SLOT(ouvrirSoa()));
    connect(groupeGenererSoiree,SIGNAL(clicked()),fenCreation,SLOT(exec()));
    connect(groupeCreerSoiree,SIGNAL(clicked()),fenInfos,SLOT(exec()));
    connect(groupePreferenceGenerateur,SIGNAL(clicked()),fenPreferences,SLOT(exec()));

    connect(fenPreferences,SIGNAL(telescopeChange(QString)),fenCreation,SLOT(changerTelescope(QString)));
    connect(fenPreferences,SIGNAL(nouveauTelescope()),fenCreation,SLOT(actualiserTelescope()));
    connect(fenPreferences,SIGNAL(villeChange(QString,QString,int)),fenCreation,SLOT(changerVille(QString,QString,int)));

    QSignalMapper *mapper = new QSignalMapper;
    connect(mapper,SIGNAL(mapped(int)),fenPreferences,SLOT(ouvrir(int)));

    mapper->setMapping(listeActions->getActionPreferencesTelescope(),2);
    connect(listeActions->getActionPreferencesTelescope(),SIGNAL(triggered()),mapper,SLOT(map()));
    mapper->setMapping(listeActions->getActionPreferencesOculaires(),3);
    connect(listeActions->getActionPreferencesOculaires(),SIGNAL(triggered()),mapper,SLOT(map()));
    mapper->setMapping(listeActions->getActionPreferencesCarteCiel(),5);
    connect(listeActions->getActionPreferencesCarteCiel(),SIGNAL(triggered()),mapper,SLOT(map()));
    mapper->setMapping(listeActions->getActionPreferencesLocalisation(),1);
    connect(listeActions->getActionPreferencesLocalisation(),SIGNAL(triggered()),mapper,SLOT(map()));
    mapper->setMapping(listeActions->getActionPersonaliserGenerateur(),4);
    connect(listeActions->getActionPersonaliserGenerateur(),SIGNAL(triggered()),mapper,SLOT(map()));

    connect(listeActions->getActionCreerSoiree(),SIGNAL(triggered()),fenCreation,SLOT(exec()));
    connect(listeActions->getActionFabriquerSoiree(),SIGNAL(triggered()),fenInfos,SLOT(exec()));
    connect(listeActions->getActionOuvrirSoiree(),SIGNAL(triggered()),this,SLOT(ouvrirSoa()));
    connect(tabOnglets,SIGNAL(tabCloseRequested(int)),this,SLOT(fermerOnglet(int)));   
    connect(listeActions->getActionFermer(),SIGNAL(triggered()),this,SLOT(fermerOnglet()));

    connect(listeActions->getActionQuitter(),SIGNAL(triggered()),this,SLOT(quitterApplication()));
    connect(listeActions->getActionSiteUniversAstronomie(),SIGNAL(triggered()),this,SLOT(ouvrirUniversAstronomie()));
    connect(listeActions->getActionObjetsRemarquables(),SIGNAL(triggered()),this,SLOT(ouvrirObjetsRemarquables()));
    connect(listeActions->getActionAide(),SIGNAL(triggered()),this,SLOT(aide()));
    connect(listeActions->getActionAPropos(),SIGNAL(triggered()),this,SLOT(aPropos()));
    connect(listeActions->getActionCDS(),SIGNAL(triggered()),this,SLOT(ouvrirCDS()));
    connect(listeActions->getActionBDD(),SIGNAL(triggered()),fenBDD,SLOT(exec()));
    connect(listeActions->getActionUpdate(),SIGNAL(triggered()),this,SLOT(miseAJour1()));

    // IMPORTANT !!!!!!
    connect(tabOnglets,SIGNAL(currentChanged(int)),this,SLOT(initialiserOngletActif(int)));

    // Informations sur la fen�tre
    setWindowTitle(tr("G�n�rateur de soir�es d'observation - astroGenerator"));
    setMinimumHeight(HAUTEUR_MIN_FENETRE);
    setMinimumWidth(LARGEUR_MIN_FENETRE);
    setWindowIcon(QIcon("images/logo-astrogenerator.png"));

    // La barre de statut
    barreStatut = statusBar();
    QLabel *label_copyright = new QLabel(tr("Copyright � 2010-2013 <a href=\"http://wwww.univers-astronomie.fr/\">Univers-Astronomie.fr</a>, All rights reserved"));
        label_copyright->setFont(QFont("Verdana",8));
        label_copyright->setOpenExternalLinks(true);
    barreStatut->addPermanentWidget(label_copyright);
    connect(listeActions->getActionBarreStatut(),SIGNAL(toggled(bool)),barreStatut,SLOT(setVisible(bool)));
    listeActions->getActionBarreStatut()->setChecked(false);
    barreStatut->setVisible(false);

    // On grise les actions qu'on ne peut pas faire � l'ouverture du programme
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

    // Si on demande � ouvrir un ou plusieurs fichiers � l'ouverture du programme
    QStringList liste = QCoreApplication::arguments();
    if(liste.size() > 1)
    {
        for(int i(1); i < liste.size();i++)
        {
            if(liste.at(i).right(4) == ".soa")
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
        menuSoireesRecentes = menuFichier->addMenu(tr("Soir�es r�centes")); // Est rempli dans la fonction actionSoireesRecentes()
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
        QMenu *exporter = menuEdition->addMenu(tr("Exporter la soir�e"));
            exporter->addAction(listeActions->getActionExporterXML());
            exporter->addAction(listeActions->getActionExporterPDF());
            //exporter->addAction(listeActions->getActionExporterJSON());
        //menuEdition->addAction(listeActions->getActionPartager());

    QMenu *menuAffichage = menuBar()->addMenu(tr("&Affichage"));
        menuAffichage->addAction(listeActions->getActionDiaporama());
        menuAffichage->addAction(listeActions->getActionBarreStatut());
        QMenu *carteCielMenu = menuAffichage->addMenu(tr("Carte du ciel"));
            carteCielMenu->addAction(listeActions->getActionCarteCiel());
            carteCielMenu->addAction(listeActions->getCarteCouleurFond());
            carteCielMenu->addAction(listeActions->getCarteCouleurEtoiles());
            carteCielMenu->addAction(listeActions->getCarteCouleurLegende());
            carteCielMenu->addAction(listeActions->getCarteCouleurConstellation());
            carteCielMenu->addAction(listeActions->getCarteCouleurObjet());

    QMenu *menuPreferences = menuBar()->addMenu(tr("&Pr�f�rences"));
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
        //menuAide->addAction(listeActions->getActionAProposGenerateur());
        menuAide->addAction(listeActions->getActionUpdate());
        menuAide->addAction(listeActions->getActionAide());
        menuAide->addAction(listeActions->getActionSiteUniversAstronomie());
}
void FenPrincipal::nouvelOngletSoiree(Soiree& soiree)
{
    QApplication::setOverrideCursor( Qt::WaitCursor ); // changer de curseur
    if(!tabOnglets->isVisible())
    {
        tabOnglets->setVisible(true);
        widgetAccueil->setVisible(false);
    } // Essayer de remplacer �a par une fonction

    // On cr�e un objet de type InterfaceLecture
    InterfaceLecture *interface = new InterfaceLecture(&soiree,listeActions,this);

    // On l'ajoute � la liste des onglets et on cr�e un nouvel onglet
    m_listeInterface.push_back(interface);
    int newIndex = tabOnglets->addTab(interface,tr("Soir�e du ","Cette cha�ne est suivie de la date de la soir�e")+interface->getSoiree()->getDebut().toString(tr("dd/MM/yyyy","Format de la date")));
    tabOnglets->setCurrentIndex(newIndex);

    barreStatut->showMessage(tr("Nouvel onglet de soir�e ajout� avec succ�s"),2000);

    initialiserOngletActif();

    connect(interface,SIGNAL(fermer(Interface*)),this,SLOT(fermerOnglet(Interface*)));
    connect(interface,SIGNAL(afficher(QString)),this,SLOT(afficherMessage(QString)));

    // On d�grise les actions
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
    if(tabOnglets->count() > 0 && index >= 0 && index < tabOnglets->count())
    {
        bool continuer(true);
        if(m_listeInterface.at(index)->getSoiree()->shouldBeSaved()) // Si la soir�e a �t� modifi�e
        {
            QMessageBox msgBox;
            msgBox.setText(tr("La soir�e a �t� modifi�e."));
            msgBox.setDetailedText(tr("Soir�e modifi�e"));
            msgBox.setInformativeText(tr("Voulez-vous sauver les changements ?"));
            msgBox.setStandardButtons(QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Save);
            int ret = msgBox.exec();

            switch(ret)
            {
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
                    continuer = false; // Ne devrait jamais �tre �x�cut�
                    break;
            }
        }
        if(continuer)
        {
            tabOnglets->removeTab(index);
            Interface *interface = m_listeInterface.at(index);
            m_listeInterface.remove(index);
            delete interface;

            barreStatut->showMessage(tr("Onglet ferm� avec succ�s"),2000);

            initialiserOngletActif();

            if(tabOnglets->count() == 0) // On regarde le nombre d'onglets
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
    if(index > -1)
        return fermerOnglet(index);
}
void FenPrincipal::ouvrirSoa(QString fileSoa)
{
    if(fileSoa == "")
        fileSoa = QFileDialog::getOpenFileName(0,tr("Ouvrir fichier de soir�e"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),"Soir�e Observation Astronomie (*.soa)");

    if(fileSoa != "")
    {
        Soiree *soiree = Soiree::soaToSoiree(fileSoa);
        nouvelOngletSoiree(*soiree);
    }
}
bool FenPrincipal::quitterApplication()
{
    bool accept(true);
    if(tabOnglets->count() == 0)
        qApp->quit();
    else
    {
        for(int i(0); i < tabOnglets->count(); i++)
        {
            if(!fermerOnglet(i)) // Si on n'a pas ferm� l'onglet, on ne peut pas quitter
                accept = false;
        }
    }
    return accept;
}
void FenPrincipal::closeEvent(QCloseEvent *event)
{
    if(quitterApplication())
        event->accept();
    else
        event->ignore();
}
void FenPrincipal::ouvrirUniversAstronomie()
{
    QDesktopServices::openUrl(QUrl(URL_UNIVERS_ASTRONOMIE));
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
    if(!tabOnglets->isVisible())
    {
        tabOnglets->setVisible(true);
        widgetAccueil->setVisible(false);
    }

    InterfaceCreation *interface = new InterfaceCreation(latitude,longitude,heureDebut,diametre,focale,listeActions, this);
    m_listeInterface.push_back(interface);
    int newIndex = tabOnglets->addTab(interface, tr("Cr�er une soir�e","Titre d'un onglet de cr�ation de soir�e"));
    tabOnglets->setCurrentIndex(newIndex);

    barreStatut->showMessage(tr("Nouvel onglet de cr�ation de soir�e ajout�"),2000);

    initialiserOngletActif();

    connect(interface,SIGNAL(fermer(Interface*)),this,SLOT(fermerOnglet(Interface*)));
    connect(interface,SIGNAL(afficher(QString)),this,SLOT(afficherMessage(QString)));
}
void FenPrincipal::initialiserOngletActif(int index)
{
    if(m_listeInterface.count() > 0 && index > -1)
    {
        for(int i(0);i < m_listeInterface.count();i++)
        {
            m_listeInterface.at(i)->setActive(false);
        }
        m_listeInterface.at(index)->setActive(true);
    }
}
void FenPrincipal::initialiserOngletActif()
{
    if(m_listeInterface.count() > 0 && tabOnglets->currentIndex() > -1)
    {
        for(int i(0);i < m_listeInterface.count();i++)
        {
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
    barreStatut->showMessage(message,duree);
}
void FenPrincipal::actionSoireesRecentes()
{ // Initialise au d�but toutes les soir�es de la BDD et les affiche dans le menu avec ajouterSoireeRecente()

    QSqlQuery requete("SELECT fichier, ID FROM soireesRecentes");
    QSqlQuery effacer;
    QString fichier;
    int id(0), i(0);
    while(requete.next())
    {
        fichier = requete.value(0).toString();
        id = requete.value(1).toInt();

        if(fichier.right(4) == ".soa" && QFile::exists(fichier)) // SI le fichier existe et est correct
        {
            ajouterSoireeRecente(fichier); // On l'ajoute dans le menu
            i++;
        }
        else // Sinon on le supprime de la BDD
        {
            effacer.prepare("DELETE FROM soireesRecentes WHERE ID = :id");
            effacer.bindValue(":id",id);
            effacer.exec();
        }
    }
    if(i==0) listeActions->griserActionSoireesRecentes(true);
}
void FenPrincipal::ajouterSoireeRecente(const QString &fichier)
{ // Ajoute une soir�e r�cente en fonction du nom et le connecte au SIGNAL

    QAction *action = new QAction(fichier,this);
    QSignalMapper *mapper = new QSignalMapper;
    connect(mapper,SIGNAL(mapped(QString)),this,SLOT(ouvrirSoa(QString)));

    mapper->setMapping(action,fichier);
    connect(action,SIGNAL(triggered()),mapper,SLOT(map()));

    menuSoireesRecentes->addAction(action);
}
void FenPrincipal::aPropos()
{
    QString about;
    about = "<strong>Version : </strong>"VERSION"<br />";
    about += "Copyright � 2011-2013 - Univers-Astronomie.fr - Gallouedec Valentin<br />";
    about += "<strong>Site web : </strong><a href='"URL_UNIVERS_ASTRONOMIE"'>"URL_UNIVERS_ASTRONOMIE"</a><br />";
    about += "<strong> Outil en ligne : </strong><a href='"URL_GENERATEUR"'>"URL_GENERATEUR"</a><br />";
    about += "<strong> Langue ordinateur : </strong>"+QLocale::system().name()+"<br />";
    about += "<br />This work is licensed under the Creative Commons Attribution - Pas d'utilisation Commerciale 3.0 non transpos� License. To view a copy of this license, visit <a href=\"http://creativecommons.org/licenses/by-nc/3.0/\">http://creativecommons.org/licenses/by-nc/3.0/</a>.";
    QMessageBox::information(this,"A propos d'Astrogenerator",about);
}
void FenPrincipal::aide()
{
    if(!QDesktopServices::openUrl(QUrl::fromLocalFile("doc.pdf")))
    {
        QMessageBox::critical(this, tr("Erreur"), tr("Impossible d'ouvrir le fichier aide."));
    }
}
void FenPrincipal::miseAJour1()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_view->load(QUrl(FICHIER_VERSION));
}
void FenPrincipal::miseAJour2(bool ok)
{
    if(ok)
    {
        QApplication::restoreOverrideCursor(); // On remet le curseur normal

        QString versionNew = m_view->page()->currentFrame()->toPlainText(); // On r�cup�re le contenu de version.vs
        if(existsNewVersion(versionNew))
        {
            int reponse  = QMessageBox::question(this,tr("Nouvelle version"),tr("Une nouvelle version est disponible. Voulez-vous la t�l�charger ?"),QMessageBox::Yes|QMessageBox::No);
            if(reponse == QMessageBox::Yes) // Si on veut t�l�charger la mise � jour
            {
                QDesktopServices::openUrl(QUrl(PAGE_TELECHARGEMENT"?name=astroGenerator-" + versionNew + "-setup.exe&version="+versionNew+"&by=astroGenerator"));
            }
        }
        else
        {
            QMessageBox::information(this,tr("Logiciel � jour"),tr("Votre logiciel est � jour ("VERSION")."));
        }
    }
    else
        QMessageBox::warning(this,tr("R�cup�ration impossible"),tr("Impossible de r�cup�rer les informations de mise � jour. V�rifiez votre connexion internet ou r�esayer plus tard."));
}
bool FenPrincipal::existsNewVersion(QString ligne)
{
    QStringList versionNewChiffreListe = ligne.split(".");
    QStringList versionChiffreListe = QString(VERSION).split(".");

    // On fait en sorte qu'ils aient le m�me nombre de chiffre
    if(versionNewChiffreListe.count() < versionChiffreListe.count())
    {
        int ecart(versionChiffreListe.count()-versionNewChiffreListe.count());
        for(int i(0);i<ecart;i++)
            versionNewChiffreListe.push_back("0");
    }
    else if(versionNewChiffreListe.count() > versionChiffreListe.count())
    {
        int ecart(versionNewChiffreListe.count()-versionChiffreListe.count());
        for(int i(0);i<ecart;i++)
            versionChiffreListe.push_back("0");
    }
    // On traite
    for(int j(0);j<versionChiffreListe.count();j++)
    {
        if(versionChiffreListe.at(j).toInt() < versionNewChiffreListe.at(j).toInt())
            return true;
    }
    return false;
}
void FenPrincipal::searchNewVersion(bool ok)
{
    if(ok)
    {
        QString versionNew = m_view2->page()->currentFrame()->toPlainText(); // On r�cup�re le contenu de version.vs
        if(existsNewVersion(versionNew))
        {
            int reponse  = QMessageBox::question(this,tr("Nouvelle version"),tr("Une nouvelle version est disponible. Voulez-vous la t�l�charger ?"),QMessageBox::Yes|QMessageBox::No);
            if(reponse == QMessageBox::Yes) // Si on veut t�l�charger la mise � jour
            {
                QDesktopServices::openUrl(QUrl(PAGE_TELECHARGEMENT"?name=astroGenerator-" + versionNew + "-setup.exe&version="+versionNew+"&by=astroGenerator"));
            }
        }
    }
}
