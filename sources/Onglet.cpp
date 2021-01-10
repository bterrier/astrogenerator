#include "Onglet.h"
#include "Carteciel.h"

Onglet::Onglet(Soiree *soiree)
{
    m_soiree = soiree;
    m_carte = new Carteciel(soiree);
    m_current = new QWidget;
}
QWidget* Onglet::getWidget(QString ref)
{
    QWidget *widgetSec = new QWidget;
    int i = m_soiree->indexFromRef(ref);

    QVBoxLayout *layoutSec = new QVBoxLayout;
    layoutSec->setAlignment(Qt::AlignLeft|Qt::AlignTop);
        QLabel *titreObjet = new QLabel(m_soiree->getPlanning().at(i)->nomComplet());
        titreObjet->setFont(QFont("Verdana", 15, 30));
    layoutSec->addWidget(titreObjet);
        QHBoxLayout *layoutIcoDesc = new QHBoxLayout;
            QLabel *label = new QLabel;
            label->setPixmap(QPixmap("icones/"+m_soiree->getPlanning().at(i)->ref()));
            layoutIcoDesc->addWidget(label,0,Qt::AlignLeft);

        // On cr�e la description de l'objet
        QString descObjet;
        descObjet = tr(" <strong>Ascension droite</strong> : ","Laisser les balises et les espaces") + m_soiree->getPlanning().at(i)->ascdr() +
                tr("<br /><strong>D�clinaison</strong> : ","Laisser les balises et les espaces") + m_soiree->getPlanning().at(i)->declinaison() +
                tr("<br /><strong>Magnitude</strong> : ","Laisser les balises et les espaces") + QString::number(m_soiree->getPlanning().at(i)->magnitude()) +
                tr("<br /><strong>Type</strong> : ","Laisser les balises et les espaces") + m_soiree->getPlanning().at(i)->type() +
                tr("<br /><strong>Constellation</strong> : ","Laisser les balises et les espaces") + m_soiree->getPlanning().at(i)->constellation();
        if(m_soiree->getPlanning().at(i)->taille() > 0)
            descObjet += "<br /> <strong>Taille</strong> : " + QString::number(m_soiree->getPlanning().at(i)->taille());
        layoutIcoDesc->addWidget(new QLabel(descObjet),0,Qt::AlignLeft);
        layoutIcoDesc->setAlignment(Qt::AlignLeft);
    layoutSec->addLayout(layoutIcoDesc);
        QTextEdit *champRemarques = new QTextEdit("L'amas a �t� d�couvert par Giovanni Domenico Maraldi en 1746 alors qu'il recherchait la com�te de De Cheseaux : il le d�crivit comme une �toile floue constitu�e de plusieurs �toiles. En 1760 il fut int�gr� dans le catalogue Messier qui y vit plut�t, comme pour la plupart des objets qu'il a catalogu�s, une � n�buleuse sans �toiles �. Une fois encore, cet amas fut r�solu en �toiles par William Herschel en 1783. En 1927, des photos prises � l'observatoire du Mont Wilson permirent d'y d�couvrir une n�buleuse plan�taire, Pease 1.");
        champRemarques->setMaximumHeight(100);
    layoutSec->addWidget(champRemarques);
        QHBoxLayout *layoutActions = new QHBoxLayout;
        QLabel *dateObs = new QLabel("Observer entre " + m_soiree->getPlanning().at(i)->getDebut().time().toString(Qt::SystemLocaleShortDate) + " et " + m_soiree->getPlanning().at(i)->getFin().time().toString(Qt::SystemLocaleShortDate));
        layoutActions->addWidget(dateObs);
    layoutSec->addLayout(layoutActions);
    layoutSec->addWidget(new QPushButton("Lancer"),0,Qt::AlignRight);
    widgetSec->setLayout(layoutSec);
    m_current = widgetSec;
    return widgetSec;
}
Soiree* Onglet::soiree() const
{
    return m_soiree;
}
QWidget* Onglet::current() const
{
    return m_current;
}
Carteciel* Onglet::carte() const
{
    return m_carte;
}
