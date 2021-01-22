#include "Diaporama.h"
#include "Carteciel.h"
#include "CompteRebours.h"
#include "WidgetHeure.h"
#include "Calculastro.h"
#include "Constantes.h"

#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>

Diaporama::Diaporama(Soiree *soiree)
{
    // Initialisation des attributs
    diapoCurrent = 0;
    m_pause = false; // Indique si on est en pause
    m_soiree = soiree;
    widgetPause = new QWidget;
    lcd_pause = new CompteRebours;
    timer = new QTimer;

    // On modifie la palette du widget Diaporama
    QPalette palette1 = palette();
    palette1.setColor(QPalette::Window, QColor(0,0,0));
    setPalette(palette1);
    setAutoFillBackground(true);
}
void Diaporama::afficher(int id)
{
    widgets.at(diapoCurrent)->setVisible(false);
    diapoCurrent = id;
    widgets.at(id)->setVisible(true);

    timer->stop(); // On arrête le timer précédent

    int temps = m_soiree->getPlanning().at(id)->getFin().toTime_t() - m_soiree->getPlanning().at(id)->getDebut().toTime_t(); // On calcule le temps d'affichage
    timer->singleShot(temps*1000,this, &Diaporama::suivant);
    // Commencer le timer correspondant ici
    CompteRebours *timer = widgets.at(id)->findChild<CompteRebours *>();
    timer->commencer(temps);
}
void Diaporama::suivant()
{
    if(m_pause) // Si on était en pause
    {
        m_pause = false; // On est plus en pause
        timer->stop();
        widgetPause->setVisible(false);

        if(diapoCurrent+1 < widgets.size()) // On affiche la diapo suivante si possible
            afficher(diapoCurrent+1);
    }
    else // Si on était pas en pause
    {
        m_pause = true;
        // On vérifie si il y a une pause
        if(diapoCurrent+1 < widgets.size())
        {
           int tempsPause = m_soiree->getPlanning().at(diapoCurrent+1)->getDebut().toTime_t() - m_soiree->getPlanning().at(diapoCurrent)->getFin().toTime_t();
           if(tempsPause > 0)
               pause(tempsPause); // On fait une pause de x secondes
           else
               suivant(); // Sinon on passe au suivant
        }
    }
}
void Diaporama::precedent()
{
    if(m_pause)
    {
        m_pause = false;
        timer->stop();
        widgetPause->setVisible(false);

        afficher(diapoCurrent);
    }
    else
    {
        m_pause = true;
        if(diapoCurrent > 0)
        {
            int tempsPause = m_soiree->getPlanning().at(diapoCurrent)->getDebut().toTime_t() - m_soiree->getPlanning().at(diapoCurrent-1)->getFin().toTime_t();
            if(tempsPause > 0)
            {
                pause(tempsPause);
                diapoCurrent--;
            }
            else
                precedent();
        }
    }
}
void Diaporama::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Left)
        precedent();
    else if(event->key() == Qt::Key_Right)
        suivant();
    else if(event->key() == Qt::Key_Escape)
        close();
}
void Diaporama::pause(int secondes)
{
    widgets.at(diapoCurrent)->setVisible(false);

    widgetPause->setVisible(true);
    lcd_pause->commencer(secondes);
    timer->singleShot(secondes*1000, this, &Diaporama::suivant);
}
void Diaporama::demarrer()
{
    QHBoxLayout *layoutPrincipal = new QHBoxLayout;
    int premierTemps;
    QSettings *user = new QSettings(NOM_EQUIPE,NOM_PROGRAMME);
    QVector<int> oculairesInt;
    QStringList oculairesString = user->value("oculaires",OCULAIREES_DEFAUT).toString().split("|");
    for(int l(0);l<oculairesString.size();l++)
        oculairesInt.push_back(oculairesString.at(l).toInt());
    premierTemps = m_soiree->getPlanning().at(0)->getFin().toTime_t() - m_soiree->getPlanning().at(0)->getDebut().toTime_t();
    // On cherche la différence entre le premier et le deuxième objet pour commencer le timer

    for(int i(0);i<m_soiree->getPlanning().count();i++)
    {
        // On crée le nom de l'objet
        QLabel *nom = new QLabel("<font color=\"#FFF\">"+m_soiree->getPlanning().at(i)->nomComplet()+"</font>");
            nom->setFont(QFont("Verdana",30,30));
            nom->setAlignment(Qt::AlignCenter);

        // On crée le compte à rebours
        CompteRebours *lcd = new CompteRebours;
            lcd->setFixedSize(LARGEUR_COMPTE_REBOURS_DIAPO,HAUTEUR_COMPTE_REBOURS_DIAPO);

        // On crée l'image d'icone
        QLabel *icone = new QLabel;
        icone->setPixmap(QPixmap("icones/"+m_soiree->getPlanning().at(i)->ref()+".jpg"));

        // On crée le widget de l'heure
        WidgetHeure *heure = new WidgetHeure;
            heure->setFixedSize(LARGEUR_COMPTE_REBOURS_DIAPO,HAUTEUR_COMPTE_REBOURS_DIAPO);

        // On crée le widget d'infos
        QWidget *widgetInfos = new QWidget;
            QVBoxLayout *layoutInfos = new QVBoxLayout;
            QLabel *infoAscdr = new QLabel(tr("<font color=\"#FFF\">Ascension droite : ") + m_soiree->getPlanning().at(i)->ascdr()+"</font>");
                infoAscdr->setFont(QFont("Verdana",15));
            QLabel *infoDec = new QLabel(tr("<font color=\"#FFF\">Déclinaison : ") + m_soiree->getPlanning().at(i)->declinaison());
                infoDec->setFont(QFont("Verdana",15));
            QLabel *infoHauteur = new QLabel(tr("<font color=\"#FFF\">Hauteur : ") + Calculastro::degreeToDms(m_soiree->hauteurAzimutObjet(i).at(0))+"</font>");
                infoHauteur->setFont(QFont("Verdana",15));
            QLabel *infoAzimut = new QLabel(tr("<font color=\"#FFF\">Azimut : ") + Calculastro::degreeToDms(m_soiree->hauteurAzimutObjet(i).at(1))+"</font>");
                infoAzimut->setFont(QFont("Verdana",15));
            QLabel *infoCons = new QLabel(tr("<font color=\"#FFF\">Constellation : ") + Calculastro::abreviationToNom(m_soiree->getPlanning().at(i)->constellation())+"</font>");
                infoCons->setFont(QFont("Verdana",15));
            QLabel *infoOculaire = new QLabel(tr("<font color=\"#FFF\">Oculaire : ") + Calculastro::getOculaire(m_soiree->getPlanning().at(i),m_soiree->getDiametre(),m_soiree->getFocale(),oculairesInt));
                infoOculaire->setFont(QFont("Verdana",15));
            layoutInfos->addWidget(infoAscdr);
            layoutInfos->addWidget(infoDec);
            layoutInfos->addWidget(infoHauteur);
            layoutInfos->addWidget(infoAzimut);
            layoutInfos->addWidget(infoCons);
            layoutInfos->addWidget(infoOculaire);
            widgetInfos->setLayout(layoutInfos);

        QHBoxLayout *layoutTop = new QHBoxLayout;
            layoutTop->addWidget(icone,0,Qt::AlignLeft);
            layoutTop->addWidget(heure,0,Qt::AlignRight);
        QHBoxLayout *layoutBottom = new QHBoxLayout;
            layoutBottom->addWidget(widgetInfos,0,Qt::AlignLeft);
            layoutBottom->addWidget(lcd,0,Qt::AlignRight);

        QVBoxLayout *layoutTotal = new QVBoxLayout;
            layoutTotal->addLayout(layoutTop);
            layoutTotal->addWidget(nom);
            layoutTotal->addLayout(layoutBottom);

        QWidget *widget = new QWidget;
            QPalette palette = widget->palette();
            palette.setColor(QPalette::Window, QColor(0,0,0));
            widget->setPalette(palette);
            widget->setAutoFillBackground(true);

        widget->setLayout(layoutTotal);

        if(i==0)
            widget->setVisible(true);
        else
            widget->setVisible(false);

        widgets.push_back(widget);

       layoutPrincipal->addWidget(widget);
    }

    // On ajoute le widget de pause
        QPalette palette = widgetPause->palette();
        palette.setColor(QPalette::Window, QColor(0,0,0));
        widgetPause->setPalette(palette);
        widgetPause->setAutoFillBackground(true);
    QVBoxLayout *layout = new QVBoxLayout;
        QLabel *label_pause = new QLabel("<font color=\"#FFF\">Pause</font>");
            label_pause->setFont(QFont("Verdana",60,30));
            label_pause->setAlignment(Qt::AlignHCenter);
        layout->addWidget(label_pause);
            lcd_pause->setFixedSize(LARGEUR_COMPTE_REBOURS_DIAPO,HAUTEUR_COMPTE_REBOURS_DIAPO);
        layout->addWidget(lcd_pause,0,Qt::AlignHCenter);
        layout->setAlignment(Qt::AlignHCenter);
    widgetPause->setLayout(layout);
    widgetPause->setVisible(false);
    layoutPrincipal->addWidget(widgetPause);
    // ---------------------------------

    setLayout(layoutPrincipal);

    timer->singleShot(premierTemps*1000, this, &Diaporama::suivant);
    widgets.at(0)->findChild<CompteRebours *>()->commencer(premierTemps);

    showFullScreen();
}
