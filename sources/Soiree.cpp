#include "Soiree.h"
#include "Calculastro.h"
#include "ObjetObs.h"
#include "HauteurMaxTemps.h"
#include "Constantes.h"
#include "ObjetCPObs.h"
#include "Carteciel.h"
#include "ObjetCP.h"
#include "ObjetPlaneteObs.h"
#include <cmath>

#include <QMessageBox>

#include <QtXml>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QGraphicsView>
#include <QFileDialog>
#include <QApplication>

Soiree::Soiree() {}
void Soiree::genererSoiree(double lat, double longi, QDateTime debut, QDateTime fin, int duree_une, QString constellation, QString niveau, unsigned int diametre, unsigned int focale, QSettings *user, bool boolPlanete)
{
    int nb_objets(0), moyTime(0), hauteurMin(30), rep(0), espaceMin(0);
    QDateTime obsPla[2];
    QVector<double> infosPlanete;
    QString valeur;
    double magMax(0), raDecimal(0), declinaisonDecimal(0), note(0);

    espaceMin = user->value("generateur/pauseMin",TEMPS_ESPACE).toInt()*60; // En secondes, utile pour la fonction verifDisponibilite

    duree_une *= 60; // On convertit en secondes
    nb_objets = (fin.toTime_t()-debut.toTime_t())/duree_une;
    rep = (fin.toTime_t()-debut.toTime_t())/60;

    magMax = Calculastro::round(2+5*log10(diametre),1)+1; //  Loi de Pogson diametre du télescope -> magnitude max;

    QVector<ObjetObs *> liste_observation; // Contiendra dans l'ordre les objets avec leur moment d'observation
    moyTime = floor((debut.toTime_t()+fin.toTime_t())/2); // Heure à la moitié de la soirée

    QStringList planetes;
    planetes << "P1" << "P2" << "P3" << "P4" << "P5" << "P6" << "P7";

    QDateTime dateTime;
    dateTime.setTime_t(moyTime);

    if(boolPlanete) // Si on veut ajouter les planètes
    {
        for(int i(0); i < planetes.size(); i++)
        {
            emit generation(i+1);
            valeur = planetes.at(i);

            infosPlanete = Calculastro::coordonneesPlanetes(dateTime.date(),dateTime.time(),Calculastro::referencePlaneteToNom(valeur));
            if(infosPlanete[2] <= magMax) // On vérifie qu'on peut l'observer
            {
                HauteurMaxTemps hauteurMaxPla = Calculastro::hauteurMaxObjet(Calculastro::referencePlaneteToNom(valeur),debut,fin,lat,longi);
                obsPla[0] = hauteurMaxPla.temps().addSecs(-duree_une/2);
                obsPla[1] = hauteurMaxPla.temps().addSecs(duree_une/2);

                // ON COMPENSE LES TEMPS QUI DEPASSENT DE LA DUREE DE LA SOIREE
                if(hauteurMaxPla.temps().addSecs(-duree_une/2) < debut)
                { // Si on est au début
                    int compenser(0);
                    compenser = debut.toTime_t() - hauteurMaxPla.temps().addSecs(-duree_une/2).toTime_t();
                    obsPla[0] = obsPla[0].addSecs(compenser);
                    obsPla[1] = obsPla[1].addSecs(compenser);
                }
                else if(hauteurMaxPla.temps().addSecs(duree_une/2) > fin)
                { // Si on est au début
                    int compenser(0);
                    compenser = fin.toTime_t() - hauteurMaxPla.temps().addSecs(duree_une/2).toTime_t();
                    obsPla[0] = obsPla[0].addSecs(compenser);
                    obsPla[1] = obsPla[1].addSecs(compenser);
                }

                if(Calculastro::verifDisponibilite(liste_observation,obsPla[0],obsPla[1],espaceMin))
                {
                    if(hauteurMaxPla.hauteurMax() >= user->value("generateur/hauteurMin",HAUTEUR_MIN_OBJET).toInt()) {
                        liste_observation.push_back(new ObjetPlaneteObs(valeur,obsPla[0],obsPla[1]));
                    }
                }
                else
                {
                    int j(0);
                    double hau1(0), hau2(0);
                    QDateTime tempsPlus[2], tempsMoins[2], tempsPlace1[2], tempsPlace2[2];
                    tempsPlus[0] = obsPla[0].addSecs(60);
                    tempsPlus[1] = obsPla[1].addSecs(60);
                    tempsMoins[0] = obsPla[0].addSecs(-60);
                    tempsMoins[1] = obsPla[1].addSecs(-60);
                    bool tryV;

                    QVector<double> altazPlus, altazMoins;

                    while(j<719)
                    {
                        tryV = true;

                        altazPlus = Calculastro::hauteurAzimutDegree(tempsPlus[0].date(), tempsPlus[0].time(), infosPlanete[0], infosPlanete[1], lat, longi);
                        altazMoins = Calculastro::hauteurAzimutDegree(tempsMoins[0].date(), tempsMoins[0].time(), infosPlanete[0], infosPlanete[1], lat, longi);

                        if(altazPlus[0] > altazMoins[0])
                        { // On regarde à quel endroit doit être placé l'objet pour qu'il soit le plus haut
                            tempsPlace1[0] = tempsPlus[0];
                            tempsPlace1[1] = tempsPlus[1];

                            tempsPlace2[0] = tempsMoins[0];
                            tempsPlace2[1] = tempsMoins[1];

                            hau1 = altazPlus[0];
                            hau2 = altazMoins[0];
                        }
                        else
                        {
                            tempsPlace1[0] = tempsMoins[0];
                            tempsPlace1[1] = tempsMoins[1];

                            tempsPlace2[0] = tempsPlus[0];
                            tempsPlace2[1] = tempsPlus[1];

                            hau1 = altazMoins[0];
                            hau2 = altazPlus[0];
                        }
                        if(Calculastro::verifDisponibilite(liste_observation,tempsPlace1[0],tempsPlace1[1],espaceMin))
                        {
                            if(hau1 > user->value("generateur/hauteurMin",HAUTEUR_MIN_OBJET).toInt()) // Si on dépasse 10°
                            {
                                if(tempsPlace1[0] >= debut && tempsPlace1[0] <= fin && tempsPlace1[1] >= debut && tempsPlace1[1] <= fin)
                                {
                                    liste_observation.push_back(new ObjetPlaneteObs(valeur,tempsPlace1[0],tempsPlace1[1]));
                                    break;
                                }
                                else
                                    tryV = false;
                            }
                            else
                                tryV = false;
                        }
                        else
                            tryV = false;

                        if(!tryV && Calculastro::verifDisponibilite(liste_observation,tempsPlace2[0],tempsPlace2[1],espaceMin))
                        {
                            if(hau2 > user->value("generateur/hauteurMin",HAUTEUR_MIN_OBJET).toInt())
                            {
                                if(tempsPlace2[0] >= debut && tempsPlace2[0] <= fin && tempsPlace2[1] >= debut && tempsPlace2[1] <= fin)
                                {
                                    liste_observation.push_back(new ObjetPlaneteObs(valeur,tempsPlace2[0],tempsPlace2[1]));
                                    break;
                                }
                            }
                        }

                        tempsPlus[0] = tempsPlus[0].addSecs(60);
                        tempsPlus[1] = tempsPlus[1].addSecs(60);
                        tempsMoins[0] = tempsMoins[0].addSecs(-60);
                        tempsMoins[1] = tempsMoins[1].addSecs(-60);
                        j++;
                    }
                }
            }
        }
    }
    QString sql;
    if(constellation == "toutes")
        sql = "";
    else if(constellation.length() > 3)
        sql = " AND (constellation = '"+constellation.replace('|',"' OR constellation = '")+"')";
    else if(constellation.length() == 3)
        sql = " AND constellation = '"+constellation+"'";

    // mettre un else ici pour gérer le cas des constellations multiples

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost");
    db.setDatabaseName("dbastrogenerator");
    db.setUserName("univers2");
    db.setPassword("iwxldmkdgpf");
    db.open();

    QMap<double,QString> objets_visibles1;
    ObjetCP *objet(0);

    HauteurMaxTemps hauteurMax2;

    QSqlQuery requeteCount("SELECT COUNT(*) as nbr FROM ngcic WHERE type <> '' AND magnitude <> 0 AND taille > 0 AND interet > 2"+sql);
    requeteCount.next();
    int nbResult(0);
    nbResult = requeteCount.value(0).toInt();

    QSqlQuery requete("SELECT nom, reference, type, ascdr, declinaison, constellation, magnitude, messier, interet, taille, difficulte FROM ngcic WHERE type <> '' AND magnitude <> 0 AND taille > 0 AND interet > 2"+sql+" ORDER BY reference");

    while(requete.next())
    {
        emit generation(7 + requete.at()*25/nbResult);

        objet = new ObjetCP(requete.value(1).toString());
        hauteurMax2 = Calculastro::hauteurMaxObjet(objet, debut, fin, lat, longi);

        if(hauteurMax2.hauteurMax() > hauteurMin)
        {
            note = Calculastro::noterObjetVisible(requete.value(2).toString(), requete.value(8).toInt(), requete.value(6).toDouble(), diametre, niveau, hauteurMax2.hauteurMax(), requete.value(10).toInt(), user); // on calcule sa note
            objets_visibles1.insert(note,requete.value(1).toString());
        }
    }

    QMapIterator<double,QString> iterateur(objets_visibles1);

    iterateur.toBack(); // On parcourt le tableau en sens inverse (note décroissante)

    int k(0);
    while(iterateur.hasPrevious())
    {
        k++;
        emit generation(32 + k*68/objets_visibles1.count());

        iterateur.previous();

        if(liste_observation.count() >= nb_objets) // Si on a placé assez d'objet, on arrête
            break;

        ObjetCP *objet = new ObjetCP(iterateur.value());
        raDecimal = objet->ascdrDouble();
        declinaisonDecimal = objet->decDouble();

        HauteurMaxTemps hauteurMaxD = Calculastro::hauteurMaxObjet(objet,debut,fin,lat,longi);
        QDateTime tempsObs[2];
        tempsObs[0] = hauteurMaxD.temps().addSecs(-duree_une/2);
        tempsObs[1] = hauteurMaxD.temps().addSecs(duree_une/2);

        // ON COMPENSE LES TEMPS QUI DEPASSENT DE LA DUREE DE LA SOIREE
        if(hauteurMaxD.temps().addSecs(-duree_une/2) < debut)
        { // Si on est au début
            int compenser(0);
            compenser = debut.toTime_t() - hauteurMaxD.temps().addSecs(-duree_une/2).toTime_t();
            tempsObs[0] = tempsObs[0].addSecs(compenser);
            tempsObs[1] = tempsObs[1].addSecs(compenser);
        }
        else if(hauteurMaxD.temps().addSecs(duree_une/2) > fin)
        { // Si on est au début
            int compenser(0);
            compenser = fin.toTime_t() - hauteurMaxD.temps().addSecs(duree_une/2).toTime_t();
            tempsObs[0] = tempsObs[0].addSecs(compenser);
            tempsObs[1] = tempsObs[1].addSecs(compenser);
        }

        if(Calculastro::verifDisponibilite(liste_observation,tempsObs[0],tempsObs[1],espaceMin))
        {
            if(Calculastro::isObjetVisible(objet,tempsObs[0],lat,longi) && Calculastro::isObjetVisible(objet,tempsObs[1],lat,longi))
                liste_observation.push_back(new ObjetCPObs(objet,tempsObs[0],tempsObs[1]));
        }
        else
        {
            int i = 0;
            QDateTime tempsPlus[2], tempsMoins[2], tempsPlace1[2], tempsPlace2[2];
            double hau1(0), hau2(0);
            tempsPlus[0] = tempsObs[0].addSecs(60);
            tempsPlus[1] = tempsObs[1].addSecs(60);
            tempsMoins[0] = tempsObs[0].addSecs(-60);
            tempsMoins[1] = tempsObs[1].addSecs(-60);

            QVector<double> altazPlus, altazMoins;
            bool tryVar(true);

            while(i<rep)
            {
                altazPlus = Calculastro::hauteurAzimutDegree(tempsPlus[0].date(), tempsPlus[0].time(), raDecimal, declinaisonDecimal, lat , longi);
                altazMoins = Calculastro::hauteurAzimutDegree(tempsMoins[0].date(), tempsMoins[0].time(), raDecimal, declinaisonDecimal, lat , longi);
                tryVar = true;

                if(altazPlus[0] > altazMoins[0])
                {
                    tempsPlace1[0] = tempsPlus[0];
                    tempsPlace1[1] = tempsPlus[1];

                    tempsPlace2[0] = tempsMoins[0];
                    tempsPlace2[1] = tempsMoins[1];

                    hau1 = altazPlus[0];
                    hau2 = altazMoins[0];
                }
                else
                {
                    tempsPlace1[0] = tempsMoins[0];
                    tempsPlace1[1] = tempsMoins[1];

                    tempsPlace2[0] = tempsPlus[0];
                    tempsPlace2[1] = tempsPlus[1];

                    hau1 = altazMoins[0];
                    hau2 = altazPlus[0];
                }
                if(Calculastro::verifDisponibilite(liste_observation,tempsPlace1[0],tempsPlace1[1],espaceMin))
                {
                    if(hau1 > hauteurMin) // Si on dépasse $hauteurMin°
                    {
                        if(tempsPlace1[0] >= debut && tempsPlace1[0] <= fin && tempsPlace1[1] >= debut && tempsPlace1[1] <= fin)
                        {
                            liste_observation.push_back(new ObjetCPObs(objet, tempsPlace1[0], tempsPlace1[1]));
                            break;
                        }
                        else
                            tryVar = false;
                    }
                    else
                        tryVar = false;
                }
                else
                    tryVar = false;
                if(!tryVar && Calculastro::verifDisponibilite(liste_observation,tempsPlace2[0],tempsPlace2[1],espaceMin))
                {
                    if(hau2 > hauteurMin)
                    {
                        if(tempsPlace2[0] >= debut && tempsPlace2[0] <= fin && tempsPlace2[1] >= debut && tempsPlace2[1] <= fin)
                        {
                            liste_observation.push_back(new ObjetCPObs(objet,tempsPlace2[0],tempsPlace2[1]));
                            break;
                        }
                    }
                }
                tempsPlus[0] = tempsPlus[0].addSecs(60);
                tempsPlus[1] = tempsPlus[1].addSecs(60);
                tempsMoins[0] = tempsMoins[0].addSecs(-60);
                tempsMoins[1] = tempsMoins[1].addSecs(-60);

                if(tempsPlus[1] > fin) // Si on a atteint la fin
                {
                    tempsPlus[1] = fin;
                    tempsPlus[0] = fin.addSecs(-duree_une);
                }
                if(tempsMoins[0] < debut)
                {
                    tempsMoins[0] = debut;
                    tempsMoins[1] = debut.addSecs(duree_une);
                }
                i++;
            } // Au bout de 720 fois, la boucle s'arrête
        }
    }
    liste_observation = trierPlanning(liste_observation);
    if(liste_observation.size() > 0) // Si il n'y a aucun objet à observer
    {
        m_boolPlanete = boolPlanete;
        m_debut = debut;
        m_diametre = diametre;
        m_fin = fin;
        m_focale = focale;
        m_lat = lat;
        m_longi = longi;
        m_niveau = niveau;
        m_listeObjets = liste_observation;
        m_listeObjetsOld = liste_observation;
        m_file = "";
    }
    // on crée le tableau avec $liste_observation et les autres informations
    // on retourne le tableau !
}
QVector<ObjetObs *> Soiree::trierPlanning(QVector<ObjetObs *> planning)
{
    QVectorIterator<ObjetObs *> i(planning);
    QVector<ObjetObs *> newPlanning;
    int position(0);

    while(planning.size() > 0)
    {
        position = trouverPlusPetit(planning);
        i.findNext(planning[position]);
        newPlanning.push_back(planning[position]);
        planning.erase(planning.begin()+position);
    }
    return newPlanning;
}
int Soiree::trouverPlusPetit(const QVector<ObjetObs *> &planning)
{
    int temp(0);
    for(int i(0); i < planning.size(); i++)
    {
        if(planning[i]->getDebut() < planning[temp]->getDebut())
            temp = i;
    }
    return temp;
}
void Soiree::setBoolPlanete(bool planete)
{
    m_boolPlanete = planete;
}
void Soiree::setDebut(QDateTime debut)
{
    if(debut.isValid() && !debut.isNull())
        m_debut = debut;
}
void Soiree::setDiametre(unsigned int diametre)
{
    if(diametre >= DIAMETRE_MINIMUM && diametre <= DIAMETRE_MAXIMUM)
        m_diametre = diametre;
}
void Soiree::setFin(QDateTime fin)
{
    if(fin.isValid() && !fin.isNull())
        m_fin = fin;
}
void Soiree::setFocale(unsigned int focale)
{
    if(focale >= FOCALE_MINIMUM && focale <= FOCALE_MAXIMUM)
        m_focale = focale;
}
void Soiree::setLat(double lat)
{
    m_lat = lat;
}
void Soiree::setLongi(double longi)
{
    m_longi = longi;
}
void Soiree::setNiveau(QString niveau)
{
    m_niveau = niveau;
}
void Soiree::setPlanning(QVector<ObjetObs *> liste)
{
    if(liste.size() > 0)
        m_listeObjets = liste;
}
void Soiree::setPlanningOld(QVector<ObjetObs *> liste)
{
    if(liste.size() > 0)
        m_listeObjetsOld = liste;
}
void Soiree::setFile(QString file)
{
    m_file = file;
}
QString Soiree::getFile() const
{
    return m_file;
}
QDateTime Soiree::getDebut() const
{
    return m_debut;
}
QDateTime Soiree::getFin() const
{
    return m_fin;
}
int Soiree::getDureeUne() const
{
    return m_duree_une;
}
double Soiree::getLat() const
{
    return m_lat;
}
double Soiree::getLongi() const
{
    return m_longi;
}
QString Soiree::getNiveau() const
{
    return m_niveau;
}
int Soiree::getDiametre() const
{
    return m_diametre;
}
int Soiree::getFocale() const
{
    return m_focale;
}
bool Soiree::getBoolPlanete() const
{
    return m_boolPlanete;
}
QVector<ObjetObs *> Soiree::getPlanning() const
{
    return m_listeObjets;
}
QVector<ObjetObs *> Soiree::getPlanningOld() const
{
    return m_listeObjetsOld;
}
void Soiree::supprimerObjet(int index)
{
    if(index >= 0 && index < m_listeObjets.size())
        m_listeObjets.erase(m_listeObjets.begin()+index);
    else
        QMessageBox::critical(0,tr("Suppression impossible"),tr("L'index fourni pour la supression est invalide"));
}
QString Soiree::getPays() const
{
    if(m_pays == "")
        return COORDONNEES_INCONNU;
    else
        return m_pays;
}
QString Soiree::getVille() const
{
    if(m_ville == "")
        return COORDONNEES_INCONNU;
    else
        return m_ville;
}
void Soiree::setVille(QString ville)
{
    m_ville = ville;
}
void Soiree::setPays(QString pays)
{
    m_pays = pays;
}
void Soiree::monterObjet(int index)
{
    if(index <= m_listeObjets.size()-1 && index > 0)    // On vérifie si l'index rentre bien dans le planning
    {
        QDateTime debutP, debutF, finP, finF;
        debutP = m_listeObjets[index-1]->getDebut();
        finP = m_listeObjets[index-1]->getFin();
        debutF = m_listeObjets[index]->getDebut();
        finF = m_listeObjets[index]->getFin();

        m_listeObjets[index-1]->setFin(finF);
        m_listeObjets[index]->setDebut(debutP);
        m_listeObjets[index]->setFin(debutP.addSecs(finF.toTime_t()-debutF.toTime_t()));
        m_listeObjets[index-1]->setDebut(finF.addSecs(-(finP.toTime_t()-debutP.toTime_t())));

        m_listeObjets = trierPlanning(m_listeObjets);
    }
    else
        QMessageBox::critical(0,tr("Erreur de modification"),tr("L'index de modification fournie est invalide"));
}
void Soiree::descendreObjet(int index)
{
    // Si on veut descendre index, ça revient à monter index+1
    monterObjet(index+1);
}
void Soiree::modifierDuree(int index, int duree)
{
    if(index >= 0 && index < m_listeObjets.size())
    {
        if(duree <= DUREE_OBJET_MAX && duree >= DUREE_OBJET_MIN)
        {
            int dif(0); // En secondes
            dif = (duree*60) - (m_listeObjets[index]->getFin().toTime_t() - m_listeObjets[index]->getDebut().toTime_t());
            m_listeObjets[index]->setFin(m_listeObjets[index]->getDebut().addSecs(duree*60));
            for(int i(index+1);i < m_listeObjets.size();i++)
            {
                m_listeObjets[i]->setDebut(m_listeObjets[i]->getDebut().addSecs(dif));
                m_listeObjets[i]->setFin(m_listeObjets[i]->getFin().addSecs(dif));
            }
        }
        else
            QMessageBox::critical(0,tr("Erreur de modification"),tr("Il est impossible de modifier l'objet avec la durée indiquée : %n min. Elle doit être comprise entre 1 et 20 min.","",duree));
    }
    else
        QMessageBox::critical(0,tr("Erreur de modification"),tr("L'index fourni pour la modification est invalide"));
}
void Soiree::ajouterObjet(int index, ObjetCP* objetParam, int duree)
{ // INDEX : index de l'objet précédent REFERENCE : pour instancier Objet DUREE : duree de l'objet

    QSettings *user = new QSettings(NOM_EQUIPE,NOM_PROGRAMME);
    if(index >= 0 && index < m_listeObjets.size() && indexFromRef(objetParam->ref()) == -1) // On vérifie qu'on a un index correct et que l'objet n'est pas dans la soirée
    {
        QVector<double> hauteurAzimut;
        QDateTime debutObs, finObs;

        debutObs = m_listeObjets[index]->getFin().addSecs(user->value("generateur/pauseMin",TEMPS_ESPACE).toInt()*60);
        finObs = debutObs.addSecs(duree*60);

        ObjetCPObs *objet = new ObjetCPObs(objetParam, debutObs, finObs);
        if(objet->isValid())
        {
            hauteurAzimut = Calculastro::hauteurAzimutDegree(debutObs.date(),debutObs.time(),objet->ascdrDouble(),objet->decDouble(),m_lat,m_longi);
            if(hauteurAzimut[0] > user->value("generateur/hauteurMin",HAUTEUR_MIN_OBJET).toInt())
            {
                for(int i(index+1); i<m_listeObjets.size();i++)
                {
                    m_listeObjets[i]->setDebut(m_listeObjets[i]->getDebut().addSecs(duree*60 + user->value("generateur/pauseMin",TEMPS_ESPACE).toInt()*60));
                    m_listeObjets[i]->setFin(m_listeObjets[i]->getFin().addSecs(duree*60 +user->value("generateur/pauseMin",TEMPS_ESPACE).toInt()*60));
                }
                m_listeObjets.push_back(objet);
                m_listeObjets = trierPlanning(m_listeObjets);
            }
            else
                QMessageBox::critical(0,tr("Ajout impossible"),tr("Il est impossible d'ajouter l'objet car il est trop bas sur l'horizon. Minimum 10°"));
        }
        else
            QMessageBox::critical(0,tr("Ajout impossible"),tr("L'objet demandé est introuvable"));

    }
    else
        QMessageBox::critical(0,tr("Erreur d'ajout"),tr("L'index fourni pour l'ajout de l'objet est invalide."));
}
void Soiree::ajouterObjet(int index, QString refPlanete, int duree)
{
    QSettings *user = new QSettings(NOM_EQUIPE,NOM_PROGRAMME);
    if(index >= 0 && index < m_listeObjets.size() && indexFromRef(refPlanete) == -1) // On vérifie qu'on a un index correct et que l'objet n'est pas dans la soirée
    {
        QVector<double> hauteurAzimut;
        QDateTime debutObs, finObs;

        debutObs = m_listeObjets[index]->getFin().addSecs(user->value("generateur/pauseMin",TEMPS_ESPACE).toInt()*60);
        finObs = debutObs.addSecs(duree*60);

        ObjetPlaneteObs *objet = new ObjetPlaneteObs(refPlanete,debutObs,finObs);
        if(objet->isValid())
        {
            hauteurAzimut = Calculastro::hauteurAzimutDegree(debutObs.date(),debutObs.time(),objet->ascdrDouble(),objet->decDouble(),m_lat,m_longi);
            if(hauteurAzimut[0] > user->value("generateur/hauteurMin",HAUTEUR_MIN_OBJET).toInt())
            {
                for(int i(index+1); i<m_listeObjets.size();i++)
                {
                    m_listeObjets[i]->setDebut(m_listeObjets[i]->getDebut().addSecs(duree*60 + user->value("generateur/pauseMin",TEMPS_ESPACE).toInt()*60));
                    m_listeObjets[i]->setFin(m_listeObjets[i]->getFin().addSecs(duree*60 +user->value("generateur/pauseMin",TEMPS_ESPACE).toInt()*60));
                }
                m_listeObjets.push_back(objet);
                m_listeObjets = trierPlanning(m_listeObjets);
            }
            else
                QMessageBox::critical(0,tr("Ajout impossible"),tr("Il est impossible d'ajouter l'objet car il est trop bas sur l'horizon."));
        }
        else
            QMessageBox::critical(0,tr("Ajout impossible"),tr("L'objet demandé est introuvable"));
    }
    else
        QMessageBox::critical(0,tr("Erreur d'ajout"),tr("L'index fourni pour l'ajout de l'objet est invalide."));
}
void Soiree::ajouterObjet(ObjetObs *objet)
{
    QSettings *user = new QSettings(NOM_EQUIPE,NOM_PROGRAMME);
    if(objet->getDebut() >= m_debut && objet->getDebut() < m_fin && objet->getFin() > m_debut && objet->getFin() <= m_fin)
    { // On vérifie qu'il rentre bien dans la soirée

        bool ok(true);
        for(int i(0); i < m_listeObjets.count(); i++)
        {
            if(objet->getDebut() > m_listeObjets.at(i)->getDebut().addSecs(-user->value("generateur/pauseMin",TEMPS_ESPACE).toInt()*60) && objet->getDebut() < m_listeObjets.at(i)->getFin().addSecs(user->value("generateur/pauseMin",TEMPS_ESPACE).toInt()*60))
                ok = false;
            else if(objet->getFin() > m_listeObjets.at(i)->getDebut().addSecs(-user->value("generateur/pauseMin",TEMPS_ESPACE).toInt()*60) && objet->getFin() < m_listeObjets.at(i)->getFin().addSecs(user->value("generateur/pauseMin",TEMPS_ESPACE).toInt()*60))
                ok = false;
        }
        if(ok && objet->isValid())
        { // On vérifie qu'il n'y a pas d'autres objets à cet endroit
            QVector<double> hauteurAzimut;
            hauteurAzimut = Calculastro::hauteurAzimutDegree(objet->getDebut().date(),objet->getDebut().time(),objet->ascdrDouble(),objet->decDouble(),m_lat,m_longi);
            if(hauteurAzimut[0] > user->value("generateur/hauteurMin",HAUTEUR_MIN_OBJET).toInt())
            {
                m_listeObjets.push_back(objet);
                m_listeObjets = trierPlanning(m_listeObjets);
            }
            else
                QMessageBox::critical(0,tr("Erreur"),tr("L'objet n'est pas assez haut au moment demandé pour pouvoir être ajouté dans la soirée."));
        }
        else
            QMessageBox::critical(0,tr("Erreur"),tr("L'objet n'est pas valide ou ne peut être placé ici"));
    }
    else
        QMessageBox::critical(0,tr("Erreur"),tr("La date de l'objet ne correspond pas avec la date de la soirée."));
}

