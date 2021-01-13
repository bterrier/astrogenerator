#include "InterfaceLecture.h"
#include "Soiree.h"
#include "ActionsFenetre.h"
#include "Constantes.h"
#include "Carteciel.h"
#include "Diaporama.h"
#include "ObjetPlaneteObs.h"
#include "ObjetCPObs.h"
#include "Calculastro.h"

#include <QInputDialog>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QPrintDialog>
#include <QToolBar>
#include <QLabel>

InterfaceLecture::InterfaceLecture(Soiree *soiree, ActionsFenetre *listeActionsParam, QWidget *parent) :
    Interface(listeActionsParam, parent)
{
    m_soiree = soiree;
    m_active = true; // On considère qu'à sa construction l'interface est active
    m_modele = soiree->toModele();
    m_carte = new Carteciel(soiree);
    m_vue = new QTableView;
        m_vue->setModel(m_modele);
        m_vue->resizeColumnsToContents(); // Adapte la largeur des colonnes en fonction de leur contenu
        m_vue->setCornerButtonEnabled(false);
        m_vue->setSelectionBehavior(QAbstractItemView::SelectRows);
        m_vue->setSelectionMode(QAbstractItemView::SingleSelection);
        m_vue->setEditTriggers(QAbstractItemView::NoEditTriggers);
        m_vue->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QHBoxLayout *layoutPrincipal = new QHBoxLayout;

    QToolBar *outilsOnglets = new QToolBar;
        outilsOnglets->addAction(m_listeActions->getActionAjouterObjet());
        outilsOnglets->addAction(m_listeActions->getActionSupprimerObjet());
        outilsOnglets->addAction(m_listeActions->getActionMonterObjet());
        outilsOnglets->addAction(m_listeActions->getActionDescendreObjet());
        outilsOnglets->addAction(m_listeActions->getActionChangerDureeObjet());
        outilsOnglets->addAction(m_listeActions->getActionAjouterPlanete());
        outilsOnglets->addAction(m_listeActions->getActionCarteCiel());
        outilsOnglets->addAction(m_listeActions->getActionDiaporama());
        outilsOnglets->addAction(m_listeActions->getActionExporterPDF());
        outilsOnglets->addAction(m_listeActions->getActionInfoSoiree());
        outilsOnglets->setOrientation(Qt::Vertical);
    layoutPrincipal->addWidget(outilsOnglets);

    QVBoxLayout *layoutDroiteContenu = new QVBoxLayout;
    layoutDroiteContenu->addWidget(m_vue);

    layoutPrincipal->addLayout(layoutDroiteContenu);

    setLayout(layoutPrincipal);

    // On fait toutes les connexions
    connect(m_vue,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(afficherInfosObjet(QModelIndex)));
    connect(m_vue,SIGNAL(clicked(QModelIndex)),this,SLOT(griserActions()));
    connect(m_listeActions->getActionMonterObjet(),SIGNAL(triggered()),this,SLOT(monterObjet()));
    connect(m_listeActions->getActionDescendreObjet(),SIGNAL(triggered()),this,SLOT(descendreObjet()));
    connect(m_listeActions->getActionSupprimerObjet(),SIGNAL(triggered()),this,SLOT(supprimerObjet()));
    connect(m_listeActions->getActionChangerDureeObjet(),SIGNAL(triggered()),this,SLOT(modifierObjet()));
    connect(m_listeActions->getActionAjouterObjet(),SIGNAL(triggered()),this,SLOT(ajouterObjet()));
    connect(m_listeActions->getActionAjouterPlanete(),SIGNAL(triggered()),this,SLOT(ajouterPlanete()));
    connect(m_listeActions->getCarteCouleurConstellation(),SIGNAL(triggered()),this,SLOT(setCouleurConstellations()));
    connect(m_listeActions->getCarteCouleurEtoiles(),SIGNAL(triggered()),this,SLOT(setCouleurEtoiles()));
    connect(m_listeActions->getCarteCouleurFond(),SIGNAL(triggered()),this,SLOT(setCouleurFond()));
    connect(m_listeActions->getCarteCouleurLegende(),SIGNAL(triggered()),this,SLOT(setCouleurLegende()));
    connect(m_listeActions->getCarteCouleurObjet(),SIGNAL(triggered()),this,SLOT(setCouleurObjet()));
    connect(m_listeActions->getActionEnregistrer(),SIGNAL(triggered()),this,SLOT(enregistrerSoiree()));
    connect(m_listeActions->getActionCarteCiel(),SIGNAL(triggered()),this,SLOT(carteCiel()));
    connect(m_listeActions->getActionDiaporama(),SIGNAL(triggered()),this,SLOT(diaporama()));
    connect(m_listeActions->getActionExporterXML(),SIGNAL(triggered()),this,SLOT(toXML()));
    connect(m_listeActions->getActionExporterPDF(),SIGNAL(triggered()),this,SLOT(toPDF()));
    connect(m_listeActions->getActionInfoSoiree(),SIGNAL(triggered()),this,SLOT(infosSoiree()));
    connect(m_listeActions->getActionImprimer(),SIGNAL(triggered()),this,SLOT(imprimer()));
}
void InterfaceLecture::afficherInfosObjet(QModelIndex cells)
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

                //QMessageBox::information(this,"Lien absolu",QDir::currentPath());

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
void InterfaceLecture::monterObjet()
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
                    m_modele = m_soiree->toModele();
                    m_vue->setModel(m_modele);
                    griserActions();
                    emit afficher(tr("L'objet a été monté"));
                }
            }
        }
    }
}
void InterfaceLecture::descendreObjet()
{
    if(m_active)
    {
        if(m_vue->currentIndex().isValid())
        {
            int reponse = QMessageBox::question(0, tr("Confirmer"), tr("Voulez-vous vraiment descendre l'objet ?"), QMessageBox ::Yes | QMessageBox::No);

            if(reponse == QMessageBox::Yes)
            {
                int ligne = m_vue->currentIndex().row();
                QString ref = m_modele->item(ligne,1)->text();

                int index = m_soiree->indexFromRef(ref);
                if(index > -1)
                {
                    m_soiree->descendreObjet(index);
                    m_modele = m_soiree->toModele();
                    m_vue->setModel(m_modele);
                    griserActions();
                    emit afficher(tr("L'objet a été descendu"));
                }
            }
        }
    }
}
void InterfaceLecture::supprimerObjet()
{
    if(m_active)
    {
        if(m_vue->currentIndex().isValid())
        {
            int reponse = QMessageBox::question(0, tr("Confirmer"), tr("Voulez-vous vraiment supprimer l'objet ?"), QMessageBox ::Yes | QMessageBox::No);
            if(reponse == QMessageBox::Yes)
            {
                QString ref = m_modele->item(m_vue->currentIndex().row(),1)->text();

                int index = m_soiree->indexFromRef(ref);
                if(index > -1)
                {
                    m_soiree->supprimerObjet(index);
                    m_modele = m_soiree->toModele();
                    m_vue->setModel(m_modele);
                    griserActions();
                    emit afficher(tr("L'objet a été supprimé"));
                }
            }
        }
    }
}
void InterfaceLecture::modifierObjet()
{
    if(m_active)
    {
        if(m_vue->currentIndex().isValid())
        {
            bool* ok = new bool;
            int reponse = QInputDialog::getInt(0,tr("Modifier la durée"),tr("Quelle est la nouvelle durée ? (min)"),5,DUREE_OBJET_MIN,DUREE_OBJET_MAX,1,ok);
            if(*ok == true)
            {
                QString ref = m_modele->item(m_vue->currentIndex().row(),1)->text();

                int index = m_soiree->indexFromRef(ref);
                if(index > -1)
                {
                    m_soiree->modifierDuree(index,reponse);
                    m_modele = m_soiree->toModele();
                    m_vue->setModel(m_modele);
                    griserActions();
                    emit afficher(tr("L'objet a été modifié"));
                }
            }
         }
    }
}
void InterfaceLecture::ajouterPlanete()
{
    if(m_active)
    {
        if(m_vue->currentIndex().isValid())
        {
            bool ok = true;
            QStringList planetes;
            planetes << tr("Mercure") << tr("Vénus") << tr("Mars") << tr("Jupiter") << tr("Saturne") << tr("Uranus") << tr("Neptune");
            QString reponse = QInputDialog::getItem(0,tr("Ajouter une planète"),tr("Choisisssez la planète à ajouter"),planetes,0,false,&ok);
            if(ok)
            {
                int index = planetes.indexOf(reponse);
                if(index == -1)
                    qDebug() << "[ERREUR INTERNE] ajouterPlanete() dans InterfaceLecture";
                else
                {
                    QString ref = m_modele->item(m_vue->currentIndex().row(),1)->text();
                    int indexPlace = m_soiree->indexFromRef(ref);

                    m_soiree->ajouterObjet(indexPlace,"P"+QString::number(index+1),TEMPS_OBS_DEFAUT);

                    m_modele = m_soiree->toModele();
                    m_vue->setModel(m_modele);
                    griserActions();
                    emit afficher(tr("La planète a été ajoutée"));
                }
            }
        }
    }
}

