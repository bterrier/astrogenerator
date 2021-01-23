#include "FenetreBDD.h"
#include "Calculastro.h"
#include "Constantes.h"
#include "Objet.h"
#include "ObjetCPObs.h"
#include <QDesktopServices>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QtSql>

FenetreBDD::FenetreBDD(QWidget *parent) :
    QDialog(parent)
{
    m_modele = new QStandardItemModel;
    m_vue = new QTableView;
    m_vue->setCornerButtonEnabled(false);
    m_vue->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_vue->setSelectionMode(QAbstractItemView::SingleSelection);
    m_vue->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_vue->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_vue->horizontalHeader()->hide();
    m_vue->verticalHeader()->hide();

    m_vue->setModel(m_modele);

    // On crée le layout avec tous les champs de personalisation
    QVBoxLayout *layoutVertical = new QVBoxLayout;
    QHBoxLayout *layoutHorizontal = new QHBoxLayout;

    // CHAMP DE RECHERCHE
    QFormLayout *layoutRecherche = new QFormLayout;
    champRecherche = new QLineEdit;
    layoutRecherche->addRow("&Rechercher", champRecherche);

    // GROUPBOX DES TYPES D'OBJETS
    QGroupBox *groupeBoxType = new QGroupBox(tr("Types d'objets"));
    QVBoxLayout *layoutType = new QVBoxLayout;
    typeGalaxie = new QCheckBox(tr("Galaxie"));
    typeGalaxie->setChecked(true);
    typeAmasG = new QCheckBox(tr("Amas Globulaires"));
    typeAmasG->setChecked(true);
    typeNebuleuseR = new QCheckBox(tr("Nébuleuse en réflection"));
    typeNebuleuseR->setChecked(true);
    typeNebuleuseP = new QCheckBox(tr("Nébuleuse planétaire"));
    typeNebuleuseP->setChecked(true);
    typeAmasNebuleuse = new QCheckBox(tr("Amas + Nébuleuse"));
    typeAmasNebuleuse->setChecked(true);
    typeAmasOuvert = new QCheckBox(tr("Amas ouvert"));
    typeAmasOuvert->setChecked(true);

    layoutType->addWidget(typeGalaxie);
    layoutType->addWidget(typeAmasG);
    layoutType->addWidget(typeNebuleuseR);
    layoutType->addWidget(typeNebuleuseP);
    layoutType->addWidget(typeAmasNebuleuse);
    layoutType->addWidget(typeAmasOuvert);

    groupeBoxType->setLayout(layoutType);

    // GROUPBOX DE DIFFICULTE
    QGroupBox *groupeBoxDifficulte = new QGroupBox(tr("Difficulté d'observation"));
    QVBoxLayout *layoutDifficulte = new QVBoxLayout;
    difficulte1 = new QCheckBox(tr("Très facile"));
    difficulte1->setChecked(true);
    difficulte2 = new QCheckBox(tr("Facile"));
    difficulte2->setChecked(true);
    difficulte3 = new QCheckBox(tr("Moyen"));
    difficulte3->setChecked(true);
    difficulte4 = new QCheckBox(tr("Difficile"));
    difficulte4->setChecked(true);
    difficulte0 = new QCheckBox(tr("Inconnue"));

    layoutDifficulte->addWidget(difficulte1);
    layoutDifficulte->addWidget(difficulte2);
    layoutDifficulte->addWidget(difficulte3);
    layoutDifficulte->addWidget(difficulte4);
    layoutDifficulte->addWidget(difficulte0);

    groupeBoxDifficulte->setLayout(layoutDifficulte);

    // GROUPBOX DES INTERETS
    QGroupBox *groupeBoxInteret = new QGroupBox(tr("Interêt de l'observation en visuel"));
    QVBoxLayout *layoutInteret = new QVBoxLayout;
    interet1 = new QCheckBox(tr("Sans interêt"));
    interet1->setChecked(true);
    interet2 = new QCheckBox(tr("Peu interessant"));
    interet2->setChecked(true);
    interet3 = new QCheckBox(tr("Intéressant"));
    interet3->setChecked(true);
    interet4 = new QCheckBox(tr("Remarquable"));
    interet4->setChecked(true);

    layoutInteret->addWidget(interet1);
    layoutInteret->addWidget(interet2);
    layoutInteret->addWidget(interet3);
    layoutInteret->addWidget(interet4);

    groupeBoxInteret->setLayout(layoutInteret);

    // GROUPBOX DES PARAMETRES RESTANTS
    QGroupBox *groupeBoxAutres = new QGroupBox(tr("Autres critères de tri"));
    QFormLayout *layoutAutres = new QFormLayout;
    champMagnitude = new QSpinBox;
    champMagnitude->setMaximum(15);
    champMagnitude->setMinimum(0);
    champMagnitude->setValue(8);
    layoutAutres->addRow(tr("Magnitude maximum"), champMagnitude);
    groupeBoxAutres->setLayout(layoutAutres);

    nombreResultat = new QLabel;
    nombreResultat->setAlignment(Qt::AlignCenter);

    connect(champRecherche, &QLineEdit::returnPressed, this, &FenetreBDD::actualiserVue);
    connect(typeGalaxie, &QCheckBox::clicked, this, &FenetreBDD::actualiserVue);
    connect(typeAmasG, &QCheckBox::clicked, this, &FenetreBDD::actualiserVue);
    connect(typeNebuleuseR, &QCheckBox::clicked, this, &FenetreBDD::actualiserVue);
    connect(typeNebuleuseP, &QCheckBox::clicked, this, &FenetreBDD::actualiserVue);
    connect(typeAmasNebuleuse, &QCheckBox::clicked, this, &FenetreBDD::actualiserVue);
    connect(typeAmasOuvert, &QCheckBox::clicked, this, &FenetreBDD::actualiserVue);

    connect(interet1, &QCheckBox::clicked, this, &FenetreBDD::actualiserVue);
    connect(interet2, &QCheckBox::clicked, this, &FenetreBDD::actualiserVue);
    connect(interet3, &QCheckBox::clicked, this, &FenetreBDD::actualiserVue);
    connect(interet4, &QCheckBox::clicked, this, &FenetreBDD::actualiserVue);

    connect(difficulte0, &QCheckBox::clicked, this, &FenetreBDD::actualiserVue);
    connect(difficulte1, &QCheckBox::clicked, this, &FenetreBDD::actualiserVue);
    connect(difficulte2, &QCheckBox::clicked, this, &FenetreBDD::actualiserVue);
    connect(difficulte3, &QCheckBox::clicked, this, &FenetreBDD::actualiserVue);
    connect(difficulte4, &QCheckBox::clicked, this, &FenetreBDD::actualiserVue);

    connect(champMagnitude, qOverload<int>(&QSpinBox::valueChanged), this, &FenetreBDD::actualiserVue);

    connect(m_vue, &QTableView::doubleClicked, this, &FenetreBDD::afficherObjet);

    layoutVertical->addLayout(layoutRecherche);
    layoutVertical->addWidget(groupeBoxType);
    layoutVertical->addWidget(groupeBoxDifficulte);
    layoutVertical->addWidget(groupeBoxInteret);
    layoutVertical->addWidget(groupeBoxAutres);
    layoutVertical->addWidget(nombreResultat);

    layoutHorizontal->addLayout(layoutVertical);
    layoutHorizontal->addWidget(m_vue, 4);

    setLayout(layoutHorizontal);

    setWindowTitle(tr("Base de données du programme"));
    setMinimumSize(800, 560);

    actualiserVue();
}
void FenetreBDD::actualiserVue()
{
    QSqlQuery *requete = new QSqlQuery(creerRequete());
    QString nom, s;
    int i(0);
    m_modele->clear();
    while (requete->next()) {
        i++;
        if (requete->value(0).toString() != "") {
            nom = requete->value(0).toString() + " (";
            if (requete->value(2).toString() != "0")
                nom += requete->value(2).toString() + ", " + requete->value(1).toString() + ")";
            else
                nom += requete->value(1).toString() + ")";
        } else if (requete->value(2).toString() != "0")
            nom = requete->value(2).toString() + " (" + requete->value(1).toString() + ")";
        else
            nom = requete->value(1).toString();

        QStandardItem *ligne = new QStandardItem(nom);
        ligne->appendRow(new QStandardItem(requete->value(1).toString()));
        m_modele->appendRow(ligne);
    }
    if (i > 1)
        s = "s";
    nombreResultat->setText("Il y a <strong>" + QString::number(i) + " résultat" + s + "</strong>");
    m_vue->setModel(m_modele);
}
QString FenetreBDD::creerRequete() const
{ // On crée la requete à partir de tous les champs

    QString requete, type, difficulte, interet, autres;
    int i = 0;
    requete += "SELECT nom, reference, messier FROM ngcic WHERE ";

    if (typeGalaxie->isChecked() || typeAmasG->isChecked() || typeNebuleuseR->isChecked() || typeNebuleuseP->isChecked() || typeAmasNebuleuse->isChecked() || typeAmasOuvert->isChecked()) {
        type += "(type = ";
        if (typeGalaxie->isChecked())
            type += "'Galaxie' OR type = ";
        if (typeAmasG->isChecked())
            type += "'Amas Globulaire' OR type = ";
        if (typeNebuleuseR->isChecked())
            type += "'Nébuleuse en réflection' OR type = ";
        if (typeNebuleuseP->isChecked())
            type += "'Nébuleuse planétaire' OR type = ";
        if (typeAmasNebuleuse->isChecked())
            type += "'Amas+Nébuleuse' OR type = ";
        if (typeAmasOuvert->isChecked())
            type += "'Amas ouvert' OR type = ";

        i = type.length() - 11;
        type.remove(i, 11);
        type += ") AND ";
    }
    if (interet1->isChecked() || interet2->isChecked() || interet3->isChecked() || interet4->isChecked()) {
        interet += "(interet = ";
        if (interet1->isChecked())
            interet += "1 OR interet = ";
        if (interet2->isChecked())
            interet += "2 OR interet = ";
        if (interet3->isChecked())
            interet += "3 OR interet = ";
        if (interet4->isChecked())
            interet += "4 OR interet = ";

        i = interet.length() - 14;
        interet.remove(i, 14);
        interet += ") AND ";
    }
    if (difficulte0->isChecked() || difficulte1->isChecked() || difficulte2->isChecked() || difficulte3->isChecked() || difficulte4->isChecked()) {
        difficulte += "(difficulte = ";
        if (difficulte0->isChecked())
            difficulte += "0 OR difficulte = ";
        if (difficulte1->isChecked())
            difficulte += "1 OR difficulte = ";
        if (difficulte2->isChecked())
            difficulte += "2 OR difficulte = ";
        if (difficulte3->isChecked())
            difficulte += "3 OR difficulte = ";
        if (difficulte4->isChecked())
            difficulte += "4 OR difficulte = ";

        i = difficulte.length() - 17;
        difficulte.remove(i, 17);
        difficulte += ") AND ";
    }
    autres = "magnitude <= " + QString::number(champMagnitude->value());

    if (champRecherche->text().isEmpty())
        requete += type + interet + difficulte + autres;
    else
        requete = "SELECT nom, reference, messier FROM ngcic WHERE nom LIKE '%" + champRecherche->text() + "%' OR messier LIKE '%" + champRecherche->text() + "%' OR reference LIKE '%" + champRecherche->text() + "%'";

    return requete;
}
void FenetreBDD::afficherObjet(QModelIndex index) const
{
    QString ref = m_modele->data(m_modele->index(0, 0, index)).toString();
    ObjetCPObs *objet = new ObjetCPObs(ref, QDateTime::currentDateTime(), QDateTime::currentDateTime().addSecs(600));
    if (objet->isValid()) {
        QDialog fenetreInfos;
        fenetreInfos.setWindowTitle(objet->nomComplet());

        QLabel *image = new QLabel;
        image->setPixmap(QPixmap("icones/" + objet->ref() + ".jpg"));
        QVBoxLayout *layoutV = new QVBoxLayout;
        QLabel *l_type = new QLabel(tr("<strong>Type</strong> : ") + objet->type());
        QLabel *l_ascdr = new QLabel(tr("<strong>Ascension droite</strong> : ") + objet->ascdr());
        QLabel *l_dec = new QLabel(tr("<strong>Déclinaison</strong> : ") + objet->declinaison());
        QLabel *l_mag = new QLabel(tr("<strong>Magnitude</strong> : ") + QString::number(objet->magnitude()));
        QLabel *l_interet = new QLabel(tr("<strong>Interêt</strong> : ") + objet->interet(true));
        QLabel *l_difficulte = new QLabel(tr("<strong>Difficulté</strong> : ") + objet->difficulte(true));
        QLabel *l_cons = new QLabel(tr("<strong>Constellation</strong> : ") + Calculastro::abreviationToNom(objet->constellation()));
        QLabel *l_taille = new QLabel(tr("<strong>Taille</strong> : ") + QString::number(objet->taille()) + "'");
        QPushButton *boutonInfos = new QPushButton(tr("Plus d'infos sur ", "Cette chaîne est suivie de la référence de l'objet (lien vers un site)") + ref);
        layoutV->addWidget(image, 0, Qt::AlignCenter);
        layoutV->addWidget(l_type);
        layoutV->addWidget(l_ascdr);
        layoutV->addWidget(l_dec);
        layoutV->addWidget(l_mag);
        layoutV->addWidget(l_interet);
        layoutV->addWidget(l_difficulte);
        layoutV->addWidget(l_cons);
        layoutV->addWidget(l_taille);

        QVBoxLayout *layoutVertical = new QVBoxLayout;
        layoutVertical->addLayout(layoutV);
        layoutVertical->addWidget(boutonInfos);

        connect(boutonInfos, &QPushButton::clicked, this, &FenetreBDD::ouvrirCDS);

        fenetreInfos.setLayout(layoutVertical);
        fenetreInfos.exec();
    } else
        QMessageBox::critical(nullptr, tr("Erreur"), tr("Une erreur s'est produite : l'objet est introuvable"));
}
void FenetreBDD::ouvrirCDS()
{
    QItemSelectionModel *selection = m_vue->selectionModel();
    QModelIndex indexElementSelectionne = m_modele->index(0, 0, selection->currentIndex());
    QVariant elementSelectionne = m_modele->data(indexElementSelectionne, Qt::DisplayRole);

    QDesktopServices::openUrl(QUrl(DEBUT_URL_CDS_OBJET + elementSelectionne.toString()));
}
