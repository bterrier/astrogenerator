#include "Soiree.h"

#include <cmath>

#include <QTextStream>

#include <QCoreApplication>
#include <QSqlQuery>
#include <QtXml>

#include "Calculastro.h"
#include "Constantes.h"
#include "HauteurMaxTemps.h"
#include "ObjetCP.h"
#include "ObjetCPObs.h"
#include "ObjetObs.h"
#include "ObjetPlaneteObs.h"
#include "settings.h"

Soiree::Soiree() {}
void Soiree::genererSoiree(double lat, double longi, QDateTime debut, QDateTime fin, int duree_une, QString constellation, QString niveau, unsigned int diametre, unsigned int focale,
                           int hauteurMin,
                           int pauseMin,
                           const Notes &notes, bool boolPlanete)
{
	int nb_objets(0), moyTime(0), hMin(30), rep(0), espaceMin(0);
	QDateTime obsPla[2];
	QVector<double> infosPlanete;
	QString valeur;
	double magMax(0), raDecimal(0), declinaisonDecimal(0), note(0);

	espaceMin = pauseMin * 60; // En secondes, utile pour la fonction verifDisponibilite

	duree_une *= 60; // On convertit en secondes
	nb_objets = (fin.toSecsSinceEpoch() - debut.toSecsSinceEpoch()) / duree_une;
	rep = (fin.toSecsSinceEpoch() - debut.toSecsSinceEpoch()) / 60;

	magMax = Calculastro::round(2 + 5 * log10(diametre), 1) + 1; //  Loi de Pogson diametre du télescope -> magnitude max;

	QVector<ObjetObs *> liste_observation; // Contiendra dans l'ordre les objets avec leur moment d'observation
	moyTime = floor((debut.toSecsSinceEpoch() + fin.toSecsSinceEpoch()) / 2); // Heure à la moitié de la soirée

	QStringList planetes;
	planetes << "P1"
	         << "P2"
	         << "P3"
	         << "P4"
	         << "P5"
	         << "P6"
	         << "P7";

	QDateTime dateTime;
	dateTime.setSecsSinceEpoch(moyTime);

	if (boolPlanete) // Si on veut ajouter les planètes
	{
		for (int i(0); i < planetes.size(); i++) {
			emit generation(i + 1);
			valeur = planetes.at(i);

			infosPlanete = Calculastro::coordonneesPlanetes(dateTime.date(), dateTime.time(), Calculastro::referencePlaneteToNom(valeur));
			if (infosPlanete[2] <= magMax) // On vérifie qu'on peut l'observer
			{
				HauteurMaxTemps hauteurMaxPla = Calculastro::hauteurMaxObjet(Calculastro::referencePlaneteToNom(valeur), debut, fin, lat, longi);
				obsPla[0] = hauteurMaxPla.temps().addSecs(-duree_une / 2);
				obsPla[1] = hauteurMaxPla.temps().addSecs(duree_une / 2);

				// ON COMPENSE LES TEMPS QUI DEPASSENT DE LA DUREE DE LA SOIREE
				if (hauteurMaxPla.temps().addSecs(-duree_une / 2) < debut) { // Si on est au début
					int compenser(0);
					compenser = debut.toSecsSinceEpoch() - hauteurMaxPla.temps().addSecs(-duree_une / 2).toSecsSinceEpoch();
					obsPla[0] = obsPla[0].addSecs(compenser);
					obsPla[1] = obsPla[1].addSecs(compenser);
				} else if (hauteurMaxPla.temps().addSecs(duree_une / 2) > fin) { // Si on est au début
					int compenser(0);
					compenser = fin.toSecsSinceEpoch() - hauteurMaxPla.temps().addSecs(duree_une / 2).toSecsSinceEpoch();
					obsPla[0] = obsPla[0].addSecs(compenser);
					obsPla[1] = obsPla[1].addSecs(compenser);
				}

				if (Calculastro::verifDisponibilite(liste_observation, obsPla[0], obsPla[1], espaceMin)) {
					if (hauteurMaxPla.hauteurMax() >= hauteurMin) {
						liste_observation.push_back(new ObjetPlaneteObs(valeur, obsPla[0], obsPla[1]));
					}
				} else {
					int j(0);
					double hau1(0), hau2(0);
					QDateTime tempsPlus[2], tempsMoins[2], tempsPlace1[2], tempsPlace2[2];
					tempsPlus[0] = obsPla[0].addSecs(60);
					tempsPlus[1] = obsPla[1].addSecs(60);
					tempsMoins[0] = obsPla[0].addSecs(-60);
					tempsMoins[1] = obsPla[1].addSecs(-60);
					bool tryV;

					QVector<double> altazPlus, altazMoins;

					while (j < 719) {
						tryV = true;

						altazPlus = Calculastro::hauteurAzimutDegree(tempsPlus[0].date(), tempsPlus[0].time(), infosPlanete[0], infosPlanete[1], lat, longi);
						altazMoins = Calculastro::hauteurAzimutDegree(tempsMoins[0].date(), tempsMoins[0].time(), infosPlanete[0], infosPlanete[1], lat, longi);

						if (altazPlus[0] > altazMoins[0]) { // On regarde à quel endroit doit être placé l'objet pour qu'il soit le plus haut
							tempsPlace1[0] = tempsPlus[0];
							tempsPlace1[1] = tempsPlus[1];

							tempsPlace2[0] = tempsMoins[0];
							tempsPlace2[1] = tempsMoins[1];

							hau1 = altazPlus[0];
							hau2 = altazMoins[0];
						} else {
							tempsPlace1[0] = tempsMoins[0];
							tempsPlace1[1] = tempsMoins[1];

							tempsPlace2[0] = tempsPlus[0];
							tempsPlace2[1] = tempsPlus[1];

							hau1 = altazMoins[0];
							hau2 = altazPlus[0];
						}
						if (Calculastro::verifDisponibilite(liste_observation, tempsPlace1[0], tempsPlace1[1], espaceMin)) {
							if (hau1 > hauteurMin) // Si on dépasse 10°
							{
								if (tempsPlace1[0] >= debut && tempsPlace1[0] <= fin && tempsPlace1[1] >= debut && tempsPlace1[1] <= fin) {
									liste_observation.push_back(new ObjetPlaneteObs(valeur, tempsPlace1[0], tempsPlace1[1]));
									break;
								} else
									tryV = false;
							} else
								tryV = false;
						} else
							tryV = false;

						if (!tryV && Calculastro::verifDisponibilite(liste_observation, tempsPlace2[0], tempsPlace2[1], espaceMin)) {
							if (hau2 > hauteurMin) {
								if (tempsPlace2[0] >= debut && tempsPlace2[0] <= fin && tempsPlace2[1] >= debut && tempsPlace2[1] <= fin) {
									liste_observation.push_back(new ObjetPlaneteObs(valeur, tempsPlace2[0], tempsPlace2[1]));
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
	if (constellation == "toutes")
		sql = "";
	else if (constellation.length() > 3)
		sql = " AND (constellation = '" + constellation.replace('|', "' OR constellation = '") + "')";
	else if (constellation.length() == 3)
		sql = " AND constellation = '" + constellation + "'";

	// mettre un else ici pour gÃ©rer le cas des constellations multiples

	QMap<double, QString> objets_visibles1;
	ObjetCP *objet(nullptr);

	QSqlQuery requeteCount("SELECT COUNT(*) as nbr FROM ngcic WHERE type <> '' AND magnitude <> 0 AND taille > 0 AND interet > 2" + sql);
	requeteCount.next();
	int nbResult(0);
	nbResult = requeteCount.value(0).toInt();

	QSqlQuery requete("SELECT nom, reference, type, ascdr, declinaison, constellation, magnitude, messier, interet, taille, difficulte FROM ngcic WHERE type <> '' AND magnitude <> 0 AND taille > 0 AND interet > 2" + sql + " ORDER BY reference");

	while (requete.next()) {
		emit generation(7 + requete.at() * 25 / nbResult);

		objet = new ObjetCP(requete.value(1).toString());
		HauteurMaxTemps hauteurMax2 = Calculastro::hauteurMaxObjet(objet, debut, fin, lat, longi);

		if (hauteurMax2.hauteurMax() > hMin) {
			note = Calculastro::noterObjetVisible(requete.value(2).toString(), requete.value(8).toInt(), requete.value(6).toDouble(), diametre, niveau, hauteurMax2.hauteurMax(), requete.value(10).toInt(), notes); // on calcule sa note
			objets_visibles1.insert(note, requete.value(1).toString());
		}
	}

	QMapIterator<double, QString> iterateur(objets_visibles1);

	iterateur.toBack(); // On parcourt le tableau en sens inverse (note décroissante)

	int k(0);
	while (iterateur.hasPrevious()) {
		k++;
		emit generation(32 + k * 68 / objets_visibles1.count());

		iterateur.previous();

		if (liste_observation.count() >= nb_objets) // Si on a placé assez d'objet, on arrête
			break;

		ObjetCP *objet = new ObjetCP(iterateur.value());
		raDecimal = objet->ascdrDouble();
		declinaisonDecimal = objet->decDouble();

		HauteurMaxTemps hauteurMaxD = Calculastro::hauteurMaxObjet(objet, debut, fin, lat, longi);
		QDateTime tempsObs[2];
		tempsObs[0] = hauteurMaxD.temps().addSecs(-duree_une / 2);
		tempsObs[1] = hauteurMaxD.temps().addSecs(duree_une / 2);

		// ON COMPENSE LES TEMPS QUI DEPASSENT DE LA DUREE DE LA SOIREE
		if (hauteurMaxD.temps().addSecs(-duree_une / 2) < debut) { // Si on est au début
			int compenser(0);
			compenser = debut.toSecsSinceEpoch() - hauteurMaxD.temps().addSecs(-duree_une / 2).toSecsSinceEpoch();
			tempsObs[0] = tempsObs[0].addSecs(compenser);
			tempsObs[1] = tempsObs[1].addSecs(compenser);
		} else if (hauteurMaxD.temps().addSecs(duree_une / 2) > fin) { // Si on est au début
			int compenser(0);
			compenser = fin.toSecsSinceEpoch() - hauteurMaxD.temps().addSecs(duree_une / 2).toSecsSinceEpoch();
			tempsObs[0] = tempsObs[0].addSecs(compenser);
			tempsObs[1] = tempsObs[1].addSecs(compenser);
		}

		if (Calculastro::verifDisponibilite(liste_observation, tempsObs[0], tempsObs[1], espaceMin)) {
			if (Calculastro::isObjetVisible(objet, tempsObs[0], lat, longi) && Calculastro::isObjetVisible(objet, tempsObs[1], lat, longi))
				liste_observation.push_back(new ObjetCPObs(objet, tempsObs[0], tempsObs[1]));
		} else {
			int i = 0;
			QDateTime tempsPlus[2], tempsMoins[2], tempsPlace1[2], tempsPlace2[2];
			double hau1(0), hau2(0);
			tempsPlus[0] = tempsObs[0].addSecs(60);
			tempsPlus[1] = tempsObs[1].addSecs(60);
			tempsMoins[0] = tempsObs[0].addSecs(-60);
			tempsMoins[1] = tempsObs[1].addSecs(-60);

			QVector<double> altazPlus, altazMoins;
			bool tryVar(true);

			while (i < rep) {
				altazPlus = Calculastro::hauteurAzimutDegree(tempsPlus[0].date(), tempsPlus[0].time(), raDecimal, declinaisonDecimal, lat, longi);
				altazMoins = Calculastro::hauteurAzimutDegree(tempsMoins[0].date(), tempsMoins[0].time(), raDecimal, declinaisonDecimal, lat, longi);
				tryVar = true;

				if (altazPlus[0] > altazMoins[0]) {
					tempsPlace1[0] = tempsPlus[0];
					tempsPlace1[1] = tempsPlus[1];

					tempsPlace2[0] = tempsMoins[0];
					tempsPlace2[1] = tempsMoins[1];

					hau1 = altazPlus[0];
					hau2 = altazMoins[0];
				} else {
					tempsPlace1[0] = tempsMoins[0];
					tempsPlace1[1] = tempsMoins[1];

					tempsPlace2[0] = tempsPlus[0];
					tempsPlace2[1] = tempsPlus[1];

					hau1 = altazMoins[0];
					hau2 = altazPlus[0];
				}
				if (Calculastro::verifDisponibilite(liste_observation, tempsPlace1[0], tempsPlace1[1], espaceMin)) {
					if (hau1 > hMin) // Si on dépasse $hauteurMin°
					{
						if (tempsPlace1[0] >= debut && tempsPlace1[0] <= fin && tempsPlace1[1] >= debut && tempsPlace1[1] <= fin) {
							liste_observation.push_back(new ObjetCPObs(objet, tempsPlace1[0], tempsPlace1[1]));
							break;
						} else
							tryVar = false;
					} else
						tryVar = false;
				} else
					tryVar = false;
				if (!tryVar && Calculastro::verifDisponibilite(liste_observation, tempsPlace2[0], tempsPlace2[1], espaceMin)) {
					if (hau2 > hMin) {
						if (tempsPlace2[0] >= debut && tempsPlace2[0] <= fin && tempsPlace2[1] >= debut && tempsPlace2[1] <= fin) {
							liste_observation.push_back(new ObjetCPObs(objet, tempsPlace2[0], tempsPlace2[1]));
							break;
						}
					}
				}
				tempsPlus[0] = tempsPlus[0].addSecs(60);
				tempsPlus[1] = tempsPlus[1].addSecs(60);
				tempsMoins[0] = tempsMoins[0].addSecs(-60);
				tempsMoins[1] = tempsMoins[1].addSecs(-60);

				if (tempsPlus[1] > fin) // Si on a atteint la fin
				{
					tempsPlus[1] = fin;
					tempsPlus[0] = fin.addSecs(-duree_une);
				}
				if (tempsMoins[0] < debut) {
					tempsMoins[0] = debut;
					tempsMoins[1] = debut.addSecs(duree_une);
				}
				i++;
			} // Au bout de 720 fois, la boucle s'arrête
		}
	}
	liste_observation = trierPlanning(liste_observation);
	if (liste_observation.size() > 0) // Si il n'y a aucun objet à observer
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

	while (planning.size() > 0) {
		position = trouverPlusPetit(planning);
		i.findNext(planning[position]);
		newPlanning.push_back(planning[position]);
		planning.erase(planning.begin() + position);
	}
	return newPlanning;
}
int Soiree::trouverPlusPetit(const QVector<ObjetObs *> &planning)
{
	int temp(0);
	for (int i(0); i < planning.size(); i++) {
		if (planning[i]->getDebut() < planning[temp]->getDebut())
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
	if (debut.isValid() && !debut.isNull())
		m_debut = debut;
}
void Soiree::setDiametre(unsigned int diametre)
{
	if (diametre >= DIAMETRE_MINIMUM && diametre <= DIAMETRE_MAXIMUM)
		m_diametre = diametre;
}
void Soiree::setFin(QDateTime fin)
{
	if (fin.isValid() && !fin.isNull())
		m_fin = fin;
}
void Soiree::setFocale(unsigned int focale)
{
	if (focale >= FOCALE_MINIMUM && focale <= FOCALE_MAXIMUM)
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
	if (liste.size() > 0)
		m_listeObjets = liste;
}
void Soiree::setPlanningOld(QVector<ObjetObs *> liste)
{
	if (liste.size() > 0)
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
	if (index >= 0 && index < m_listeObjets.size())
		m_listeObjets.erase(m_listeObjets.begin() + index);
	else
		qCritical() << "L'index fourni pour la supression est invalide";
}
QString Soiree::getPays() const
{
	if (m_pays == "")
		return COORDONNEES_INCONNU;
	else
		return m_pays;
}
QString Soiree::getVille() const
{
	if (m_ville == "")
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
	if (index <= m_listeObjets.size() - 1 && index > 0) // On vérifie si l'index rentre bien dans le planning
	{
		QDateTime debutP, debutF, finP, finF;
		debutP = m_listeObjets[index - 1]->getDebut();
		finP = m_listeObjets[index - 1]->getFin();
		debutF = m_listeObjets[index]->getDebut();
		finF = m_listeObjets[index]->getFin();

		m_listeObjets[index - 1]->setFin(finF);
		m_listeObjets[index]->setDebut(debutP);
		m_listeObjets[index]->setFin(debutP.addSecs(finF.toSecsSinceEpoch() - debutF.toSecsSinceEpoch()));
		m_listeObjets[index - 1]->setDebut(finF.addSecs(-(finP.toSecsSinceEpoch() - debutP.toSecsSinceEpoch())));

		m_listeObjets = trierPlanning(m_listeObjets);
	} else
		qCritical() << "L'index de modification fournie est invalide";
}
void Soiree::descendreObjet(int index)
{
	// Si on veut descendre index, ça revient à monter index+1
	monterObjet(index + 1);
}
void Soiree::modifierDuree(int index, int duree)
{
	if (index >= 0 && index < m_listeObjets.size()) {
		if (duree <= DUREE_OBJET_MAX && duree >= DUREE_OBJET_MIN) {
			int dif(0); // En secondes
			dif = (duree * 60) - (m_listeObjets[index]->getFin().toSecsSinceEpoch() - m_listeObjets[index]->getDebut().toSecsSinceEpoch());
			m_listeObjets[index]->setFin(m_listeObjets[index]->getDebut().addSecs(duree * 60));
			for (int i(index + 1); i < m_listeObjets.size(); i++) {
				m_listeObjets[i]->setDebut(m_listeObjets[i]->getDebut().addSecs(dif));
				m_listeObjets[i]->setFin(m_listeObjets[i]->getFin().addSecs(dif));
			}
		} else
			qCritical() << QStringLiteral("Il est impossible de modifier l'objet avec la durée indiquée : %1 min. Elle doit être comprise entre 1 et 20 min.").arg(duree);
	} else
		qCritical() << "L'index fourni pour la modification est invalide";
}
void Soiree::ajouterObjet(int index, ObjetCP *objetParam, int duree)
{ // INDEX : index de l'objet précédent REFERENCE : pour instancier Objet DUREE : duree de l'objet

	QSettings *user = new QSettings(NOM_EQUIPE, NOM_PROGRAMME);
	if (index >= 0 && index < m_listeObjets.size() && indexFromRef(objetParam->ref()) == -1) // On vérifie qu'on a un index correct et que l'objet n'est pas dans la soirée
	{
		QVector<double> hauteurAzimut;
		QDateTime debutObs, finObs;

		debutObs = m_listeObjets[index]->getFin().addSecs(user->value("generateur/pauseMin", TEMPS_ESPACE).toInt() * 60);
		finObs = debutObs.addSecs(duree * 60);

		ObjetCPObs *objet = new ObjetCPObs(objetParam, debutObs, finObs);
		if (objet->isValid()) {
			hauteurAzimut = Calculastro::hauteurAzimutDegree(debutObs.date(), debutObs.time(), objet->ascdrDouble(), objet->decDouble(), m_lat, m_longi);
			if (hauteurAzimut[0] > user->value("generateur/hauteurMin", HAUTEUR_MIN_OBJET).toInt()) {
				for (int i(index + 1); i < m_listeObjets.size(); i++) {
					m_listeObjets[i]->setDebut(m_listeObjets[i]->getDebut().addSecs(duree * 60 + user->value("generateur/pauseMin", TEMPS_ESPACE).toInt() * 60));
					m_listeObjets[i]->setFin(m_listeObjets[i]->getFin().addSecs(duree * 60 + user->value("generateur/pauseMin", TEMPS_ESPACE).toInt() * 60));
				}
				m_listeObjets.push_back(objet);
				m_listeObjets = trierPlanning(m_listeObjets);
			} else
				qCritical() << "Il est impossible d'ajouter l'objet car il est trop bas sur l'horizon. Minimum 10°";
		} else
			qCritical() << "L'objet demandé est introuvable";

	} else
		qCritical() << "L'index fourni pour l'ajout de l'objet est invalide.";
}
void Soiree::ajouterObjet(int index, QString refPlanete, int duree)
{
	QSettings *user = new QSettings(NOM_EQUIPE, NOM_PROGRAMME);
	if (index >= 0 && index < m_listeObjets.size() && indexFromRef(refPlanete) == -1) // On vérifie qu'on a un index correct et que l'objet n'est pas dans la soirée
	{
		QVector<double> hauteurAzimut;
		QDateTime debutObs, finObs;

		debutObs = m_listeObjets[index]->getFin().addSecs(user->value("generateur/pauseMin", TEMPS_ESPACE).toInt() * 60);
		finObs = debutObs.addSecs(duree * 60);

		ObjetPlaneteObs *objet = new ObjetPlaneteObs(refPlanete, debutObs, finObs);
		if (objet->isValid()) {
			hauteurAzimut = Calculastro::hauteurAzimutDegree(debutObs.date(), debutObs.time(), objet->ascdrDouble(), objet->decDouble(), m_lat, m_longi);
			if (hauteurAzimut[0] > user->value("generateur/hauteurMin", HAUTEUR_MIN_OBJET).toInt()) {
				for (int i(index + 1); i < m_listeObjets.size(); i++) {
					m_listeObjets[i]->setDebut(m_listeObjets[i]->getDebut().addSecs(duree * 60 + user->value("generateur/pauseMin", TEMPS_ESPACE).toInt() * 60));
					m_listeObjets[i]->setFin(m_listeObjets[i]->getFin().addSecs(duree * 60 + user->value("generateur/pauseMin", TEMPS_ESPACE).toInt() * 60));
				}
				m_listeObjets.push_back(objet);
				m_listeObjets = trierPlanning(m_listeObjets);
			} else
				qCritical() << "Il est impossible d'ajouter l'objet car il est trop bas sur l'horizon.";
		} else
			qCritical() << "L'objet demandé est introuvable";
	} else
		qCritical() << "L'index fourni pour l'ajout de l'objet est invalide.";
}
void Soiree::ajouterObjet(ObjetObs *objet)
{
	QSettings *user = new QSettings(NOM_EQUIPE, NOM_PROGRAMME);
	if (objet->getDebut() >= m_debut && objet->getDebut() < m_fin && objet->getFin() > m_debut && objet->getFin() <= m_fin) { // On vérifie qu'il rentre bien dans la soirée

		bool ok(true);
		for (int i(0); i < m_listeObjets.count(); i++) {
			if (objet->getDebut() > m_listeObjets.at(i)->getDebut().addSecs(-user->value("generateur/pauseMin", TEMPS_ESPACE).toInt() * 60) && objet->getDebut() < m_listeObjets.at(i)->getFin().addSecs(user->value("generateur/pauseMin", TEMPS_ESPACE).toInt() * 60))
				ok = false;
			else if (objet->getFin() > m_listeObjets.at(i)->getDebut().addSecs(-user->value("generateur/pauseMin", TEMPS_ESPACE).toInt() * 60) && objet->getFin() < m_listeObjets.at(i)->getFin().addSecs(user->value("generateur/pauseMin", TEMPS_ESPACE).toInt() * 60))
				ok = false;
		}
		if (ok && objet->isValid()) { // On vérifie qu'il n'y a pas d'autres objets à cet endroit
			QVector<double> hauteurAzimut;
			hauteurAzimut = Calculastro::hauteurAzimutDegree(objet->getDebut().date(), objet->getDebut().time(), objet->ascdrDouble(), objet->decDouble(), m_lat, m_longi);
			if (hauteurAzimut[0] > user->value("generateur/hauteurMin", HAUTEUR_MIN_OBJET).toInt()) {
				m_listeObjets.push_back(objet);
				m_listeObjets = trierPlanning(m_listeObjets);
			} else
				qCritical() << "L'objet n'est pas assez haut au moment demandé pour pouvoir être ajouté dans la soirée.";
		} else
			qCritical() << "L'objet n'est pas valide ou ne peut être placé ici";
	} else
		qCritical() << "La date de l'objet ne correspond pas avec la date de la soirée.";
}

Soiree *Soiree::soaToSoiree(QString const &fileName)
{
	QFile soa(fileName);
	if (soa.open(QIODevice::ReadOnly)) {
		QTextStream flux(&soa);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
		flux.setCodec("UTF-8");
#endif
		QVector<ObjetObs *> planning;
		QVector<double> oculairesVector;
		QString infosSoiree, ligne, oculaires;
		QStringList listInfosSoiree, listInfosObjet, listOculaires;
		QDateTime debut, fin;
		infosSoiree = flux.readLine();
		listInfosSoiree = infosSoiree.split("|");

		if (listInfosSoiree.count() != 9) {
			qCritical() << "Le fichier demandé est incorrect";
			return new Soiree;
		} else {
			oculaires = flux.readLine();
			listOculaires = oculaires.split("|");
			for (int i(0); i < listOculaires.count(); i++) {
				oculairesVector.push_back(listOculaires.at(i).toDouble());
			}
			while (!flux.atEnd()) {
				ligne = flux.readLine();
				listInfosObjet = ligne.split("|");
				if (listInfosObjet.count() != 3) {
					qCritical() << "Le fichier demandé est incorrect";
					return new Soiree;
				}
				if (listInfosObjet.at(0).at(0) != 'P') {
					debut.setSecsSinceEpoch(listInfosObjet.at(1).toInt());
					fin.setSecsSinceEpoch(listInfosObjet.at(2).toInt());

					planning.push_back(new ObjetCPObs(listInfosObjet[0], debut, fin));
				} else {
					debut.setSecsSinceEpoch(listInfosObjet.at(1).toInt());
					fin.setSecsSinceEpoch(listInfosObjet.at(2).toInt());

					planning.push_back(new ObjetPlaneteObs(listInfosObjet[0], debut, fin));
				}
			}

			Soiree *soiree = new Soiree;
			soiree->setBoolPlanete(false);
			QDateTime debutSoiree;
			debutSoiree.setSecsSinceEpoch(listInfosSoiree.at(5).toInt());
			soiree->setDebut(debutSoiree);
			QDateTime finSoiree;
			finSoiree.setSecsSinceEpoch(listInfosSoiree.at(6).toInt());
			soiree->setFin(finSoiree);
			soiree->setDiametre(listInfosSoiree.at(7).toInt());
			soiree->setFocale(listInfosSoiree.at(8).toInt());
			soiree->setLat(listInfosSoiree.at(3).toDouble());
			soiree->setLongi(listInfosSoiree.at(4).toDouble());

			if (listInfosSoiree.at(2) == "pays")
				soiree->setPays(COORDONNEES_INCONNU);
			else
				soiree->setPays(listInfosSoiree.at(2));
			if (listInfosSoiree.at(1) == "ville")
				soiree->setVille(COORDONNEES_INCONNU);
			else
				soiree->setVille(listInfosSoiree.at(1));

			soiree->setNiveau(QString("debutant"));
			soiree->setPlanning(planning);
			soiree->setPlanningOld(planning);
			soiree->setFile(fileName);

			return soiree;
		}
	} else {
		qCritical() << "Le fichier demandé est impossible à ouvrir : " + fileName;
		Soiree *soiree = new Soiree;
		return soiree;
	}
}
bool Soiree::soireeToSoa(const QString &fileName)
{
	const auto oculaires = Settings::instance().eyepieces();

	QString ligne1, ligne2;
	ligne1 = "cle|" + getVille() + "|" + getPays() + "|" + QString::number(m_lat) + "|" + QString::number(m_longi) + "|" + QString::number(m_debut.toSecsSinceEpoch()) + "|" + QString::number(m_fin.toSecsSinceEpoch()) + "|" + QString::number(m_diametre) + "|" + QString::number(m_focale);
	for (const auto &eyepiece : oculaires) {
		ligne2 += QString::number(eyepiece.focalLength()) + "|";
	}
	ligne2 = ligne2.left(ligne2.count() - 1);
	QFile soa(fileName);
	if (soa.open(QIODevice::WriteOnly)) {
		QTextStream flux(&soa);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
		flux.setCodec("UTF-8");
#endif
		flux << ligne1 << Qt::endl
		     << ligne2 << Qt::endl;

		for (int j(0); j < m_listeObjets.count(); j++) {
			flux << m_listeObjets.at(j)->ref() << "|" << QString::number(m_listeObjets.at(j)->getDebut().toSecsSinceEpoch()) << "|" << QString::number(m_listeObjets.at(j)->getFin().toSecsSinceEpoch()) << Qt::endl;
		}
		soa.close();
		return true;
	} else
		return false;
}
int Soiree::indexFromRef(QString ref) const
{
	for (int i(0); i < m_listeObjets.count(); i++) {
		if (m_listeObjets.at(i)->ref() == ref) {
			return i;
		}
	}
	return -1;
}
bool Soiree::hasChanged()
{
	if (m_listeObjets.count() != m_listeObjetsOld.count())
		return true;
	else {
		for (int i(0); i < m_listeObjets.count(); i++) {
			if (m_listeObjets.at(i) != m_listeObjetsOld.at(i))
				return true;
		}
		return false;
	}
}
void Soiree::toXML(QIODevice *device) const
{
	QLocale locale;
	QDomDocument doc;
	QDomProcessingInstruction instr = doc.createProcessingInstruction("xml", "version=\"1.0\"  encoding=\"UTF-8\"");
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
	for (int i(0); i < m_listeObjets.count(); i++) {
		QDomElement objet = doc.createElement("objet" + QString::number(i + 1));
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
		taille.appendChild(doc.createTextNode(QString::number(m_listeObjets.at(i)->taille()) + "'"));
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

	device->write(doc.toByteArray());
}

QVector<double> Soiree::hauteurAzimutObjet(int index) const
{
	QVector<double> hauteurAzimut;
	ObjetObs *objet = getPlanning().at(index);

	hauteurAzimut = Calculastro::hauteurAzimutDegree(objet->getDebut().date(), objet->getDebut().time(), objet->ascdrDouble(), objet->decDouble(), m_lat, m_longi);
	return hauteurAzimut;
}
bool Soiree::shouldBeSaved()
{
	if (hasChanged() || m_file == "" || m_listeObjetsOld.count() == 0)
		return true;
	else
		return false;
}