QStandardItemModel* Soiree::toModele() const
{
    QStandardItemModel *modele = new QStandardItemModel;
    QStandardItem *reference(0), *reference2(0), *reference3(0), *reference4(0), *reference5(0), *reference6(0), *reference7(0), *reference8(0), *reference9(0);
    for(int i(0); i < m_listeObjets.count();i++)
    {
        reference = new QStandardItem(m_listeObjets.at(i)->nomComplet());
        reference2 = new QStandardItem(m_listeObjets.at(i)->ref());
        reference3 = new QStandardItem(m_listeObjets.at(i)->ascdr());
        reference4 = new QStandardItem(m_listeObjets.at(i)->declinaison());
        reference5 = new QStandardItem(QString::number(m_listeObjets.at(i)->magnitude()));
        reference6 = new QStandardItem(Calculastro::abreviationToNom(m_listeObjets.at(i)->constellation()));
        reference7 = new QStandardItem(m_listeObjets.at(i)->difficulte(true));
        reference8 = new QStandardItem(m_listeObjets.at(i)->interet(true));
        reference9 = new QStandardItem(m_listeObjets.at(i)->getDebut().toLocalTime().time().toString(tr("hh:mm")) + " à " + m_listeObjets.at(i)->getFin().toLocalTime().time().toString(tr("hh:mm")));

        modele->setItem(i,0,reference);
        modele->setItem(i,1,reference2);
        modele->setItem(i,2,reference3);
        modele->setItem(i,3,reference4);
        modele->setItem(i,4,reference5);
        modele->setItem(i,5,reference6);
        modele->setItem(i,6,reference7);
        modele->setItem(i,7,reference8);
        modele->setItem(i,8,reference9);

        reference->appendRow(new QStandardItem(m_listeObjets.at(i)->ref()));
    }
    modele->setHeaderData(0, Qt::Horizontal, tr("Nom"));
    modele->setHeaderData(1, Qt::Horizontal, tr("Référence"));
    modele->setHeaderData(2, Qt::Horizontal, tr("Ascension droite"));
    modele->setHeaderData(3, Qt::Horizontal, tr("Déclinaison"));
    modele->setHeaderData(4, Qt::Horizontal, tr("Magnitude"));
    modele->setHeaderData(5, Qt::Horizontal, tr("Constellation"));
    modele->setHeaderData(6, Qt::Horizontal, tr("Difficulté"));
    modele->setHeaderData(7, Qt::Horizontal, tr("Interêt"));
    modele->setHeaderData(8, Qt::Horizontal, tr("Heure d'observation"));

    return modele;
}
Soiree* Soiree::soaToSoiree(QString const& fileName)
{
    QFile soa(fileName);
    if(soa.open(QIODevice::ReadOnly))
    {
        QTextStream flux(&soa);
        QVector<ObjetObs *> planning;
        QVector<double> oculairesVector;
        QString infosSoiree, ligne, oculaires;
        QStringList listInfosSoiree, listInfosObjet, listOculaires;
        QDateTime debut, fin;
        infosSoiree = flux.readLine();
        listInfosSoiree = infosSoiree.split("|");

        if(listInfosSoiree.count() != 9)
        {
            QMessageBox::critical(0,tr("Fichier incorrect"),tr("Le fichier demandé est incorrect"));
            return new Soiree;
        }
        else
        {
            oculaires = flux.readLine();
            listOculaires = oculaires.split("|");
            for(int i(0);i<listOculaires.count();i++)
            {
                oculairesVector.push_back(listOculaires.at(i).toDouble());
            }
            while(!flux.atEnd())
            {
                ligne = flux.readLine();
                listInfosObjet = ligne.split("|");
                if(listInfosObjet.count() != 3)
                {
                    QMessageBox::critical(0,tr("Fichier incorrect"),tr("Le fichier demandé est incorrect"));
                    return new Soiree;
                }
                if(listInfosObjet.at(0).left(1) != "P")
                {
                    debut.setTime_t(listInfosObjet.at(1).toInt());
                    fin.setTime_t(listInfosObjet.at(2).toInt());

                    planning.push_back(new ObjetCPObs(listInfosObjet[0],debut,fin));
                }
                else
                {
                    debut.setTime_t(listInfosObjet.at(1).toInt());
                    fin.setTime_t(listInfosObjet.at(2).toInt());

                    planning.push_back(new ObjetPlaneteObs(listInfosObjet[0],debut,fin));
                }
            }

            Soiree *soiree = new Soiree;
            soiree->setBoolPlanete(false);
            QDateTime debutSoiree;
            debutSoiree.setTime_t(listInfosSoiree.at(5).toInt());
            soiree->setDebut(debutSoiree);
            QDateTime finSoiree;
            finSoiree.setTime_t(listInfosSoiree.at(6).toInt());
            soiree->setFin(finSoiree);
            soiree->setDiametre(listInfosSoiree.at(7).toInt());
            soiree->setFocale(listInfosSoiree.at(8).toInt());
            soiree->setLat(listInfosSoiree.at(3).toDouble());
            soiree->setLongi(listInfosSoiree.at(4).toDouble());

            if(listInfosSoiree.at(2) == "pays")
                soiree->setPays(COORDONNEES_INCONNU);
            else
                soiree->setPays(listInfosSoiree.at(2));
            if(listInfosSoiree.at(1) == "ville")
                soiree->setVille(COORDONNEES_INCONNU);
            else
                soiree->setVille(listInfosSoiree.at(1));

            soiree->setNiveau(QString("debutant"));
            soiree->setPlanning(planning);
            soiree->setPlanningOld(planning);
            soiree->setFile(fileName);

            ajouterSoireeRecentes(fileName);
            return soiree;
        }
    }
    else
    {
        QMessageBox::critical(0,tr("Erreur d'ouverture"),tr("Le fichier demandé est impossible à ouvrir : ","Suivi du nom du fichier") + fileName);
        Soiree *soiree = new Soiree;
        return soiree;
    }
}
bool Soiree::soireeToSoa(const QString &fileName)
{
    QSettings *user = new QSettings(NOM_EQUIPE,NOM_PROGRAMME);
    QStringList oculaires = user->value("oculaires").toString().split("|");

    QString ligne1, ligne2;
    ligne1 = "cle|"+getVille()+"|"+getPays()+"|"+QString::number(m_lat)+"|"+QString::number(m_longi)+"|"+QString::number(m_debut.toTime_t())+"|"+QString::number(m_fin.toTime_t())+"|"+QString::number(m_diametre)+"|"+QString::number(m_focale);
    for(int i(0); i < oculaires.size(); i++)
    {
        ligne2 += oculaires.at(i)+"|";
    }
    ligne2 = ligne2.left(ligne2.count()-1);
    QFile soa(fileName);
    if(soa.open(QIODevice::WriteOnly))
    {
        QTextStream flux(&soa);
        flux.setCodec("UTF-8");
        flux << ligne1 << Qt::endl << ligne2 << Qt::endl;

        for(int j(0); j < m_listeObjets.count(); j++)
        {
            flux << m_listeObjets.at(j)->ref() << "|" << QString::number(m_listeObjets.at(j)->getDebut().toTime_t()) << "|" << QString::number(m_listeObjets.at(j)->getFin().toTime_t()) << Qt::endl;
        }
        soa.close();
        return true;
    }
    else
        return false;
}
int Soiree::indexFromRef(QString ref) const
{
    for(int i(0); i < m_listeObjets.count(); i++)
    {
        if(m_listeObjets.at(i)->ref() == ref)
        {
            return i;
        }
    }
    return -1;
}
bool Soiree::hasChanged()
{
    if(m_listeObjets.count() != m_listeObjetsOld.count())
        return true;
    else
    {
        for(int i(0); i < m_listeObjets.count();i++)
        {
            if(m_listeObjets.at(i) != m_listeObjetsOld.at(i))
                return true;
        }
        return false;
    }
}
void Soiree::toXML() const
{
    QLocale locale;
    QDomDocument doc;
    QDomProcessingInstruction instr =  doc.createProcessingInstruction("xml", "version=\"1.0\"  encoding=\"UTF-8\"");
    doc.appendChild(instr);

    QDomElement soiree = doc.createElement("soiree");
    doc.appendChild(soiree);
    QDomElement date = doc.createElement("date");
    date.appendChild(doc.createTextNode(locale.toString(m_debut.date(), QLocale::ShortFormat)));
    soiree.appendChild(date);

    QDomElement heure = doc.createElement("heure");
    heure.appendChild(doc.createTextNode("De " + locale.toString(m_debut.time(), QLocale::ShortFormat) + " à " + locale.toString(m_fin.time(), QLocale::ShortFormat)));
    soiree.appendChild(heure);

    QDomElement listeObjet = doc.createElement("listeObject");
    for(int i(0); i < m_listeObjets.count(); i++)
    {
        QDomElement objet = doc.createElement("objet"+QString::number(i+1));
        QDomElement nom = doc.createElement("nom");
        nom.appendChild(doc.createTextNode(m_listeObjets.at(i)->nomComplet()));
        objet.appendChild(nom);

        QDomElement horaire = doc.createElement("horaire");
        horaire.appendChild(doc.createTextNode("Entre " + locale.toString(m_listeObjets.at(i)->getDebut().time(), QLocale::ShortFormat) + " et "
                                        + locale.toString(m_listeObjets.at(i)->getFin().time(), QLocale::ShortFormat)));
        objet.appendChild(horaire);

        QDomElement element = doc.createElement("infos");
        QDomElement ad = doc.createElement("ad");
        ad.appendChild(doc.createTextNode(m_listeObjets.at(i)->ascdr()));
        element.appendChild(ad);

        QDomElement dec = doc.createElement("dec");
        dec.appendChild(doc.createTextNode(m_listeObjets.at(i)->declinaison()));
        element.appendChild(dec);

        QDomElement magnitude = doc.createElement("magnitude");
        magnitude.appendChild(doc.createTextNode(QString::number(m_listeObjets.at(i)->magnitude())));
        element.appendChild(magnitude);

        QDomElement interet = doc.createElement("interet");
        interet.appendChild(doc.createTextNode(m_listeObjets.at(i)->interet(true)));
        element.appendChild(interet);

        QDomElement taille = doc.createElement("taille");
        taille.appendChild(doc.createTextNode(QString::number(m_listeObjets.at(i)->taille())+"'"));
        element.appendChild(taille);

        QDomElement type = doc.createElement("type");
        type.appendChild(doc.createTextNode(m_listeObjets.at(i)->type()));
        element.appendChild(type);

        QDomElement constellation = doc.createElement("constellation");
        constellation.appendChild(doc.createTextNode(Calculastro::abreviationToNom(m_listeObjets.at(i)->constellation())));
        element.appendChild(constellation);
        objet.appendChild(element);

        listeObjet.appendChild(objet);
    }
    soiree.appendChild(listeObjet);

    QString nomFichier = QFileDialog::getSaveFileName(0,tr("Sauver la soirée au format XML"),QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)+"/soiree.xml","Extensible Markup Language (*.xml)");
    if(nomFichier != "")
    {
        QFile file(nomFichier);
        if(file.open(QIODevice::WriteOnly))
        {
            QTextStream flux(&file);
            flux.setCodec("UTF-8");
            flux << doc.toString();
            file.close();
        }
        else
            file.close();
    }
}
bool Soiree::paintPdf(QPrinter *printer)
{
    qreal mT(7),mG(7),mD(7);
    const auto margins = printer->pageLayout().margins(QPageLayout::Millimeter);
    mT = margins.top();
    mG = margins.left();
    mD = margins.right();

    if(printer->isValid())
    {
        QSettings *user = new QSettings(NOM_EQUIPE,NOM_PROGRAMME);
        // ON CREE LE PDF
        double k(printer->resolution()/25.4); // On crée le facteur de positionnement pour la résolution
        QPainter painter(printer);
        if(painter.isActive())
        {
            QStringList listeConseils;
            listeConseils << tr("pensez à régler votre télescope") << tr("profitez-en pour prendre une boisson une chaude") << tr("profitez-en pour prendre un goûter") << tr("observez le ciel à l'oeil nu pendant ce temps") << tr("remplissez la fiche de note avec vos impressions d'observations") << tr("si besoin, réalignez votre chercheur");

            QVector<int> oculairesInt;
            QStringList oculairesString = user->value("oculaires",OCULAIREES_DEFAUT).toString().split("|");
            for(int l(0);l<oculairesString.size();l++)
                oculairesInt.push_back(oculairesString.at(l).toInt());

            // PAGE DE GARDE
            QRectF rect;
            painter.drawImage(52*k-mG*k,101*k-mT*k,QImage("images/bandeau-pdf"));
            QFont font = painter.font();
            font.setPointSize(7*k);
            font.setFamily("Arial");
            font.setWeight(QFont::Bold);
            painter.setFont(font);
            // On écrit le titre de la soirée
            painter.drawText(QRectF(52.1*k-mG*k,132.5*k-mT*k,105*k,20*k),Qt::AlignCenter,tr("Soirée d'observation\n")+m_debut.toLocalTime().toString("d")+" "+Calculastro::monthNumToName(m_debut.toLocalTime().toString("M").toInt())+" "+m_debut.toLocalTime().toString("yyyy"));
            // On écrit l'horaire
            font.setWeight(QFont::Normal);
            font.setPointSize(3*k);
            painter.setFont(font);
            painter.drawText(QRectF(52.1*k-mG*k,152.5*k-mT*k,105*k,7*k),Qt::AlignCenter,tr("De ")+m_debut.toLocalTime().toString(tr("hh'h'mm"))+tr(" à ")+m_fin.toLocalTime().toString(tr("hh'h'mm")));
            // On écrit le nombre d'objets
            painter.drawText(QRectF(52.1*k-mG*k,159.5*k-mT*k,105*k,7*k),Qt::AlignCenter,QString::number(m_listeObjets.size())+" "+tr("objets"));
            // On écrit la ville et le pays
            painter.drawText(QRectF(0,250*k-mT*k,(210-mD-mG)*k,7*k),Qt::AlignRight,tr("Près de ")+m_ville+", "+m_pays);
            // On écrit les coordonnées
            painter.drawText(QRectF(0,257*k-mT*k,(210-mD-mG)*k,7*k),Qt::AlignRight,Calculastro::degreeToDms(m_lat)+" / "+Calculastro::degreeToDms(m_longi));
            // On écrit le numéro de page
            font.setPointSize(2*k);
            painter.setFont(font);
            painter.drawText(QRectF(0,0,(210-mD-mG)*k,7*k),Qt::AlignRight,"1");

            // PAGE 2
            printer->newPage();
            // On écrit le numéro de page
            font.setPointSize(2*k);
            painter.setFont(font);
            painter.drawText(QRectF(0,0,(210-mD-mG)*k,7*k),Qt::AlignRight,"2");
            // On écrit l'intro
            font.setPointSize(3.5*k);
            painter.setFont(font);
            painter.drawText(QRectF(13*k-mG*k,17*k-mT*k,(210-mD-mG)*k,25*k),Qt::AlignJustify,tr("Avec les informations que vous avez rentrées, nous avons préparé un dossier avec toutes les\ninformations pour réussir votre soirée. Dans ce dossier vous trouverez beaucoup d'infos : une\nliste de conseils, le planning d'observation, une carte du ciel et enfin une page de notes.\nCe dossier est destiné à être imprimé. Bonne observation !"));
            // On écrit le titre
            font.setPointSize(6*k);
            font.setWeight(QFont::Bold);
            painter.setFont(font);
            painter.setPen(QColor(36,42,112));
            painter.drawImage(QRectF(13*k-mG*k,45*k-mT*k,7*k,7*k),QImage("images/title-pdf.png"));
            painter.drawText(QRectF(22*k-mG*k,45*k-mT*k,(210-mD-mG)*k,10*k),Qt::AlignLeft,tr("Checklist"));
            painter.drawLine(13*k-mG*k,55*k-mT*k,(210-mD-mG-13)*k,55*k-mT*k);
            // On écrit le texte de la checklist
            font.setPointSize(3*k);
            font.setWeight(QFont::Normal);
            painter.setFont(font);
            painter.setPen(QColor(0,0,0));
            painter.drawText(QRectF(13*k-mG*k,59*k-mT*k,(210-mD-mG)*k,5*k),Qt::AlignLeft,tr("Avant de commencer à observer, pensez à prendre les objets suivants (cochez quand c'est fait) : "));
            // On écrit les items
            painter.drawRect(QRectF(15*k,68*k-mT*k,5*k,5*k));
            painter.drawText(QRectF(22*k,68*k-mT*k,(210-mD-mG)*k,5*k),Qt::AlignLeft,tr("Une lampe rouge"));

            painter.drawRect(QRectF(15*k,75*k-mT*k,5*k,5*k));
            painter.drawText(QRectF(22*k,75*k-mT*k,(210-mD-mG)*k,5*k),Qt::AlignLeft,tr("Un laser"));

            painter.drawRect(QRectF(15*k,82*k-mT*k,5*k,5*k));
            painter.drawText(QRectF(22*k,82*k-mT*k,(210-mD-mG)*k,5*k),Qt::AlignLeft,tr("Des vêtements chauds"));

            painter.drawRect(QRectF(15*k,89*k-mT*k,5*k,5*k));
            painter.drawText(QRectF(22*k,89*k-mT*k,(210-mD-mG)*k,5*k),Qt::AlignLeft,tr("Une boisson chaude (ex : café)"));

            painter.drawRect(QRectF(15*k,96*k-mT*k,5*k,5*k));
            painter.drawText(QRectF(22*k,96*k-mT*k,(210-mD-mG)*k,5*k),Qt::AlignLeft,tr("Un goûter"));

            painter.drawRect(QRectF(15*k,103*k-mT*k,5*k,5*k));
            painter.drawText(QRectF(22*k,103*k-mT*k,(210-mD-mG)*k,5*k),Qt::AlignLeft,tr("Un stylo (pour noter vos observations)"));

            painter.drawRect(QRectF(15*k,110*k-mT*k,5*k,5*k));
            painter.drawText(QRectF(22*k,110*k-mT*k,(210-mD-mG)*k,5*k),Qt::AlignLeft,tr("Les accessoires du télescope (oculaires, barlow, filtres etc.)"));
            // On écrit l'autre titre (planning d'observation de la soirée)
            font.setPointSize(6*k);
            font.setWeight(QFont::Bold);
            painter.setFont(font);
            painter.setPen(QColor(36,42,112));
            painter.drawImage(QRectF(13*k-mG*k,125*k-mT*k,7*k,7*k),QImage("images/title-pdf.png"));
            painter.drawText(QRectF(22*k-mG*k,125*k-mT*k,(210-mD-mG)*k,10*k),Qt::AlignLeft,tr("Planning d'observation de la soirée"));
            painter.drawLine(13*k-mG*k,135*k-mT*k,(210-mD-mG-13)*k,135*k-mT*k);
            // On écrit la légende
            font.setPointSize(3.7*k);
            font.setWeight(QFont::Normal);
            painter.setFont(font);
            painter.setPen(QColor(0,0,0));
            painter.drawText(QRectF(13*k-mG*k,140*k-mT*k,(210-mD-mG)*k,50*k),Qt::AlignLeft,tr("Le planning suivant donne plusieurs informations pratiques pour faciliter le repérage et\nl'observation des objets. Au dessus vous avez le nom principal de l'objet suivi si nécessaire\ndes autres références. Les deux colonnes de gauche vous indiquent les informations fixes\nsur l'objet (AD, DEC, magnitude etc...). La troisième colonne fournit l'heure d'observation\nconseillée. Enfin la dernière colonne renseigne sur l'azimut et la hauteur de l'objet au début\nde son observation. Vous avez également un conseil sur l'oculaire à utiliser. Il peut arriver\nque des pauses s'intercalent entre deux objets, dans ces cas-là, un conseil vous est donné\npour patienter. Bonne observation !"));
            // On écrit le bloc de mise en température
            painter.setPen(QColor(136,136,136));
            rect = QRectF(13*k-mG*k,192*k-mT*k,(210-mD-mG-13)*k,8*k);
            painter.fillRect(rect,QColor(238,238,238));
            painter.drawLine(rect.topLeft(),rect.topRight());
            painter.drawLine(rect.topRight(),rect.bottomRight());
            painter.drawLine(rect.bottomRight(),rect.bottomLeft());
            painter.drawLine(rect.bottomLeft(),rect.topLeft());
            painter.setPen(QColor(0,0,0));
            painter.drawText(QRectF(13*k-mG*k,192.3*k-mT*k,(210-mD-mG-13)*k,8*k),Qt::AlignCenter,tr("Conseil : Sortez votre télescope vers ") + m_debut.addSecs(-Calculastro::miseEnTemperature(m_diametre)*60).toString("hh'h'mm") + tr(" pour le mettre en température"));
            // On écrit tous les autres blocs dans une boucle

            int tL(204), page(2); // Le point en haut à gauche du premier bloc
            QString icone;
            QVector<double> hauAzi;
            for(int i(0);i <m_listeObjets.count();i++)
            { // [FAIRE]  oculaires
                if(tL+28 > 285)
                {
                    printer->newPage();
                    page++;
                    tL = 27;
                    // On écrit le numéro de page
                    font.setPointSize(2*k);
                    painter.setFont(font);
                    painter.drawText(QRectF(0,0,(210-mD-mG)*k,7*k),Qt::AlignRight,QString::number(page));
                }
                if(i>0 && (m_listeObjets.at(i)->getDebut().toTime_t()-m_listeObjets.at(i-1)->getFin().toTime_t()) > user->value("generateur/pauseMin",TEMPS_ESPACE).toUInt()*60)
                {
                    // Si on est pas au premier objet et que le temps entre cet objet et celui d'avant est supérieur à TEMPS_ESPACE, alors on affiche une pause
                    int pause = m_listeObjets.at(i)->getDebut().toTime_t()-m_listeObjets.at(i-1)->getFin().toTime_t();
                    pause = (int) pause/60;
                    painter.setPen(QColor(136,136,136));
                    rect = QRectF(13*k-mG*k,tL*k-mT*k,(210-mD-mG-13)*k,6*k);
                    painter.fillRect(rect,QColor(238,238,238));
                    painter.drawLine(rect.topLeft(),rect.topRight());
                    painter.drawLine(rect.topRight(),rect.bottomRight());
                    painter.drawLine(rect.bottomRight(),rect.bottomLeft());
                    painter.drawLine(rect.bottomLeft(),rect.topLeft());
                    painter.setPen(QColor(0,0,0));
                    font.setPointSize(2.2*k);
                    painter.setFont(font);
                    painter.drawText(QRectF(13*k-mG*k,tL*k-mT*k,(210-mD-mG-13)*k,6*k),Qt::AlignCenter,tr("Pause de ")+QString::number(pause)+tr(" min. Conseil : ")+listeConseils.at(QRandomGenerator::global()->bounded(listeConseils.size())));
                    tL += 10;
                    if(tL+28 > 285)
                    {
                        printer->newPage();
                        page++;

                        // On écrit le numéro de page
                        font.setPointSize(2*k);
                        painter.setFont(font);
                        painter.drawText(QRectF(0,0,(210-mD-mG)*k,7*k),Qt::AlignRight,QString::number(page));

                        tL = 27;
                    }
                }
                if(QFile::exists("icones/"+m_listeObjets.at(i)->ref()+".jpg"))
                    icone = "icones/"+m_listeObjets.at(i)->ref()+".jpg";
                else
                    icone = "icones/default.png";

                painter.setPen(QColor(136,136,136));
                //Rectangle principal
                painter.drawRect(QRectF(13*k-mG*k,tL*k-mT*k,(210-mD-mG-13)*k,28*k));
                // Image
                painter.drawImage(QRectF(13*k-mG*k,tL*k-mT*k,28*k,28*k),QImage(icone));
                // Nom de l'objet
                font.setPointSize(2.8*k);
                painter.setFont(font);
                rect = QRectF(45*k-mG*k,(tL+1)*k-mT*k,(210-mD-mG-45)*k,5*k);
                painter.fillRect(rect,QColor(238,238,238));
                painter.drawLine(rect.topLeft(),rect.topRight());
                painter.drawLine(rect.topRight(),rect.bottomRight());
                painter.drawLine(rect.bottomRight(),rect.bottomLeft());
                painter.drawLine(rect.bottomLeft(),rect.topLeft());
                painter.setPen(QColor(0,0,0));
                painter.drawText(QRectF(46*k-mG*k,(tL+1.3)*k-mT*k,(210-mD-mG-46)*k,5*k),Qt::AlignLeft,m_listeObjets.at(i)->nomComplet());
                // Constellation
                font.setPointSize(1.6*k);
                painter.setFont(font);
                painter.drawText(QRectF(46*k-mG*k,(tL+1.75)*k-mT*k,(210-mD-mG-48)*k,5*k),Qt::AlignRight,Calculastro::abreviationToNom(m_listeObjets.at(i)->constellation()));
                // On écrit les infos de l'objet
                font.setPointSize(2.2*k);
                painter.setFont(font);
                painter.drawText(QRectF(48*k-mG*k,(tL+9)*k-mT*k,30*k,4*k),Qt::AlignLeft,tr("AD : ")+m_listeObjets.at(i)->ascdr());
                painter.drawText(QRectF(48*k-mG*k,(tL+14)*k-mT*k,30*k,4*k),Qt::AlignLeft,tr("DEC : ")+m_listeObjets.at(i)->declinaison());
                painter.drawText(QRectF(48*k-mG*k,(tL+19)*k-mT*k,30*k,4*k),Qt::AlignLeft,tr("Magnitude : ")+QString::number(m_listeObjets.at(i)->magnitude()));

                if(m_listeObjets.at(i)->type() == "Planète")
                {
                    ObjetPlaneteObs *planete = qobject_cast<ObjetPlaneteObs*>(m_listeObjets.at(i));
                    painter.drawText(QRectF(80*k-mG*k,(tL+9)*k-mT*k,40*k,4*k),Qt::AlignLeft,tr("Illumination : ")+QString::number(planete->illumination())+"%");
                    painter.drawText(QRectF(80*k-mG*k,(tL+14)*k-mT*k,40*k,4*k),Qt::AlignLeft,tr("Distance : ")+QString::number(planete->distance())+" UA");
                    painter.drawText(QRectF(80*k-mG*k,(tL+19)*k-mT*k,40*k,4*k),Qt::AlignLeft,tr("Type : ")+m_listeObjets.at(i)->type());
                }
                else
                {
                    painter.drawText(QRectF(80*k-mG*k,(tL+9)*k-mT*k,40*k,4*k),Qt::AlignLeft,tr("Interêt : ")+m_listeObjets.at(i)->interet(true));
                    painter.drawText(QRectF(80*k-mG*k,(tL+14)*k-mT*k,40*k,4*k),Qt::AlignLeft,tr("Taille : ")+QString::number(m_listeObjets.at(i)->taille())+"'");
                    painter.drawText(QRectF(80*k-mG*k,(tL+19)*k-mT*k,40*k,4*k),Qt::AlignLeft,tr("Type : ")+m_listeObjets.at(i)->type());
                }

                painter.drawText(QRectF(124*k-mG*k,(tL+9)*k-mT*k,30*k,4*k),Qt::AlignLeft,tr("Observer entre"));
                font.setPointSize(3*k);
                painter.setFont(font);
                painter.drawText(QRectF(124*k-mG*k,(tL+14)*k-mT*k,30*k,7*k),Qt::AlignLeft,m_listeObjets.at(i)->getDebut().toLocalTime().toString("hh'h'mm")+tr(" et ")+m_listeObjets.at(i)->getFin().toLocalTime().toString("hh'h'mm"));
                // On trace la ligne de séparation
                painter.setPen(QColor(136,136,136));
                painter.drawLine(158*k-mG*k,(tL+9)*k-mT*k,158*k-mG*k,(tL+23)*k-mT*k);
                // On écrit les infos de droite (hauteur, azimut,oculaire)
                painter.setPen(QColor(0,0,0));
                font.setPointSize(2.2*k);
                painter.setFont(font);
                hauAzi = hauteurAzimutObjet(i);
                painter.drawText(QRectF(163*k-mG*k,(tL+9)*k-mT*k,40*k,4*k),Qt::AlignLeft,tr("Hauteur : ")+Calculastro::degreeToDms(hauAzi.at(0)));
                painter.drawText(QRectF(163*k-mG*k,(tL+14)*k-mT*k,40*k,4*k),Qt::AlignLeft,tr("Azimut : ")+Calculastro::degreeToDms(hauAzi.at(1)));
                painter.drawText(QRectF(163*k-mG*k,(tL+19)*k-mT*k,40*k,4*k),Qt::AlignLeft,tr("Oculaire : ")+Calculastro::getOculaire(m_listeObjets.at(i),m_diametre,m_focale,oculairesInt));

                tL += 32;
            }
            printer->newPage(); // On change de page
            page++;
            // On écrit le numéro de page
            font.setPointSize(2*k);
            painter.setFont(font);
            painter.drawText(QRectF(0,0,(210-mD-mG)*k,7*k),Qt::AlignRight,QString::number(page));
            // On écrit le titre : CARTE DU CIEL
            font.setPointSize(6*k);
            font.setWeight(QFont::Bold);
            painter.setFont(font);
            painter.setPen(QColor(36,42,112));
            painter.drawImage(QRectF(13*k-mG*k,10*k-mT*k,7*k,7*k),QImage("images/title-pdf.png"));
            painter.drawText(QRectF(22*k-mG*k,10*k-mT*k,(210-mD-mG)*k,10*k),Qt::AlignLeft,tr("Carte du ciel"));
            painter.drawLine(13*k-mG*k,20*k-mT*k,(210-mD-mG-13)*k,20*k-mT*k);
            // On écrit la légende
            font.setPointSize(3.7*k);
            font.setWeight(QFont::Normal);
            painter.setFont(font);
            painter.setPen(QColor(0,0,0));
            QDateTime moitieObs;
            moitieObs.setTime_t((m_debut.toTime_t()+m_fin.toTime_t())/2);
            painter.drawText(QRectF(13*k-mG*k,25*k-mT*k,(210-mD-mG)*k,10*k),Qt::AlignLeft,tr("Carte du ciel à la moitié de la soirée d'observation(")+moitieObs.toString("hh'h'mm")+tr("). Seuls les objets de la soirée sont\nplacés."));
            // On dessine l'image
            QGraphicsView *vue = new QGraphicsView(new Carteciel(this));
            vue->setBackgroundBrush(QColor(255,255,255));
            vue->setFixedSize(800,800);
            QPixmap *pixmap = new QPixmap(800,800);
            QPainter *painterImage = new QPainter(pixmap);
            painterImage->setRenderHint(QPainter::Antialiasing);
            vue->render(painterImage,QRectF(0,0,800,800),QRect(0,0,800,800));
            painterImage->end();
            painter.drawImage(QRectF(13*k-mG*k,45*k-mT*k,185*k,185*k),pixmap->toImage());

            // ON FAIT LES DEUX PAGES DE NOTES
            printer->newPage();
            page++;
            // On écrit le numéro de page
            font.setPointSize(2*k);
            painter.setFont(font);
            painter.drawText(QRectF(0,0,(210-mD-mG)*k,7*k),Qt::AlignRight,QString::number(page));
            // Ecriture du titre
            font.setPointSize(6*k);
            font.setWeight(QFont::Bold);
            painter.setFont(font);
            painter.setPen(QColor(36,42,112));
            painter.drawImage(QRectF(13*k-mG*k,10*k-mT*k,7*k,7*k),QImage("images/title-pdf.png"));
            painter.drawText(QRectF(22*k-mG*k,10*k-mT*k,(210-mD-mG)*k,10*k),Qt::AlignLeft,tr("Pages de notes"));
            painter.drawLine(13*k-mG*k,20*k-mT*k,(210-mD-mG-13)*k,20*k-mT*k);
            // Ecriture des pointillés
            font.setPointSize(3.7*k);
            font.setWeight(QFont::Normal);
            painter.setFont(font);
            painter.setPen(QColor(200,200,200));
            QString pointilles;
            for(int j(1);j <= 5217;j++)
            {
                pointilles+="-";
                if(j%111 == 0 && j > 0)
                    pointilles+="\n";
            }
            painter.drawText(QRectF(13*k-mG*k,25*k-mT*k,(210-mD-mG)*k,250*k),Qt::AlignLeft,pointilles);
            printer->newPage();
            page++;
            // On écrit le numéro de page
            painter.setPen(QColor(0,0,0));
            font.setPointSize(2*k);
            painter.setFont(font);
            painter.drawText(QRectF(0,0,(210-mD-mG)*k,7*k),Qt::AlignRight,QString::number(page));
            // On met les pointillés
            painter.setPen(QColor(200,200,200));
            font.setPointSize(3.7*k);
            painter.setFont(font);
            painter.drawText(QRectF(13*k-mG*k,10*k-mT*k,(210-mD-mG)*k,250*k),Qt::AlignLeft,pointilles);

            return true;
        }
        else
            return false;
    }
    else
        return false;
}
void Soiree::toPDF()
{
    QString fileName = QFileDialog::getSaveFileName(0,tr("Exporter la soirée en PDF"),QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)+"/soiree-pdf.pdf","*.pdf");
    if(!fileName.isEmpty())
    {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        int mT(7),mG(7),mD(7),mB(7); // On crée les marges (en millimètres)

        if(QFileInfo(fileName).suffix().isEmpty())
            fileName.append(".pdf");

        QPrinter printer(QPrinter::ScreenResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);
        printer.setPageMargins(QMarginsF(mG,mT,mD,mB), QPageLayout::Millimeter);
        if(printer.isValid())
        {

            if(paintPdf(&printer)) // ON DIT QUE TOUT S'EST BIEN PASSE
            {
                QApplication::restoreOverrideCursor();
                QMessageBox::information(0,tr("Enregistrement réussi"),tr("La soirée a bien été enregistrée au format PDF"));
            }
            else
                QApplication::restoreOverrideCursor();
        }
        else
        {
            QApplication::restoreOverrideCursor();
            QMessageBox::critical(0,tr("Erreur d'ouverture"),tr("Erreur d'ouverture, le fichier est-il lisible ?"));
        }
    }
}
QVector<double> Soiree::hauteurAzimutObjet(int index) const
{
    QVector<double> hauteurAzimut;
    ObjetObs *objet = getPlanning().at(index);

    hauteurAzimut = Calculastro::hauteurAzimutDegree(objet->getDebut().date(),objet->getDebut().time(),objet->ascdrDouble(),objet->decDouble(),m_lat,m_longi);
    return hauteurAzimut;
}
bool Soiree::ajouterSoireeRecentes(const QString &fileName)
{
    if(fileName.right(4) == ".soa" && QFile::exists(fileName))
    {
        QSqlQuery requete("SELECT COUNT(*) AS nbr FROM soireesRecentes WHERE fichier = :fichier");
        requete.bindValue(":fichier",fileName);
        requete.exec();
        requete.next();
        if(requete.value(0).toInt() == 0)
        {
            requete.prepare("INSERT INTO soireesRecentes (fichier) VALUES(:fichier)");
            requete.bindValue(":fichier",fileName);
            return requete.exec();
        }
        else
            return false;
    }
    else
        return false;
}
bool Soiree::enregistrerSoiree()
{
    if(m_file == "") // si il n'y a pas de fichier raccordés à la soirée, alors on l'enregistre
    {
        QString fileName = QFileDialog::getSaveFileName(0,tr("Enregistrer la soirée"),QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)+"/soiree.soa","Soirée d'observation astronomie (*.soa)");
        if(fileName != "")
        {
            if(!soireeToSoa(fileName))
            {
                QMessageBox::critical(0,tr("Echec"),tr("Echec de l'enregistrement de la soirée."));
                return false;
            }
            else
            {
                m_listeObjetsOld = m_listeObjets;
                m_file = fileName;
                ajouterSoireeRecentes(fileName);
                return true;
            }
        }
        else
            return false;
    }
    else
    {
        if(QFile::exists(m_file)) // Si le fichier existe on le supprime
            QFile::remove(m_file);

        if(!soireeToSoa(m_file))
        {
            QMessageBox::critical(0,tr("Echec"),tr("Echec de l'enregistrement de la soirée."));
            return false;
        }
        else
        {
            m_listeObjetsOld = m_listeObjets;
            return true;
        }
    }
}
bool Soiree::shouldBeSaved()
{
    if(hasChanged() || m_file == "" || m_listeObjetsOld.count() == 0)
        return true;
    else
        return false;
}
