#include "FenCreerSoiree.h"
#include "Soiree.h"
#include "Constantes.h"
#include "Calculastro.h"

#include <QMessageBox>
#include <QPushButton>
#include <QSqlQuery>

FenCreerSoiree::FenCreerSoiree(FenPrincipal *parent) : QDialog()
{
    m_parent = parent;

    QString villeD,paysD;
    paysD = m_parent->getUser()->value("localisation/pays",PAYS_DEFAUT).toString();
    villeD = m_parent->getUser()->value("localisation/ville",VILLE_DEFAUT).toString();

    m_pays = new QComboBox;
    QStringList pays;
        QSqlQuery *requetePays = new QSqlQuery("SELECT pays FROM villes_monde GROUP BY pays ORDER BY pays");
        while(requetePays->next())
            pays << requetePays->value(0).toString();
        m_pays->addItems(pays);
        m_pays->setCurrentIndex(m_pays->findText(paysD));

    m_departement = new QSpinBox;
        m_departement->setMinimum(1);
        m_departement->setMaximum(95);

    m_villes = new QComboBox;
    m_latitude = new QDoubleSpinBox;
        m_latitude->setMaximum(90.0);
        m_latitude->setMinimum(-90.0);
        m_latitude->setDecimals(3);
    m_longitude = new QDoubleSpinBox;
        m_longitude->setMinimum(-180);
        m_longitude->setMaximum(180);
        m_longitude->setDecimals(3);
    QSqlQuery requeteD;
    if(paysD=="France")
    {
        m_departement->setDisabled(false);
        requeteD.prepare("SELECT departement, latitude, longitude FROM villes_france WHERE nom = :nom");
        requeteD.bindValue(":nom",villeD);
        requeteD.exec();
        requeteD.next();
        int dept = requeteD.value(0).toInt();
        double latitude = requeteD.value(1).toDouble();
        double longitude = requeteD.value(2).toDouble();

        requeteD.prepare("SELECT nom FROM villes_france WHERE departement = :dept ORDER BY nom");
        requeteD.bindValue(":dept",dept);
        requeteD.exec();
        while(requeteD.next())
        {
            m_villes->addItem(requeteD.value(0).toString());
        }
        m_villes->setCurrentIndex(m_villes->findText(villeD));
        m_departement->setValue(dept);
        m_latitude->setValue(latitude);
        m_longitude->setValue(longitude);
    }
    else
    {
        m_departement->setDisabled(true);
        requeteD.prepare("SELECT nom FROM villes_monde WHERE pays = :pays ORDER BY nom");
        requeteD.bindValue(":pays",paysD);
        requeteD.exec();
        while(requeteD.next())
        {
            m_villes->addItem(requeteD.value(0).toString());
        }
        m_villes->setCurrentIndex(m_villes->findText(villeD));

        requeteD.prepare("SELECT latitude, longitude FROM villes_monde WHERE pays = :pays AND nom = :nom");
        requeteD.bindValue(":pays",paysD);
        requeteD.bindValue(":nom",villeD);
        requeteD.exec();
        requeteD.next();
        m_latitude->setValue(requeteD.value(0).toDouble());
        m_longitude->setValue(requeteD.value(1).toDouble());
    }
    m_date = new QDateEdit;
    m_heure = new QTimeEdit;
        m_heure->setDisplayFormat("hh:mm");
    m_duree = new QSpinBox;
    m_dureeObjet = new QSpinBox;
    m_niveau = new QComboBox;
    m_planetes = new QCheckBox(tr("Ajouter les planètes"));
    m_telescope = new QComboBox;
    m_submit = new QPushButton(tr("Générer la soirée"));
    m_close = new QPushButton(tr("Fermer","La fenêtre de génération"));
    m_progress = new QProgressBar;

    m_date->setDate(QDate::currentDate());

    m_heure->setTime(QTime::currentTime());

    m_duree->setMaximum(DUREE_SOIREE_MAX);
    m_duree->setMinimum(DUREE_SOIREE_MIN);
    m_duree->setAccelerated(false);
    m_duree->setSuffix(" heures");

    m_dureeObjet->setMaximum(DUREE_OBJET_MAX);
    m_dureeObjet->setMinimum(DUREE_OBJET_MIN);
    m_dureeObjet->setAccelerated(false);
    m_dureeObjet->setSuffix(tr(" minutes","Laisser l'espace avant"));

    QStringList listNiveau;
    listNiveau << tr("Débutant") << tr("Amateur") << tr("Amateur confirmé") << tr("Expert");
    m_niveau->addItems(listNiveau);
    m_niveau->setCurrentIndex(m_parent->getUser()->value("niveau",0).toInt());

    QStringList listTelescope;
    QSqlQuery requete("SELECT nom FROM telescope ORDER BY nom");
    while(requete.next())
    {
        listTelescope << requete.value(0).toString();
    }
    m_telescope->addItems(listTelescope);
    m_telescope->setCurrentIndex(m_telescope->findText(m_parent->getUser()->value("telescope/nom",TELESCOPE_DEFAUT).toString()));

    // On crée la fenetre pour les constellations
    m_fenetreConstellation = new QDialog;
    m_fenetreConstellation->setWindowTitle(tr("Choisissez une ou plusieurs constellations"));
    QHBoxLayout *layoutConst = new QHBoxLayout;
        QVBoxLayout *layoutConstV1 = new QVBoxLayout;
        QVBoxLayout *layoutConstV2 = new QVBoxLayout;
        QVBoxLayout *layoutConstV3 = new QVBoxLayout;
        QVBoxLayout *layoutConstV4 = new QVBoxLayout;
        QVBoxLayout *layoutConstV5 = new QVBoxLayout;
            layoutConstV5->setAlignment(Qt::AlignTop);
    QCheckBox *checkBox(0);
    QSqlQuery *requeteConst = new QSqlQuery("SELECT constellation FROM ngcic GROUP BY constellation");
    QStringList listeConstellation;
    while(requeteConst->next())
    {
        listeConstellation << Calculastro::abreviationToNom(requeteConst->value(0).toString());
    }
    listeConstellation.sort(); // on trie les items dans l'ordre alphabétique
    for(int m(0); m<listeConstellation.count();m++)
    {
        checkBox = new QCheckBox(listeConstellation.at(m));
        checkBox->setChecked(true);
        m_checkBoxConstellation.push_back(checkBox);
        if(m < 20)
            layoutConstV1->addWidget(checkBox);
        else if(m < 40)
            layoutConstV2->addWidget(checkBox);
        else if(m < 60)
            layoutConstV3->addWidget(checkBox);
        else if(m < 80)
            layoutConstV4->addWidget(checkBox);
        else if(m < 100)
            layoutConstV5->addWidget(checkBox);

        connect(checkBox,SIGNAL(stateChanged(int)),this,SLOT(updateTexteConstellation()));
    }
    layoutConst->addLayout(layoutConstV1);
    layoutConst->addLayout(layoutConstV2);
    layoutConst->addLayout(layoutConstV3);
    layoutConst->addLayout(layoutConstV4);
    layoutConst->addLayout(layoutConstV5);

    QPushButton *boutonConstFermer = new QPushButton(tr("Valider"));
    QPushButton *boutonToutCocher = new QPushButton(tr("Tout cocher"));
    QPushButton *boutonToutDecocher = new QPushButton(tr("Tout décocher"));

    QVBoxLayout *layoutBoutons = new QVBoxLayout;
    layoutBoutons->addWidget(boutonConstFermer);
    layoutBoutons->addWidget(boutonToutCocher);
    layoutBoutons->addWidget(boutonToutDecocher);
    layoutBoutons->setAlignment(Qt::AlignTop);

    layoutConst->addLayout(layoutBoutons);

    m_fenetreConstellation->setLayout(layoutConst);
    QPushButton *boutonConst = new QPushButton(tr("Choisir une constellation"));

    layoutForm = new QFormLayout;

    layoutForm->addRow(tr("&Pays","Laisser le & devant la lettre la plus représentative du mot (pour les raccourcis claviers)"), m_pays);
    layoutForm->addRow(tr("&Département"),m_departement);
    layoutForm->addRow(tr("&Villes"),m_villes);
    layoutForm->addRow(tr("L&atitude"), m_latitude);
    layoutForm->addRow(tr("L&ongitude"),m_longitude);
    layoutForm->addRow(tr("Date d'observation"),m_date);
    layoutForm->addRow(tr("&Heure d'observation"), m_heure);
    layoutForm->addRow(tr("Durée de la &soirée"), m_duree);
    layoutForm->addRow(tr("Durée d'observation par objet"), m_dureeObjet);
    layoutForm->addRow(tr("&Niveau astro'"),m_niveau);
        QHBoxLayout *layoutFormConstellation = new QHBoxLayout;
        layoutFormConstellation->addWidget(boutonConst);
        m_texteConstellation = new QLabel(creerTexteConstellation());
        layoutFormConstellation->addWidget(m_texteConstellation);
    layoutForm->addRow(tr("Constellation"),layoutFormConstellation);
    layoutForm->addRow(tr("Planètes"),m_planetes);
    layoutForm->addRow(tr("Télescopes"), m_telescope);
        QHBoxLayout *layoutBouton = new QHBoxLayout;
        layoutBouton->addWidget(m_submit);
        layoutBouton->addWidget(m_close);
    layoutForm->addRow(layoutBouton);
    layoutForm->addRow(m_progress);

    setLayout(layoutForm);
    setWindowTitle(tr("Générer une soirée","Titre de la fenêtre"));

    connect(m_close,SIGNAL(clicked()),this,SLOT(close()));
    connect(m_submit,SIGNAL(clicked()),this,SLOT(genererSoiree()));
    connect(m_departement,SIGNAL(valueChanged(int)),this,SLOT(actualiserVilles()));
    connect(m_pays,SIGNAL(currentIndexChanged(QString)),this,SLOT(actualiserVilles(QString)));
    connect(m_villes,SIGNAL(currentIndexChanged(QString)),this,SLOT(actualiserCoordonnees(QString)));
    connect(boutonConst,SIGNAL(clicked()),m_fenetreConstellation,SLOT(exec()));
    connect(boutonConstFermer,SIGNAL(clicked()),m_fenetreConstellation,SLOT(close()));
    connect(boutonToutCocher,SIGNAL(clicked()),this,SLOT(toutCocherConstellation()));
    connect(boutonToutDecocher,SIGNAL(clicked()),this,SLOT(toutDecocherConstellation()));
}
void FenCreerSoiree::genererSoiree()
{
    Soiree *soiree = new Soiree;
    QString niveau;

    QDateTime debut(m_date->date(),m_heure->time());
    QDateTime fin(m_date->date(),m_heure->time());
    fin = fin.addSecs(m_duree->value()*3600);

    // --> IL FAUT CONVERTIR EN TU

    debut = debut.toUTC(); // Le convertissement est correct
    fin = fin.toUTC();

    QString countryLocal = Calculastro::enumQtToPays(QLocale::countryToString(QLocale::system().country()));

    if(countryLocal != m_pays->currentText() && countryLocal != "")
        QMessageBox::warning(this,tr("Attention !"),tr("Vous essayez de générer une soirée dans un pays qui n'est pas le vôtre. L'heure correspond à celle de votre ordinateur (")+countryLocal+").");

    /*
    Le problème ici c'est qu'on considère qu'on observe dans le même fuseau que celui où se trouve
    notre machine. En effet on convertit le temps avec toUTC() qui se base sur la localisation du PC
    et non sur la localisation rentrée dans le formulaire.
    Donc on met un avertissement pour prévenir si on ne génère pas dans le même pays.
    */

    switch(m_niveau->currentIndex())
    {
        case 0 : niveau = "debutant"; break;
        case 1 : niveau = "amateur"; break;
        case 2 : niveau = "amateurc"; break;
        case 3 : niveau = "expert"; break;
        default : niveau = "debutant"; break;
    }

    // TELESCOPE
    QSqlQuery *requeteTelescope = new QSqlQuery();
            requeteTelescope->prepare("SELECT diametre, focale FROM telescope WHERE nom = :nom");
    requeteTelescope->bindValue(":nom",m_telescope->currentText());
    if(requeteTelescope->exec())
    {
        requeteTelescope->next();
    }

    // CONSTELLATION
    uint coche(0);
    QString constList;
    for(int i(0); i< m_checkBoxConstellation.count();i++)
    {
        if(m_checkBoxConstellation.at(i)->isChecked())
        {
            constList += Calculastro::abreviationToNom(m_checkBoxConstellation.at(i)->text(),true)+"|";
            coche++;
        }
    }
    if(coche == 88)
        constList = "toutes";
    else if(coche == 0)
    {
        QMessageBox::critical(this,tr("Erreur"),tr("Vous devez choisir au moins une constellation"));
        delete soiree;
        return;
    }
    else
        constList = constList.left(constList.count()-1);

    m_close->setDisabled(true);
    connect(soiree,SIGNAL(generation(int)),m_progress,SLOT(setValue(int)));

    soiree->genererSoiree(m_latitude->value(),m_longitude->value(),debut, fin, m_dureeObjet->value(),constList,niveau, requeteTelescope->value(0).toUInt(), requeteTelescope->value(1).toUInt(), m_parent->getUser(), m_planetes->isChecked());

    soiree->setVille(m_villes->currentText());
    soiree->setPays(m_pays->currentText());

    QMessageBox::information(this,tr("Succès"),tr("La soirée a été générée avec succès !"));
    m_progress->setValue(0);
    m_close->setDisabled(false);

    // On change les informations de base
    m_parent->getUser()->setValue("latitude",m_latitude->value());
    m_parent->getUser()->setValue("longitude",m_longitude->value());
    m_parent->getUser()->setValue("niveau",m_niveau->currentIndex());

    m_parent->nouvelOngletSoiree(*soiree);
    this->close();
}
void FenCreerSoiree::actualiserVilles(QString pays)
{
    if(pays != "France")
    {
        QSqlQuery *requeteVilles = new QSqlQuery;
        requeteVilles->prepare("SELECT nom FROM villes_monde WHERE pays = :pays ORDER BY nom");
        requeteVilles->bindValue(":pays",pays);
        requeteVilles->exec();
        QStringList villes;
        while(requeteVilles->next())
        {
            villes << requeteVilles->value(0).toString();
        }
        m_villes->clear();
        m_villes->addItems(villes);
        m_departement->setDisabled(true);
    }
    else
    {
        m_departement->setDisabled(false);
        QStringList liste;
        QSqlQuery *requeteVilles = new QSqlQuery("SELECT nom FROM villes_france WHERE departement = "+QString::number(m_departement->value())+" ORDER BY nom");
        while(requeteVilles->next())
        {
            liste << requeteVilles->value(0).toString();
        }
        m_villes->clear();
        m_villes->addItems(liste);
    }
}
void FenCreerSoiree::changerVille(QString pays, QString ville, int dept)
{
    QSqlQuery requete;
    // PAYS
    m_pays->setCurrentIndex(m_pays->findText(pays));

    // VILLE
    if(pays == "France")
    {
        requete.prepare("SELECT nom FROM villes_france WHERE departement = :dept ORDER BY nom");
        requete.bindValue(":dept",dept);
        requete.exec();
        m_villes->clear();
        while(requete.next())
            m_villes->addItem(requete.value(0).toString());

        m_departement->setValue(dept);
        m_villes->setCurrentIndex(m_villes->findText(ville));
    }
    else
    {
        requete.prepare("SELECT nom FROM villes_monde WHERE pays = :pays ORDER BY nom");
        requete.bindValue(":pays",pays);
        requete.exec();
        while(requete.next())
            m_villes->addItem(requete.value(0).toString());

        m_villes->setCurrentIndex(m_villes->findText(ville));
    }
}
void FenCreerSoiree::actualiserCoordonnees(const QString &nom)
{
    if(m_pays->currentText() == "France")
    {
        QSqlQuery *requeteCoordonnees = new QSqlQuery("SELECT latitude, longitude FROM villes_france WHERE nom = '"+nom+"' AND departement = "+QString::number(m_departement->value()));
        requeteCoordonnees->next();
        m_latitude->setValue(requeteCoordonnees->value(0).toDouble());
        m_longitude->setValue(requeteCoordonnees->value(1).toDouble());
    }
    else
    {
        QSqlQuery *requeteCoordonnees = new QSqlQuery;
        requeteCoordonnees->prepare("SELECT latitude, longitude FROM villes_monde WHERE nom = :nom AND pays = :pays");
        requeteCoordonnees->bindValue(":nom",nom);
        requeteCoordonnees->bindValue(":pays",m_pays->currentText());
        requeteCoordonnees->exec();
        requeteCoordonnees->next();

        m_latitude->setValue(requeteCoordonnees->value(0).toDouble());
        m_longitude->setValue(requeteCoordonnees->value(1).toDouble());
    }
}
QString FenCreerSoiree::creerTexteConstellation()
{
    QString retour;
    int coche(0);
    QString constellation;
    for(int i(0);i < m_checkBoxConstellation.count();i++)
    {
        if(m_checkBoxConstellation.at(i)->isChecked())
        {
            coche++;
            constellation = m_checkBoxConstellation.at(i)->text();
        }
    }
    if(coche < 88 && coche > 1)
        retour = QString::number(coche)+tr(" constellations selectionnées");
    else if(coche == 88)
        retour = tr("Toutes les constellations");
    else if(coche == 1)
        retour = tr("1 constellation selectionnée")+ " ("+constellation+")";
    else
        retour = tr("Aucune constellation sélectionnée");

    return retour;
}
void FenCreerSoiree::toutCocherConstellation()
{
    for(int i(0);i < m_checkBoxConstellation.count();i++)
        m_checkBoxConstellation.at(i)->setChecked(true);
}
void FenCreerSoiree::toutDecocherConstellation()
{
    for(int i(0);i < m_checkBoxConstellation.count();i++)
        m_checkBoxConstellation.at(i)->setChecked(false);
}
void FenCreerSoiree::updateTexteConstellation()
{
    m_texteConstellation->setText(creerTexteConstellation());
}
void FenCreerSoiree::changerTelescope(const QString &telescope)
{
    int index = m_telescope->findText(telescope);
    if(index != -1)
        m_telescope->setCurrentIndex(index);
}
void FenCreerSoiree::actualiserTelescope()
{
    QSqlQuery requete;
    requete.exec("SELECT nom FROM telescope ORDER BY nom");
    m_telescope->clear();
    while(requete.next())
    {
        m_telescope->addItem(requete.value(0).toString());
    }
}
