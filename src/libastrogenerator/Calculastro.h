#ifndef CALCULASTRO_H
#define CALCULASTRO_H

#include <cmath>

#include <QLocale>
#include <QSettings>
#include <QString>
#include <QVector>

#include "HauteurMaxTemps.h"
#include "ObjetCPObs.h"
#include "ObjetObs.h"

// Si "OK" après une fonction, elle a été vérifiée et fonctionne parfaitement

class Calculastro
{
public:
    static QVector<QString> trouverVillePays(double latitude, double longitude);
    static QString paysToEnumQt(QString ref);
    static QString enumQtToPays(QString ref);
    static QString enumQtToPays(QLocale::Country country);
    static double partieDecimal(double val); // OK
    static double heureSiderale(QDate date, QTime heure, bool apparent = false); // OK
    static double toAngleHoraireDegree(QDate date, QTime heure, double ra, double longi); // OK
    static QVector<double> hauteurAzimutDegree(QDate const &date, QTime const &heure, double ra, double dec, double lat, double longi);
    static std::vector<double> RaDecPrecession(int year, double ra, double dec);
    static QString abreviationToNom(QString abreviation, bool inverse = false);
    static QVector<double> nutationObliquity(double j2000); // OK
    static QVector<double> coordonneesPlanetes(QDate date, QTime heure, QString planete);
    static double rad2deg(double rad);
    static double deg2rad(double deg);
    static QMap<char, double> getLBR(double j2000, QString planete);
    static QString degreeToDms(double val);
    static QString degreeToHms(double val);
    static double round(double n, unsigned d);
    static QString referencePlaneteToNom(QString ref, bool read = false);
    static HauteurMaxTemps hauteurMaxObjet(Objet *objet, QDateTime debut, QDateTime fin, double latitude, double longitude);
    static HauteurMaxTemps hauteurMaxObjet(QString const &planete, QDateTime debut, QDateTime fin, double latitude, double longitude);
    static bool verifDisponibilite(QVector<ObjetObs *> const &liste_observation, QDateTime const &debut, QDateTime const &fin, int espace);
    static double noterObjetVisible(QString type, int interet, double magnitude, int diametre, QString niveau, double hauteurMax, int difficulte, QSettings *user);
    static bool isObjetVisible(Objet *objet, QDateTime temps, double latitude, double longitude);
    static int miseEnTemperature(int diametre);
    static QString getOculaire(Objet *objet, int diametre, int focale, QVector<int> oculaires);
};

#endif // CALCULASTRO_H
