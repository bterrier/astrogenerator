/* #include "Fenetre.h"
#include "Calculastro.h"

Fenetre::Fenetre(ActionsFenetre *listeActions, Soiree *soiree, QAbstractItemView *vue) : QObject()
{
    m_listeActions = listeActions;
    m_soiree = soiree;
}
void Fenetre::afficherInfosObjet(Objet *objet)
{
    QDialog fenetreInfos;
        fenetreInfos.setWindowTitle(objet->nomComplet());
        QHBoxLayout *layout = new QHBoxLayout;

            QLabel *image = new QLabel;
            image->setPixmap(QPixmap("icones/"+objet->ref()+".jpg"));
            layout->addWidget(image);
            QVBoxLayout *layoutV = new QVBoxLayout;
            QLabel *l_ascdr = new QLabel(tr("<strong>Ascension droite</strong> : ")+objet->ascdr());
            QLabel *l_dec = new QLabel(tr("<strong>Déclinaison</strong> : ")+objet->declinaison());
            QLabel *l_mag = new QLabel(tr("<strong>Magnitude</strong> : ")+QString::number(objet->magnitude()));
            QLabel *l_cons = new QLabel(tr("<strong>Constellation</strong> : ")+Calculastro::abreviationToNom(objet->constellation()));
            QLabel *l_taille = new QLabel(tr("<strong>Taille</strong> : ")+QString::number(objet->taille())+"'");
            layoutV->addWidget(l_ascdr);
            layoutV->addWidget(l_dec);
            layoutV->addWidget(l_mag);
            layoutV->addWidget(l_cons);
            layoutV->addWidget(l_taille);
            layout->addLayout(layoutV);

        fenetreInfos.setLayout(layout);
    fenetreInfos.exec();
}*/

