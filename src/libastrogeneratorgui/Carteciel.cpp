#include "Carteciel.h"

#include <QColorDialog>
#include <QDebug>
#include <QDesktopServices>
#include <QFile>
#include <QGraphicsTextItem>
#include <QSqlQuery>
#include <QTextStream>

#include "Calculastro.h"
#include "Constantes.h"
#include "Soiree.h"
#include "astrocalc.h"

Carteciel::Carteciel(Soiree *soireeParam) :
	QGraphicsScene()
{
	m_user = new QSettings(NOM_EQUIPE, NOM_PROGRAMME);

	m_temps = soireeParam->getDebut();
	m_latitude = soireeParam->getLat();
	m_longitude = soireeParam->getLongi();
	m_planning = soireeParam->getPlanning();

	m_couleurFond = QColor(m_user->value("couleurCarte/fond", COULEUR_FOND).toString());
	m_couleurLegende = QColor(m_user->value("couleurCarte/legende", COULEUR_LEGENDE).toString());
	m_couleurObjet = QColor(m_user->value("couleurCarte/objet", COULEUR_OBJET).toString());
	m_couleurEtoile = QColor(m_user->value("couleurCarte/etoile", COULEUR_ETOILE).toString());
	m_couleurConstellation = QColor(m_user->value("couleurCarte/constellation", COULEUR_CONSTELLATION).toString());

	dessinerCarte();
}
void Carteciel::dessinerCarte()
{
	setSceneRect(0, 0, 800, 800);
	QRectF rectangle(0, 0, 800, 800);
	;
	addEllipse(rectangle, QColor(0, 0, 0), m_couleurFond);

	QGraphicsTextItem *carteDuCiel = addText("Carte du ciel le " + m_temps.toLocalTime().date().toString("dd/MM/yyyy") + " à " + m_temps.toLocalTime().time().toString("hh:mm"));
	carteDuCiel->setPos(10, 20);
	QGraphicsTextItem *latTexte = addText("Latitude : " + QString::number(m_latitude));
	latTexte->setPos(10, 40);
	QGraphicsTextItem *longTexte = addText("Longitude : " + QString::number(m_longitude));
	longTexte->setPos(10, 60);

	// On place la polaire
	double ascdr(0), dec(0), decrad(0), adrad(0), polaire_dec(0), polaire_ad(0), X(0), Y(0);
	int xInt(0), yInt(0), taille(0);
	ascdr = 37.95;
	dec = 89.25;
	QVector<double> haPolaire, hauteurAzimut;
	double raDecimal(0), declinaisonDecimal(0);
	QString line;
	QStringList couple;

	haPolaire = Calculastro::hauteurAzimutDegree(m_temps.date(), m_temps.time(), ascdr, dec, m_latitude, m_longitude);
	decrad = AstroCalc::deg2rad(haPolaire[0]);
	adrad = AstroCalc::deg2rad(haPolaire[1]);
	polaire_dec = AstroCalc::deg2rad(90);
	polaire_ad = AstroCalc::deg2rad(0);

	Y = (sin(decrad) * cos(polaire_dec) - cos(decrad) * sin(polaire_dec) * cos(adrad - polaire_ad)) / (1 + sin(decrad) * sin(polaire_dec) + cos(decrad) * cos(polaire_dec) * cos(adrad - polaire_ad));
	X = -1 * (cos(decrad) * sin(adrad - polaire_ad)) / (1 + sin(decrad) * sin(polaire_dec) + cos(decrad) * cos(polaire_dec) * cos(adrad - polaire_ad));

	xInt = floor(400 * (X + 1));
	yInt = floor(-400 * (Y - 1));
	yInt = 800 - yInt;
	addEllipse(xInt - 2, yInt - 2, 4, 4, m_couleurEtoile, m_couleurEtoile);
	QGraphicsTextItem *polaris = addText("Polaris");
	polaris->setDefaultTextColor(m_couleurEtoile);
	polaris->setPos(xInt + 5, yInt - 5);

	// On place les étoiles
	QSqlQuery etoiles("SELECT ascdr, declinaison, magnitude, nom FROM etoiles "
					  "WHERE magnitude < 6");
	while (etoiles.next()) {
		// Afficher les étoiles ici : voir code PHP toradian.php
		raDecimal = hmsToDegree(etoiles.value(0).toString());
		declinaisonDecimal = dmsToDegree(etoiles.value(1).toString());

		hauteurAzimut = Calculastro::hauteurAzimutDegree(m_temps.date(), m_temps.time(), raDecimal, declinaisonDecimal,
														 m_latitude, m_longitude);
		decrad = AstroCalc::deg2rad(hauteurAzimut[0]);
		adrad = AstroCalc::deg2rad(hauteurAzimut[1]);

		Y = (sin(decrad) * cos(polaire_dec) - cos(decrad) * sin(polaire_dec) * cos(adrad - polaire_ad)) / (1 + sin(decrad) * sin(polaire_dec) + cos(decrad) * cos(polaire_dec) * cos(adrad - polaire_ad));
		X = -1 * (cos(decrad) * sin(adrad - polaire_ad)) / (1 + sin(decrad) * sin(polaire_dec) + cos(decrad) * cos(polaire_dec) * cos(adrad - polaire_ad));

		xInt = 400 * (X + 1);
		yInt = 400 * (Y + 1);

		if (-yInt <= sqrt(160000 - pow(xInt - 400, 2)) - 400 && -yInt >= -sqrt(160000 - pow(xInt - 400, 2)) - 400) {
			taille = Calculastro::round(-0.597 * etoiles.value(2).toDouble() + 4.1, 0);
			addEllipse(xInt - taille / 2, yInt - taille / 2, taille, taille, m_couleurEtoile, m_couleurEtoile);
			if (etoiles.value(2).toDouble() < 1 && etoiles.value(3).toString() != "") {
				QGraphicsTextItem *texteEtoile = addText(etoiles.value(3).toString());
				texteEtoile->setDefaultTextColor(m_couleurLegende);
				texteEtoile->setPos(xInt + 2, yInt - 2);
			}
		}
	}
	// On place les objets de la soirée
	QString nomPlace;
	double magnitude;
	for (int i(0); i < m_planning.size(); i++) {
		hauteurAzimut = Calculastro::hauteurAzimutDegree(m_temps.date(), m_temps.time(), m_planning[i]->ascdrDouble(),
														 m_planning[i]->decDouble(), m_latitude, m_longitude);
		decrad = AstroCalc::deg2rad(hauteurAzimut[0]);
		adrad = AstroCalc::deg2rad(hauteurAzimut[1]);

		Y = (sin(decrad) * cos(polaire_dec) - cos(decrad) * sin(polaire_dec) * cos(adrad - polaire_ad)) / (1 + sin(decrad) * sin(polaire_dec) + cos(decrad) * cos(polaire_dec) * cos(adrad - polaire_ad));
		X = -1 * (cos(decrad) * sin(adrad - polaire_ad)) / (1 + sin(decrad) * sin(polaire_dec) + cos(decrad) * cos(polaire_dec) * cos(adrad - polaire_ad));
		xInt = 400 * (X + 1);
		yInt = 400 * (Y + 1);

		if (-yInt <= sqrt(160000 - pow(xInt - 400, 2)) - 400 && -yInt >= -sqrt(160000 - pow(xInt - 400, 2)) - 400) {
			nomPlace = m_planning.at(i)->nomComplet(true);

			magnitude = m_planning[i]->magnitude() + 2;
			taille = 12 - Calculastro::round(magnitude, 0);

			QGraphicsTextItem *nomObjetSoiree = addText(nomPlace);

			if (m_planning.at(i)->type() == "Planète") {
				nomObjetSoiree->setDefaultTextColor(planeteToColor(m_planning.at(i)->ref()));
				addEllipse(xInt, yInt, taille, taille, planeteToColor(m_planning.at(i)->ref()),
						   planeteToColor(m_planning.at(i)->ref()));
			} else {
				nomObjetSoiree->setDefaultTextColor(m_couleurObjet);
				addEllipse(xInt, yInt, taille + 2, taille, m_couleurObjet, m_couleurFond);
			}
			nomObjetSoiree->setPos(xInt + taille + 5, yInt - taille - 2);
		}
	}

	// On dessine les constellations
	QFile dessins("dessin-constellation.txt");
	if (dessins.open(QIODevice::ReadOnly)) {
		QTextStream flux(&dessins);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
		flux.setCodec("UTF-8");
#endif
		bool dessine(false);
		int X1(0), X2(0), Y1(0), Y2(0);
		double x1(0), x2(0), y1(0), y2(0);

		while (!flux.atEnd()) {
			line = flux.readLine();
			couple = line.split(' ');
			dessine = true;

			QSqlQuery requeteDessin("SELECT ascdr, declinaison FROM etoiles WHERE hr = " + couple.at(1) + " OR hr = " + couple.at(2));

			// Premier point
			if (!requeteDessin.next()) {
				qCritical() << "No result for" << requeteDessin.executedQuery();
				continue;
			}
			raDecimal = hmsToDegree(requeteDessin.value(0).toString());
			declinaisonDecimal = dmsToDegree(requeteDessin.value(1).toString());

			hauteurAzimut = Calculastro::hauteurAzimutDegree(m_temps.date(), m_temps.time(), raDecimal,
															 declinaisonDecimal, m_latitude, m_longitude);

			if (hauteurAzimut[0] >= 0) {
				decrad = AstroCalc::deg2rad(hauteurAzimut[0]);
				adrad = AstroCalc::deg2rad(hauteurAzimut[1]);

				y1 = (sin(decrad) * cos(polaire_dec) - cos(decrad) * sin(polaire_dec) * cos(adrad - polaire_ad)) / (1 + sin(decrad) * sin(polaire_dec) + cos(decrad) * cos(polaire_dec) * cos(adrad - polaire_ad));
				x1 = -1 * (cos(decrad) * sin(adrad - polaire_ad)) / (1 + sin(decrad) * sin(polaire_dec) + cos(decrad) * cos(polaire_dec) * cos(adrad - polaire_ad));

				X1 = Calculastro::round(400 * (x1 + 1), 0);
				Y1 = Calculastro::round(-400 * (y1 - 1), 0);
				Y1 = 800 - Y1;
			} else
				dessine = false;

			// Deuxieme point
			requeteDessin.next();
			raDecimal = hmsToDegree(requeteDessin.value(0).toString());
			declinaisonDecimal = dmsToDegree(requeteDessin.value(1).toString());

			hauteurAzimut = Calculastro::hauteurAzimutDegree(m_temps.date(), m_temps.time(), raDecimal,
															 declinaisonDecimal, m_latitude, m_longitude);

			if (hauteurAzimut[0] >= 0) {
				decrad = AstroCalc::deg2rad(hauteurAzimut[0]);
				adrad = AstroCalc::deg2rad(hauteurAzimut[1]);

				y2 = (sin(decrad) * cos(polaire_dec) - cos(decrad) * sin(polaire_dec) * cos(adrad - polaire_ad)) / (1 + sin(decrad) * sin(polaire_dec) + cos(decrad) * cos(polaire_dec) * cos(adrad - polaire_ad));
				x2 = -1 * (cos(decrad) * sin(adrad - polaire_ad)) / (1 + sin(decrad) * sin(polaire_dec) + cos(decrad) * cos(polaire_dec) * cos(adrad - polaire_ad));

				X2 = Calculastro::round(400 * (x2 + 1), 0);
				Y2 = Calculastro::round(-400 * (y2 - 1), 0);
				Y2 = 800 - Y2;
			} else
				dessine = false;

			// On dessine
			if (dessine)
				addLine(X1, Y1, X2, Y2, m_couleurConstellation);
		}
		dessins.close();
	}

	// On écrit les noms des constellations
	QFile noms("nom_constellation.txt");
	if (noms.open(QIODevice::ReadOnly)) {
		// setPen(QColor(156, 227, 254));
		QTextStream flux2(&noms);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
		flux2.setCodec("UTF-8");
#endif
		QRectF positions;
		while (!flux2.atEnd()) {
			line = flux2.readLine();
			couple = line.split('|');

			QSqlQuery requeteNoms("SELECT ascdr, declinaison FROM etoiles WHERE hr = " + couple.at(1));
			requeteNoms.next();

			raDecimal = hmsToDegree(requeteNoms.value(0).toString());
			declinaisonDecimal = dmsToDegree(requeteNoms.value(1).toString());

			hauteurAzimut = Calculastro::hauteurAzimutDegree(m_temps.date(), m_temps.time(), raDecimal,
															 declinaisonDecimal, m_latitude, m_longitude);
			decrad = AstroCalc::deg2rad(hauteurAzimut[0]);
			adrad = AstroCalc::deg2rad(hauteurAzimut[1]);

			Y = (sin(decrad) * cos(polaire_dec) - cos(decrad) * sin(polaire_dec) * cos(adrad - polaire_ad)) / (1 + sin(decrad) * sin(polaire_dec) + cos(decrad) * cos(polaire_dec) * cos(adrad - polaire_ad));
			X = -1 * (cos(decrad) * sin(adrad - polaire_ad)) / (1 + sin(decrad) * sin(polaire_dec) + cos(decrad) * cos(polaire_dec) * cos(adrad - polaire_ad));
			xInt = 400 * (X + 1);
			yInt = 400 * (Y + 1);

			if (-yInt <= sqrt(160000 - pow(xInt - 400, 2)) - 400 && -yInt >= -sqrt(160000 - pow(xInt - 400, 2)) - 400) {
				QGraphicsTextItem *nomConstellation = addText(couple.at(0));
				nomConstellation->setDefaultTextColor(QColor(156, 227, 254));
				positions = nomConstellation->boundingRect();
				xInt -= (positions.topRight().x() - positions.topLeft().x()) / 2;
				yInt -= (positions.bottomRight().y() - positions.topRight().y()) / 2;
				nomConstellation->setPos(xInt, yInt);
			}
		}
		noms.close();
	}
}
double Carteciel::dmsToDegree(QString dms)
{ // EXAMPLE : si dms vaut +44° 8' 5"

	double d(0), m(0), s(0);
	QString signe;
	QStringList list;
	list = dms.split(" "); // On a [0] -> +44° [1] -> 8' [2] -> 5"

	// On s'occupe du signe
	signe = list.at(0).at(0);

	// On s'occupe des degrés
	if (list.at(0).size() == 4)
		d = QStringView{list.at(0)}.mid(1, 2).toDouble();
	else if (list.at(0).size() == 3)
		d = QStringView{list.at(0)}.mid(1, 1).toDouble();
	else {
		qFatal("Erreur dans dmsToDegree()");
		// QMessageBox::critical(nullptr, "Erreur dans dmsToDegree()", "Une erreur s'est produite ...");
	}

	// On s'occupe des minutes
	m = QStringView{list.at(1)}.left(list.at(1).size() - 1).toDouble();

	// On s'occupe des secondes
	s = QStringView{list.at(2)}.left(list.at(2).size() - 1).toDouble();

	if (signe == "+")
		return d + m / 60 + s / 3600;
	else
		return -(d + m / 60 + s / 3600);
}
double Carteciel::hmsToDegree(QString hms)
{
	double h, m, s;
	QStringList list;
	list = hms.split(" ");

	// On s'occupe des heures
	h = QStringView{list.at(0)}.left(list.at(0).size() - 1).toDouble();

	// On s'occupe des minutes
	m = QStringView{list.at(1)}.left(list.at(1).size() - 3).toDouble();

	// On s'occupe des secondes
	s = QStringView{list.at(2)}.left(list.at(2).size() - 1).toDouble();

	return (h + m / 60 + s / 3600) * 15;
}

