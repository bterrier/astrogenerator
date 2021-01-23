#include "FenInfosCreation.h"
#include "Constantes.h"

#include <QComboBox>
#include <QFormLayout>
#include <QPushButton>
#include <QSqlQuery>

FenInfosCreation::FenInfosCreation(FenPrincipal *parent) :
    QDialog(parent)
{
    m_parent = parent;

    QString paysD,villeD;
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
    m_date->setDate(QDate::currentDate());
    m_heure = new QTimeEdit;
    m_heure->setTime(QTime(20,0));
    m_heure->setDisplayFormat("hh:mm");

    QStringList listTelescope;
    m_telescope = new QComboBox;
    QSqlQuery requete("SELECT nom FROM telescope ORDER BY nom");
    while(requete.next())
    {
        listTelescope << requete.value(0).toString();
    }
    m_telescope->addItems(listTelescope);
    m_telescope->setCurrentIndex(m_telescope->findText(m_parent->getUser()->value("telescope/nom",TELESCOPE_DEFAUT).toString()));

    m_submit = new QPushButton(tr("Valider"));
    m_close = new QPushButton(tr("Annuler"));

    layoutForm = new QFormLayout;
    layoutForm->addRow(tr("&Pays"),m_pays);
    layoutForm->addRow(tr("&Département"),m_departement);
    layoutForm->addRow(tr("&Ville"),m_villes);
    layoutForm->addRow(tr("&Latitude"),m_latitude);
    layoutForm->addRow(tr("&Longitude"),m_longitude);
    layoutForm->addRow(tr("D&ate de début"),m_date);
    layoutForm->addRow(tr("&Heure de début"),m_heure);
    layoutForm->addRow(tr("&Télescope"),m_telescope);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(m_submit);
    layout->addWidget(m_close);
    layoutForm->addRow(layout);

    connect(m_submit, &QPushButton::clicked, this, &FenInfosCreation::creer);
    connect(m_close, &QPushButton::clicked, this, &FenInfosCreation::close);
    connect(m_pays, qOverload<const QString &>(&QComboBox::currentIndexChanged), this, &FenInfosCreation::actualiserVilles);
    connect(m_departement, qOverload<int>(&QSpinBox::valueChanged), this, [this](){ actualiserVilles(); });
    connect(m_villes, qOverload<const QString &>(&QComboBox::currentIndexChanged), this, &FenInfosCreation::actualiserCoordonnees);

    setLayout(layoutForm);
    setWindowTitle(tr("Informations relatives à la soirée"));
}
void FenInfosCreation::actualiserCoordonnees(QString nom)
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
void FenInfosCreation::actualiserVilles(QString pays)
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
void FenInfosCreation::creer()
{
    QDateTime heureDebut(m_date->date(),m_heure->time());
    QSqlQuery requete("SELECT diametre,focale FROM telescope WHERE nom = :nom");
    requete.bindValue(":nom",m_telescope->currentText());
    requete.exec();
    requete.next();

    m_parent->nouvelOngletCreation(m_latitude->value(),m_longitude->value(),heureDebut,requete.value(0).toUInt(),requete.value(1).toUInt());
    this->close();
}