void InterfaceLecture::ajouterObjet()
{
    if(m_active)
    {
        if(m_vue->currentIndex().isValid())
        {
            bool ok = true;
            QString reponse = QInputDialog::getText(0,tr("Ajouter un objet"),tr("Quel objet ajouter ? (NGC****, M** ou P*)"),QLineEdit::Normal,"",&ok);
            if(ok == true)
            {
                if(reponse.left(1)=="P")
                {
                    QString ref = m_modele->item(m_vue->currentIndex().row(),1)->text();

                    int index = m_soiree->indexFromRef(ref);
                    if(index > -1)
                    {
                        m_soiree->ajouterObjet(index,reponse,TEMPS_OBS_DEFAUT);
                        m_modele = m_soiree->toModele();
                        m_vue->setModel(m_modele);
                        griserActions();
                        emit afficher(tr("La planète a été ajoutée"));
                    }
                }
                else
                {
                    ObjetCP *objet = new ObjetCP(reponse);
                    if(objet->isValid())
                    {
                        QString ref = m_modele->item(m_vue->currentIndex().row(),1)->text();

                        int index = m_soiree->indexFromRef(ref);
                        if(index > -1)
                        {
                            m_soiree->ajouterObjet(index,objet,TEMPS_OBS_DEFAUT);
                            m_modele = m_soiree->toModele();
                            m_vue->setModel(m_modele);
                            griserActions();
                            emit afficher(tr("L'objet a été ajouté"));
                        }
                    }
                }
            }
        }
    }
}
void InterfaceLecture::setCouleurConstellations()
{
    if(m_active)
        m_carte->setCouleurConstellation();
}
void InterfaceLecture::setCouleurEtoiles()
{
    if(m_active)
        m_carte->setCouleurEtoile();
}
void InterfaceLecture::setCouleurFond()
{
    if(m_active)
        m_carte->setCouleurFond();
}
void InterfaceLecture::setCouleurLegende()
{
    if(m_active)
        m_carte->setCouleurLegende();
}
void InterfaceLecture::setCouleurObjet()
{
    if(m_active)
        m_carte->setCouleurObjet();
}
void InterfaceLecture::enregistrerSoiree()
{
    if(m_active)
    {
        emit afficher(tr("Enregistrement de la soirée en cours..."));
        if(m_soiree->shouldBeSaved()) // Si on doit réellement l'enregistrer
        {
            m_soiree->enregistrerSoiree(); // On l'enregistre
            QMessageBox::information(this,tr("Succès"),tr("Enregistrement réussi."));
            griserActions();
        }
    }
}
void InterfaceLecture::carteCiel()
{
    if(m_active)
    {
        emit afficher(tr("Création de la carte du ciel..."));
        m_carte->afficherQDialog();
    }
}
void InterfaceLecture::diaporama()
{
    if(m_active)
    {
        bool ok;
        QStringList reponses;
        reponses << tr("1 - Lire la soirée maintenant","Important : laisser le 1 comme premier caractère") << tr("2 - Démarrer la soirée à l'heure prévue","Important : laisser le 2 comme premier caractère") << tr("3 - Ne rien faire","Important : laisser le 3 comme premier caractère");
        QString reponse = QInputDialog::getItem(0,tr("Choisissez le mode"),tr("Comment voulez-vous démarrer la soirée ?"),reponses,0,false,&ok);
        if(ok)
        {
            if(reponse.left(1) == "1") // Si on demarre tout de suite
            {
                emit afficher(tr("Initialisation du diaporama en cours..."));
                Diaporama *diapo = new Diaporama(m_soiree);
                diapo->demarrer();
            }
            else if(reponse.left(1) == "2") // Si on décide de démarrer la soirée à l'heure prévue
            {
                if(m_soiree->getDebut().toLocalTime().date() == QDate::currentDate() && m_soiree->getDebut().toTime_t() > QDateTime::currentDateTime().toTime_t()) // Si on est sur une soirée qui commence aujourd'hui et après maintenant
                {
                    int secondesRestantes(0);
                    secondesRestantes = m_soiree->getDebut().toLocalTime().toTime_t() - QDateTime::currentDateTime().toTime_t();
                    Diaporama *diapo = new Diaporama(m_soiree);

                    QTimer::singleShot(secondesRestantes*1000,diapo,SLOT(demarrer()));
                }
                else
                    QMessageBox::warning(this,tr("Impossible d'effectuer l'action"),tr("Il est impossible de programmer le lancement de cette soirée, elle ne correspond pas à l'heure et à la date actuelle."));
            }
        }
    }
}
void InterfaceLecture::toXML()
{
   if(m_active)
        m_soiree->toXML();
}
void InterfaceLecture::toPDF()
{
    if(m_active)
        m_soiree->toPDF();
}
Soiree *InterfaceLecture::getSoiree()
{
    return m_soiree;
}
void InterfaceLecture::infosSoiree()
{
    if(m_active)
    {
        QDialog fenetreInfos;
        QLabel *ville = new QLabel(tr("<strong>Ville</strong> : ")+m_soiree->getVille());
        QLabel *pays = new QLabel(tr("<strong>Pays</strong> : ")+m_soiree->getPays());
        QLabel *latitude = new QLabel(tr("<strong>Latitude</strong> : ")+Calculastro::degreeToDms(m_soiree->getLat()));
        QLabel *longitude = new QLabel(tr("<strong>Longitude</strong> : ")+Calculastro::degreeToDms(m_soiree->getLongi()));
        QLabel *debut = new QLabel(tr("<strong>Début</strong> : ")+m_soiree->getDebut().toLocalTime().toString("dd/MM/yyyy hh:mm"));
        QLabel *fin = new QLabel(tr("<strong>Fin</strong> : ")+m_soiree->getFin().toLocalTime().toString("dd/MM/yyyy hh:mm"));
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
void InterfaceLecture::griserActions()
{
    if(m_active)
    {
        if(m_vue->currentIndex().isValid()) // Si on est sur un index valide
        {
            if(m_vue->currentIndex().row()+1 == m_soiree->getPlanning().count()) // Si on est au dernier item
                m_listeActions->griserActionDescendreObjet(true);
            else
                m_listeActions->griserActionDescendreObjet(false);
            if(m_vue->currentIndex().row() == 0)
                m_listeActions->griserActionMonterObjet(true);
            else
                m_listeActions->griserActionMonterObjet(false);

            m_listeActions->griserActionChangerDureeObjet(false);
            m_listeActions->griserActionSupprimerObjet(false);
            m_listeActions->griserActionAjouterObjet(false);
            m_listeActions->griserActionAjouterPlanete(false);
        }
        else // Sinon
        {
            m_listeActions->griserActionDescendreObjet(true);
            m_listeActions->griserActionMonterObjet(true);
            m_listeActions->griserActionChangerDureeObjet(true);
            m_listeActions->griserActionSupprimerObjet(true);
            m_listeActions->griserActionAjouterObjet(true);
            m_listeActions->griserActionAjouterPlanete(true);
        }
        if(m_soiree->shouldBeSaved())
            m_listeActions->griserActionEnregistrer(false);
        else
            m_listeActions->griserActionEnregistrer(true);
    }
}
void InterfaceLecture::imprimer()
{
    QPrinter *printer = new QPrinter(QPrinter::ScreenResolution);
    printer->setPageMargins(QMarginsF(7,7,7,7), QPageLayout::Millimeter);

    QPrintDialog *dialog = new QPrintDialog(printer, this);
    dialog->setWindowTitle(tr("Imprimer la soirée"));
    dialog->addEnabledOption(QAbstractPrintDialog::PrintSelection);

    if (dialog->exec() != QDialog::Accepted)
        return;

    m_soiree->paintPdf(printer);
}