void Carteciel::setCouleurConstellation()
{
	QColor color = QColorDialog::getColor(m_couleurConstellation);
	m_couleurConstellation = color;
}
void Carteciel::setCouleurEtoile()
{
	QColor color = QColorDialog::getColor(m_couleurEtoile);
	m_couleurEtoile = color;
}
void Carteciel::setCouleurFond()
{
	QColor color = QColorDialog::getColor(m_couleurFond);
	m_couleurFond = color;
}
void Carteciel::setCouleurLegende()
{
	QColor color = QColorDialog::getColor(m_couleurLegende);
	m_couleurLegende = color;
}
void Carteciel::setCouleurObjet()
{
	QColor color = QColorDialog::getColor(m_couleurObjet);
	m_couleurObjet = color;
}
QColor Carteciel::planeteToColor(QString planeteRef)
{
	if (planeteRef == "P1") // Mercure
		return QColor(154, 154, 154);
	else if (planeteRef == "P2") {
		return QColor(210, 165, 81);
	} else if (planeteRef == "P3")
		return QColor(221, 100, 52);
	else if (planeteRef == "P4")
		return QColor(172, 125, 77);
	else if (planeteRef == "P5")
		return QColor(232, 215, 68);
	else if (planeteRef == "P6")
		return QColor(105, 183, 185);
	else if (planeteRef == "P7")
		return QColor(82, 125, 212);
	else
		return m_couleurObjet;
}
