#include "InterfaceCreation.h"
#include "ObjetObs.h"
#include "Constantes.h"
#include "Calculastro.h"
#include "ObjetPlaneteObs.h"
#include "ObjetCPObs.h"
#include "Soiree.h"

InterfaceCreation::InterfaceCreation(double latitude, double longitude, QDateTime heureDebut, uint diametre, uint focale, ActionsFenetre *listeAction, QWidget *parent) :
    Interface(listeAction, parent)
{
    // ETAPE 1 : On initialise toutes les variables

    m_modeleRecherche = new QStandardItemModel;
        m_modeleRecherche = rechercheToModele("",true); // On met a true car par défaut on affiche que les interet = 4
    m_soiree = new Soiree;
    champDate = new QDateTimeEdit;
        champDate->setDisplayFormat(tr("dd/MM/yyyy hh:mm","Format de la date et l'heure"));
    champDureeObjet = new QSpinBox;
        champDureeObjet->setMinimum(DUREE_OBJET_MIN);
        champDureeObjet->setMaximum(DUREE_OBJET_MAX);
        champDureeObjet->setSuffix(tr(" min","Unité (minutes), laisser l'espace avant"));

    m_modele = new QStandardItemModel;
    m_vue = new QTableView;
    m_vuePlanete = new QListView;
        m_vuePlanete->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_modelePlanete = new QStandardItemModel;

    ajouterObjet = new QAction(tr("Ajouter l'objet"),this);
        ajouterObjet->setIcon(QIcon("images/arrow-right.png"));
    enleverObjet = new QAction(tr("Enlever l'objet"),this);
        enleverObjet->setIcon(QIcon("images/arrow-left.png"));

    m_vueRecherche = new QListView;
        m_vueRecherche->setModel(m_modeleRecherche);
        m_vueRecherche->setMaximumWidth(LARGEUR_LISTE_OBJET_FENETRE_CREATION);
        m_vueRecherche->setEditTriggers(QAbstractItemView::NoEditTriggers);

        connect(m_vueRecherche,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(getDate()));

    // ON CREE LA PAGE POUR L'AJOUT D'OBJET


    // ETAPE 2 : On crée la page avec les layout

    QHBoxLayout *layoutPrincipal = new QHBoxLayout;

    QLineEdit *barre_recherche = new QLineEdit;
        barre_recherche->setMaximumWidth(LARGEUR_LISTE_OBJET_FENETRE_CREATION);
        barre_recherche->setPlaceholderText(tr("Rechercher un objet"));

    QVBoxLayout *layoutGauche = new QVBoxLayout;
        layoutGauche->addWidget(barre_recherche);
        layoutGauche->addWidget(m_vueRecherche);

    QToolBar *barActions = new QToolBar;
        barActions->addAction(ajouterObjet);
        barActions->addAction(enleverObjet);
        barActions->addAction(m_listeActions->getActionMonterObjet());
        barActions->addAction(m_listeActions->getActionDescendreObjet());
        barActions->addAction(m_listeActions->getActionChangerDureeObjet());
        barActions->addAction(m_listeActions->getActionAjouterPlanete());
        barActions->addAction(m_listeActions->getActionInfoSoiree());
        barActions->setOrientation(Qt::Vertical);

    m_vue->resizeColumnsToContents(); // Adapte la largeur des colonnes en fonction de leur contenu
    m_vue->setCornerButtonEnabled(false);
    m_vue->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_vue->setSelectionMode(QAbstractItemView::SingleSelection);
    m_vue->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layoutPrincipal->addLayout(layoutGauche);
    layoutPrincipal->addWidget(barActions);
    layoutPrincipal->addWidget(m_vue);

    setLayout(layoutPrincipal);

    // ETAPE 3 : On crée toutes les connexions

    connect(ajouterObjet,SIGNAL(triggered()),this,SLOT(getDate()));
    connect(enleverObjet,SIGNAL(triggered()),this,SLOT(enleveObjet()));
    connect(m_listeActions->getActionMonterObjet(),SIGNAL(triggered()),this,SLOT(monterObjet()));
    connect(m_listeActions->getActionDescendreObjet(),SIGNAL(triggered()),this,SLOT(descendreObjet()));
    connect(m_listeActions->getActionChangerDureeObjet(),SIGNAL(triggered(bool)),this,SLOT(modifierObjet()));
    connect(m_listeActions->getActionEnregistrer(),SIGNAL(triggered()),this,SLOT(enregistrerSoiree()));
    connect(m_listeActions->getActionAjouterPlanete(),SIGNAL(triggered()),this,SLOT(fenetrePlanete()));
    connect(m_listeActions->getActionInfoSoiree(),SIGNAL(triggered()),this,SLOT(infosSoiree()));
    connect(m_vue,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(afficherInfosObjet(QModelIndex)));
    connect(m_vue,SIGNAL(clicked(QModelIndex)),this,SLOT(griserActions()));
    connect(m_vueRecherche,SIGNAL(clicked(QModelIndex)),this,SLOT(griserActions()));
    connect(barre_recherche,SIGNAL(textChanged(QString)),this,SLOT(nouvelleRecherche(QString)));

    // ETAPE 4 : On remplit les infos

    QVector<QString> villePays(Calculastro::trouverVillePays(latitude,longitude)); // on trouve la ville et le pays en fonction de la latitude
        m_soiree->setDebut(heureDebut);
        m_soiree->setFin(heureDebut.addSecs(DUREE_SOIREE_MAX*3600));
        m_soiree->setLat(latitude);
        m_soiree->setLongi(longitude);
        m_soiree->setDiametre(diametre);
        m_soiree->setFocale(focale);
        m_soiree->setBoolPlanete(true);
        m_soiree->setPays(villePays.at(1));
        m_soiree->setVille(villePays.at(0));

    champDate->setDate(heureDebut.date());
    champDate->setTime(heureDebut.time());
}
void InterfaceCreation::nouvelleRecherche(QString texte)
{
    if(m_active)
    {
        m_vueRecherche->setModel(rechercheToModele(texte));
        griserActions();
    }
}
QStandardItemModel* InterfaceCreation::rechercheToModele(QString recherche, bool ok)
{
    if(m_active)
    {
        m_modeleRecherche = new QStandardItemModel;

        QString reqplus;
        if(ok || recherche == "")
            reqplus = " interet = 4 AND";

        QSqlQuery *requete = new QSqlQuery("SELECT nom, reference, messier FROM ngcic WHERE"+reqplus+" (nom LIKE '%"+recherche+"%' OR reference LIKE '%"+recherche+"%' OR messier LIKE '%"+recherche+"%')");
        QString nom;

        while(requete->next())
        {
            if(requete->value(0).toString() != "")
            {
                nom = requete->value(0).toString() + " (";
                if(requete->value(2).toString() != "0")
                    nom += requete->value(2).toString()+", "+requete->value(1).toString()+")";
                else
                    nom += requete->value(1).toString()+")";
            }
            else if(requete->value(2).toString() != "0")
                nom = requete->value(2).toString()+" ("+requete->value(1).toString()+")";
            else
                nom = requete->value(1).toString();

            QStandardItem *lignePrincipale = new QStandardItem(nom);
            m_modeleRecherche->appendRow(lignePrincipale);
            lignePrincipale->appendRow(new QStandardItem(requete->value(1).toString()));
        }
    }
    return m_modeleRecherche;
}
void InterfaceCreation::ajoutObjet()
{
    if(m_active)
    {
        if(m_vueRecherche->currentIndex().isValid())
        {
            QItemSelectionModel *selection = m_vueRecherche->selectionModel();
            QModelIndex indexElementSelectionne = selection->currentIndex().child(0,0);
            QVariant elementSelectionne = m_modeleRecherche->data(indexElementSelectionne, Qt::DisplayRole);

            QString ref = elementSelectionne.toString();

            // verifier si la date est valide
            if(champDate->dateTime().isValid())
            {
                // verfier si l'objet n'est pas déjà dans la soirée
                if(m_soiree->indexFromRef(ref) == -1)
                {
                    if(!tempsEstPris(champDate->dateTime(),champDureeObjet->value()))
                    {
                        ObjetCPObs *objet = new ObjetCPObs(ref,champDate->dateTime(),champDate->dateTime().addSecs(champDureeObjet->value()*60));
                        if(objet->isValid())
                        {
                            m_soiree->ajouterObjet(objet);
                            emit afficher(tr("L'objet a été ajouté"));
                            raffraichirListeObjet();
                        }
                    }
                    else
                        QMessageBox::critical(this,tr("Objet implaçable"),tr("Vous ne pouvez pas placer un objet à cet endroit, il y en a déjà un."));
                }
                else
                    QMessageBox::critical(this,tr("Objet présent"),tr("L'objet demandé est déjà présent dans la soirée."));
            }
            else
              QMessageBox::critical(this,tr("Date invalide"),tr("La date que vous demandez est invalide. Veuillez réesayer."));
        }
    }
}
void InterfaceCreation::getDate()
{
    if(m_active)
    {
        if(m_vueRecherche->currentIndex().isValid())
        {
            QDialog *fenetreDemandeDate = new QDialog;
                QPushButton *boutonValider = new QPushButton(tr("Valider"));
                QPushButton *boutonAnnuler = new QPushButton(tr("Annuler"));
                QFormLayout *layoutGrille = new QFormLayout;
                    layoutGrille->addRow(tr("Date"),champDate);
                    layoutGrille->addRow(tr("Durée d'observation"),champDureeObjet);
                    QHBoxLayout *layoutBoutons = new QHBoxLayout;
                    layoutBoutons->addWidget(boutonValider);
                    layoutBoutons->addWidget(boutonAnnuler);
                    layoutGrille->addRow(layoutBoutons);
                fenetreDemandeDate->setLayout(layoutGrille);

                //  Les deux boutons ferment la fenêtre
                connect(boutonAnnuler,SIGNAL(clicked()),fenetreDemandeDate,SLOT(close()));
                connect(boutonValider,SIGNAL(clicked()),fenetreDemandeDate,SLOT(close()));
                // Si on valide, alors on apelle le slot d'ajout
                connect(boutonValider,SIGNAL(clicked()),this,SLOT(ajoutObjet()));

                fenetreDemandeDate->exec();
        }
     }
}
void InterfaceCreation::raffraichirListeObjet()
{
    if(m_active)
    {
        QSettings *user = new QSettings(NOM_EQUIPE,NOM_PROGRAMME);
        m_modele = new QStandardItemModel;
        QStandardItem *reference(0), *reference2(0), *reference3(0), *reference4(0), *reference5(0), *reference6(0), *reference9(0);
        for(int i(0);i < m_soiree->getPlanning().count(); i++)
        {
            reference = new QStandardItem(m_soiree->getPlanning().at(i)->nomComplet());
            reference2 = new QStandardItem(m_soiree->getPlanning().at(i)->ref());
            reference3 = new QStandardItem(m_soiree->getPlanning().at(i)->ascdr());
            reference4 = new QStandardItem(m_soiree->getPlanning().at(i)->declinaison());
            reference5 = new QStandardItem(QString::number(m_soiree->getPlanning().at(i)->magnitude()));
            reference6 = new QStandardItem(Calculastro::abreviationToNom(m_soiree->getPlanning().at(i)->constellation()));
            reference9 = new QStandardItem(m_soiree->getPlanning().at(i)->getDebut().time().toString("hh:mm") + " à " + m_soiree->getPlanning().at(i)->getFin().time().toString("hh:mm"));

            m_modele->setItem(i,0,reference);
            m_modele->setItem(i,1,reference2);
            m_modele->setItem(i,2,reference3);
            m_modele->setItem(i,3,reference4);
            m_modele->setItem(i,4,reference5);
            m_modele->setItem(i,5,reference6);
            m_modele->setItem(i,6,reference9);

            if(i+1 == m_soiree->getPlanning().count()) // On met le champ de date à jour avec l'objet le plus loin dans la soirée
                champDate->setDateTime(m_soiree->getPlanning().at(i)->getFin().addSecs(user->value("generateur/pauseMin",TEMPS_ESPACE).toInt()*60));
        }
        m_modele->setHeaderData(0, Qt::Horizontal, tr("Nom"));
        m_modele->setHeaderData(1, Qt::Horizontal, tr("Référence"));
        m_modele->setHeaderData(2, Qt::Horizontal, tr("Ascension droite"));
        m_modele->setHeaderData(3, Qt::Horizontal, tr("Déclinaison"));
        m_modele->setHeaderData(4, Qt::Horizontal, tr("Magnitude"));
        m_modele->setHeaderData(5, Qt::Horizontal, tr("Constellation"));
        m_modele->setHeaderData(6, Qt::Horizontal, tr("Heure d'observation"));

        m_vue->setModel(m_modele);
        m_vue->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
        griserActions();
     }
}
void InterfaceCreation::enleveObjet()
{
    if(m_active)
    {
        if(m_vue->currentIndex().isValid())
        {
            int reponse = QMessageBox::question(0, tr("Confirmer"), tr("Voulez-vous vraiment enlever l'objet de la soirée ?"), QMessageBox ::Yes | QMessageBox::No);
            if(reponse == QMessageBox::Yes)
            {
                int index = m_vue->currentIndex().row();
                m_soiree->supprimerObjet(index);
                emit afficher(tr("L'objet a été enlevé"));
                raffraichirListeObjet();
            }
        }
    }
}
bool InterfaceCreation::tempsEstPris(QDateTime temps, int duree) const
{
    for(int i(0);i<m_soiree->getPlanning().count();i++)
    {
        if(temps >= m_soiree->getPlanning().at(i)->getDebut() && temps <= m_soiree->getPlanning().at(i)->getFin())
            return true;
        else if(temps.addSecs(duree*60) >= m_soiree->getPlanning().at(i)->getDebut() && temps.addSecs(duree*60) <= m_soiree->getPlanning().at(i)->getFin())
            return true;
    }
    return false;

    // --> existe déjà en tant que Calculastro::verifDisponibilite()
}
void InterfaceCreation::monterObjet()
{
    if(m_active)
    {
        if(m_vue->currentIndex().isValid())
        {
            int reponse = QMessageBox::question(0, tr("Confirmer"), tr("Voulez-vous vraiment monter l'objet ?"), QMessageBox::Yes|QMessageBox::No);
            if(reponse == QMessageBox::Yes)
            {
                int ligne = m_vue->currentIndex().row();
                QString ref = m_modele->item(ligne,1)->text();

                int index = m_soiree->indexFromRef(ref);

                if(index >= 0)
                {
                    m_soiree->monterObjet(index);
                    raffraichirListeObjet();
                    emit afficher(tr("L'objet a été monté"));
                }
            }
        }
    }
}
void InterfaceCreation::descendreObjet()
{
    if(m_active)
    {
        if(m_vue->currentIndex().isValid())
        {
            int reponse = QMessageBox::question(0, tr("Confirmer"), tr("Voulez-vous vraiment descendre l'objet ?"), QMessageBox::Yes|QMessageBox::No);
            if(reponse == QMessageBox::Yes)
            {
                int ligne = m_vue->currentIndex().row();
                QString ref = m_modele->item(ligne,1)->text();

                int index = m_soiree->indexFromRef(ref);

                if(index >= 0)
                {
                    m_soiree->descendreObjet(index);
                    raffraichirListeObjet();
                    emit afficher(tr("L'objet a été descendu"));
                }
            }
        }
    }
}
void InterfaceCreation::fenetrePlanete()
{
    if(m_active)
    {
        QDialog *fenetreAjouterPlanete = new QDialog(this);
        fenetreAjouterPlanete->setWindowTitle(tr("Ajouter une planète"));
        QPushButton *boutonOkPlanete = new QPushButton(tr("Ajouter"));
        QPushButton *boutonKoPlanete = new QPushButton(tr("Annuler"));

            QStandardItem *p1 = new QStandardItem(tr("Mercure"));
                p1->appendRow(new QStandardItem("P1"));
            QStandardItem *p2 = new QStandardItem(tr("Vénus"));
                p2->appendRow(new QStandardItem("P2"));
            QStandardItem *p3 = new QStandardItem(tr("Mars"));
                p3->appendRow(new QStandardItem("P3"));
            QStandardItem *p4 = new QStandardItem(tr("Jupiter"));
                p4->appendRow(new QStandardItem("P4"));
            QStandardItem *p5 = new QStandardItem(tr("Saturne"));
                p5->appendRow(new QStandardItem("P5"));
            QStandardItem *p6 = new QStandardItem(tr("Uranus"));
                p6->appendRow(new QStandardItem("P6"));
            QStandardItem *p7 = new QStandardItem(tr("Neptune"));
                p7->appendRow(new QStandardItem("P7"));
            m_modelePlanete->clear();
            m_modelePlanete->appendRow(p1);
            m_modelePlanete->appendRow(p2);
            m_modelePlanete->appendRow(p3);
            m_modelePlanete->appendRow(p4);
            m_modelePlanete->appendRow(p5);
            m_modelePlanete->appendRow(p6);
            m_modelePlanete->appendRow(p7);

        m_vuePlanete->setModel(m_modelePlanete);

        QFormLayout *layout = new QFormLayout;
            layout->addRow(m_vuePlanete);
            layout->addRow(tr("Heure d'observation"),champDate);
            layout->addRow(tr("Durée d'observation"), champDureeObjet);
            QHBoxLayout *layoutH = new QHBoxLayout;
                layoutH->addWidget(boutonOkPlanete);
                layoutH->addWidget(boutonKoPlanete);
            layout->addRow(layoutH);
        fenetreAjouterPlanete->setLayout(layout);

        connect(boutonKoPlanete,SIGNAL(clicked()),fenetreAjouterPlanete,SLOT(close()));
        connect(boutonOkPlanete,SIGNAL(clicked()),fenetreAjouterPlanete,SLOT(close()));
        connect(boutonOkPlanete,SIGNAL(clicked()),this,SLOT(ajouterPlanete()));

        fenetreAjouterPlanete->exec();
    }
}
void InterfaceCreation::ajouterPlanete()
{
    if(m_active)
    {
        if(m_vuePlanete->currentIndex().isValid())
        {
            if(champDate->dateTime().isValid())
            {
                QItemSelectionModel *selection = m_vuePlanete->selectionModel();
                QModelIndex indexElementSelectionne = selection->currentIndex().child(0,0);
                QVariant elementSelectionne = m_modelePlanete->data(indexElementSelectionne, Qt::DisplayRole);

                QString ref(elementSelectionne.toString());

                if(m_soiree->indexFromRef(ref) == -1)
                {
                    if(!tempsEstPris(champDate->dateTime(),champDureeObjet->value()))
                    {
                        ObjetPlaneteObs *planete = new ObjetPlaneteObs(ref,champDate->dateTime(),champDate->dateTime().addSecs(champDureeObjet->value()*60));
                        m_soiree->ajouterObjet(planete);
                        emit afficher(tr("La planète a bien été ajoutée"));
                        raffraichirListeObjet();
                    }
                    else
                        QMessageBox::critical(this,tr("Ajout impossible"),tr("Ajout de l'objet impossible à cet endroit."));
                }
                else
                    QMessageBox::critical(this,tr("Objet présent"),tr("L'objet demandé est déjà présent dans la soirée."));
            }
            else
                QMessageBox::critical(this,tr("Date invalide"),tr("La date rentrée est invalide."));
        }
        else
            QMessageBox::critical(this,tr("Aucune planète"),tr("Aucune planète sélectionnée."));
    }
}
void InterfaceCreation::modifierObjet()
{
    if(m_active)
    {
        if(m_vue->currentIndex().isValid())
        {
            bool* ok = new bool;
            int reponse = QInputDialog::getInteger(0,tr("Modifier la durée"),tr("Quelle est la nouvelle durée ?"),5,DUREE_OBJET_MIN,DUREE_OBJET_MAX,1,ok);
            if(*ok == true)
            {
                int ligne = m_vue->currentIndex().row();
                QString ref = m_modele->item(ligne,1)->text();
                int index = m_soiree->indexFromRef(ref);

                if(index > -1)
                {
                   m_soiree->modifierDuree(index,reponse);
                   raffraichirListeObjet();
                   emit afficher(tr("L'objet a bien été modifié"));
                }
            }
         }
    }
}
void InterfaceCreation::afficherInfosObjet(QModelIndex cells)
{
    if(m_active)
    {
        QString ref = m_modele->item(cells.row(),1)->text();
        int index = m_soiree->indexFromRef(ref);

        ObjetObs *objet = m_soiree->getPlanning().at(index);

        QDialog fenetreInfos;
            fenetreInfos.setWindowTitle(objet->nomComplet());
            QHBoxLayout *layout = new QHBoxLayout;

                QLabel *image = new QLabel;

                if(QFile::exists("icones/"+objet->ref()+".jpg"))
                    image->setPixmap(QPixmap("icones/"+objet->ref()+".jpg"));
                else
                    image->setPixmap(QPixmap("icones/default.png"));

                layout->addWidget(image);
                QVBoxLayout *layoutV = new QVBoxLayout;
                QLabel *l_ascdr = new QLabel(tr("<strong>Ascension droite</strong> : ")+objet->ascdr());
                QLabel *l_dec = new QLabel(tr("<strong>Déclinaison</strong> : ")+objet->declinaison());
                QLabel *l_type = new QLabel(tr("<strong>Type</strong> : ")+objet->type());
                QLabel *l_mag = new QLabel(tr("<strong>Magnitude</strong> : ")+QString::number(objet->magnitude()));
                QLabel *l_cons = new QLabel(tr("<strong>Constellation</strong> : ")+Calculastro::abreviationToNom(objet->constellation()));
                QLabel *l_taille = new QLabel(tr("<strong>Taille</strong> : ")+QString::number(objet->taille())+"'");
                QLabel *l_hauteur = new QLabel(tr("<strong>Hauteur</strong> : ")+Calculastro::degreeToDms(m_soiree->hauteurAzimutObjet(index).at(0)));
                QLabel *l_azimut = new QLabel(tr("<strong>Azimut</strong> : ")+Calculastro::degreeToDms(m_soiree->hauteurAzimutObjet(index).at(1)));

                layoutV->addWidget(l_ascdr);
                layoutV->addWidget(l_dec);
                layoutV->addWidget(l_type);
                layoutV->addWidget(l_mag);
                layoutV->addWidget(l_cons);
                layoutV->addWidget(l_taille);
                layoutV->addWidget(l_hauteur);
                layoutV->addWidget(l_azimut);
                layout->addLayout(layoutV);

            fenetreInfos.setLayout(layout);
        fenetreInfos.exec();
    }
}
void InterfaceCreation::enregistrerSoiree()
{
    if(m_active)
    {
        emit afficher(tr("Enregistrement de la soirée en cours..."));
        if(m_soiree->shouldBeSaved()) // Si on doit réellement l'enregistrer
        {
            m_soiree->enregistrerSoiree(); // On l'enregistre
            QMessageBox::information(this,"Succès","Enregistrement réussi.");
            griserActions();
        }
    }
}
void InterfaceCreation::infosSoiree()
{
    if(m_active)
    {
        QDialog fenetreInfos;
        QLabel *ville = new QLabel(tr("<strong>Ville</strong> : ")+m_soiree->getVille());
        QLabel *pays = new QLabel(tr("<strong>Pays</strong> : ")+m_soiree->getPays());
        QLabel *latitude = new QLabel(tr("<strong>Latitude</strong> : ")+Calculastro::degreeToDms(m_soiree->getLat()));
        QLabel *longitude = new QLabel(tr("<strong>Longitude</strong> : ")+Calculastro::degreeToDms(m_soiree->getLongi()));
        QLabel *debut = new QLabel(tr("<strong>Début</strong> : ")+m_soiree->getDebut().toString("dd/MM/yyyy hh:mm"));
        QLabel *fin = new QLabel(tr("<strong>Fin</strong> : ")+m_soiree->getFin().toString("dd/MM/yyyy hh:mm"));
        QLabel *nbObjets = new QLabel(tr("<strong>Nombre d'objets</strong> : ")+QString::number(m_soiree->getPlanning().count()));

        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(pays);
        layout->addWidget(ville);
        layout->addWidget(latitude);
        layout->addWidget(longitude);
        layout->addWidget(debut);
        layout->addWidget(fin);
        layout->addWidget(nbObjets);

        fenetreInfos.setLayout(layout);
        fenetreInfos.setWindowTitle(tr("Infos sur la soirée"));

        fenetreInfos.exec();
    }
}
void InterfaceCreation::griserActions()
{
    if(m_active)
    {
        // BOUTON AJOUT D'OBJET
        if(m_vueRecherche->currentIndex().isValid()) // Si on est sur un index valide
            ajouterObjet->setDisabled(false); // On montre l'action
        else
            ajouterObjet->setDisabled(true); // Sinon on la grise

        // BOUTON D'ACTIONS
        if(m_vue->currentIndex().isValid())
        {
            if(m_soiree->getPlanning().count() > 1) { // Si il y a plus d'un objet, on peut afficher
                m_listeActions->griserActionMonterObjet(false);
                m_listeActions->griserActionDescendreObjet(false);
            }
            else { // Sinon, on ne peut pas les afficher, on grise
                m_listeActions->griserActionMonterObjet(true);
                m_listeActions->griserActionDescendreObjet(true);
            }

            if(m_vue->currentIndex().row()+1 == m_soiree->getPlanning().count()) // Si on est au dernier item
                m_listeActions->griserActionDescendreObjet(true);
            else if(m_vue->currentIndex().row() == 0) // Si on est au premier item, on ne peut pas monter
                m_listeActions->griserActionMonterObjet(true);

            m_listeActions->griserActionChangerDureeObjet(false);
            m_listeActions->griserActionAjouterPlanete(false);
            enleverObjet->setDisabled(false);
        }
        else
        {
            m_listeActions->griserActionMonterObjet(true);
            m_listeActions->griserActionDescendreObjet(true);
            m_listeActions->griserActionChangerDureeObjet(true);
            m_listeActions->griserActionAjouterPlanete(true);
            enleverObjet->setDisabled(true);
        }
        m_listeActions->griserActionSupprimerObjet(true); // On n'utilise pas cette action, on la grise

        // BOUTONS AUTRES
        m_listeActions->griserActionEnregistrer(false);
        m_listeActions->griserActionFermer(false);

        m_listeActions->griserActionExporterJSON(true);
        m_listeActions->griserActionExporterPDF(true);
        m_listeActions->griserActionExporterXML(true);
        m_listeActions->griserActionDiaporama(true);
        m_listeActions->griserActionCarteCiel(true);
            m_listeActions->griserCarteCouleurConstellation(true);
            m_listeActions->griserCarteCouleurEtoiles(true);
            m_listeActions->griserCarteCouleurFond(true);
            m_listeActions->griserCarteCouleurLegende(true);
            m_listeActions->griserCarteCouleurObjet(true);

    }
}
Soiree* InterfaceCreation::getSoiree()
{
    return m_soiree;
}
