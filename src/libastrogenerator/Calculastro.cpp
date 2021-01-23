#include "Calculastro.h"
#include "Constantes.h"

#include <QtMath>

#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QSqlQuery>
#include <QTextStream>

#include "astrocalc.h"

// ------------------------------------------------------------------------------------
// NOTE IMPORTANTE
// Toutes les dates qu'utlisent ces fonctions doivent être données en TEMPS UNIVERSEL
// ------------------------------------------------------------------------------------

QString Calculastro::enumQtToPays(QString ref)
{
    QFile fichier("qlocale-pays.txt");
    QString retour;
    if (fichier.open(QIODevice::ReadOnly)) {
        QTextStream flux(&fichier);
        QString ligne;
        QStringList infos;
        while (!flux.atEnd()) {
            ligne = flux.readLine();
            infos = ligne.split("|");

            if (infos.at(0) == ref) {
                if (infos.at(1) != "--")
                    retour = infos.at(1);

                return retour;
            }
        }
        return retour;
    }
    return retour;
}
QString Calculastro::enumQtToPays(QLocale::Country country)
{
    return enumQtToPays(QLocale::countryToString(country));
}
QString Calculastro::paysToEnumQt(QString ref)
{
    QFile fichier("qlocale-pays.txt");
    QString retour;
    if (fichier.open(QIODevice::ReadOnly) && ref != "--") {
        QTextStream flux(&fichier);
        QString ligne;
        QStringList infos;
        while (!flux.atEnd()) {
            ligne = flux.readLine();
            infos = ligne.split("|");

            if (infos.at(1) == ref) {
                retour = infos.at(0);
                return retour;
            }
        }
        return retour;
    }
    return retour;
}
QVector<QString> Calculastro::trouverVillePays(double latitude, double longitude)
{
    QVector<QString> retour, retour1;

    QSqlQuery requete("SELECT nom, pays FROM villes_monde WHERE latitude = :lat AND longitude = :long");
    requete.bindValue(":lat", latitude);
    requete.bindValue(":long", longitude);
    requete.exec();
    if (requete.size() > 0) {
        requete.next();
        retour.push_back(requete.value(0).toString());
        retour.push_back(requete.value(1).toString());
        return retour;
    }

    requete.prepare("SELECT nom FROM villes_france WHERE latitude = :lat AND longitude = :long");
    requete.bindValue(":lat", latitude);
    requete.bindValue(":long", longitude);
    requete.exec();
    if (requete.size() > 0) {
        requete.next();
        retour.push_back(requete.value(0).toString());
        retour.push_back("France");
        return retour;
    }

    double ecartMin(40), ecartMin2(40), ecart(0), ecart2(0);

    requete.prepare("SELECT nom, pays, latitude, longitude FROM villes_monde WHERE latitude < :latitudeMax AND latitude > :latitudeMin AND longitude < :longitudeMax AND longitude > :longitudeMin");
    requete.bindValue(":latitudeMin", latitude - 10);
    requete.bindValue(":longitudeMin", longitude - 10);
    requete.bindValue(":latitudeMax", latitude + 10);
    requete.bindValue(":longitudeMax", longitude + 10);
    requete.exec();
    while (requete.next()) {
        ecart = qAbs(requete.value(2).toDouble() - latitude) + qAbs(requete.value(3).toDouble() - longitude);
        if (ecartMin > ecart) {
            ecartMin = ecart;
            retour.clear();
            retour.push_back(requete.value(0).toString());
            retour.push_back(requete.value(1).toString());
        }
    }
    if (retour.at(1) == "France") {
        requete.exec("SELECT nom, latitude, longitude FROM villes_france");
        while (requete.next()) {
            ecart2 = qAbs(requete.value(1).toDouble() - latitude) + qAbs(requete.value(2).toDouble() - longitude);
            if (ecart2 < ecartMin2) {
                ecartMin2 = ecart2;
                retour1.clear();
                retour1.push_back(requete.value(0).toString());
                retour1.push_back("France");
            }
        }
    }
    if (ecartMin2 < ecartMin)
        return retour1;
    else
        return retour;
}
double Calculastro::deg2rad(double deg)
{
    return AstroCalc::deg2rad(deg);
}
double Calculastro::rad2deg(double rad)
{
    return 180 * rad / M_PI;
}
double Calculastro::julianDay(QDate date, QTime heure) // OK
{
    double A(0), B(0), T(0), jd(0);

    int y(date.year());
    int m(date.month());
    int d(date.day());

    int h(heure.hour());
    int min(heure.minute());
    int s(heure.second());

    if (m < 3) {
        y -= 1;
        m += 12;
    }
    A = partieEntiere(y / 100);
    B = 2 - A + partieEntiere(A / 4);
    T = h / 24.0 + min / 1440.0 + s / 86400.0;

    if (y < 1582) // Si on est encore dans le calendrier Julien
        B = 0;

    jd = partieEntiere(365.25 * (y + 4716)) + partieEntiere(30.6001 * (m + 1)) + d + T + B - 1524.5;
    return jd;
}
double Calculastro::julianDay2000(QDate date, QTime heure)
{
    double j2000(0), jd(0);
    jd = julianDay(date, heure);
    j2000 = jd - 2451545.0;
    return j2000;
}
double Calculastro::partieDecimal(double val)
{
    if (val < 0)
        return val - ceil(val);
    else
        return val - floor(val);
}
double Calculastro::partieEntiere(double val)
{
    if (val < 0)
        return ceil(val);
    else
        return floor(val);
}
double Calculastro::heureSiderale(QDate date, QTime heure, bool apparent)
{ // La méthode de calcul présentée ici est celle de l'exemple 11.b p.85
    double T(0), JD(0), Oo(0), HSH(0);

    JD = julianDay(date, heure);
    T = (JD - 2451545.0) / 36525.0;

    // Formule (11.4)
    Oo = 280.46061837 + (360.98564736629 * (JD - 2451545.0)) + (0.000387933 * T * T) - ((T * T * T) / 38710000);
    // Puis on le ramène dans un intervalle entre 0 et 360°
    while (Oo < 0 || Oo >= 360) {
        if (Oo < 0)
            Oo += 360;
        else
            Oo -= 360;
    }
    HSH = Oo / 15; // Il était en degrés, pour obtenir en heures, on divise par 15

    if (apparent) // Si on veut temps sidéral APPARENT, il faut corriger la valeur
    {
        QVector<double> nuOb = nutationObliquity(JD - 2451545.0);
        double correction = (nuOb[0] * cos(deg2rad(nuOb[2]))) / 15; // en secondes
        HSH += correction / 3600;
    }
    return HSH; // ATTENTION IL FAUT CORRIGER CETTE VALEUR (voir p.84)
}
double Calculastro::toAngleHoraireDegree(QDate date, QTime heure, double ra, double longi)
{ // Pour vérification, voir exemple 12.b p.91
    double angleHoraire(0);

    angleHoraire = heureSiderale(date, heure, true) + longi / 15.0 - ra / 15.0; // résultat en heures
    angleHoraire *= 15.0; // on le convertit en degrés
    angleHoraire = toZero360(angleHoraire); // On le ramène dans un intervalle entre 0 et 360°

    return angleHoraire;
}
QVector<double> Calculastro::hauteurAzimutDegree(QDate const &date, QTime const &heure, double ra, double dec, double lat, double longi)
{
    std::vector<double> radecCorriger;
    double angleHoraireDegre(0), sinDec(0), sinLat(0), cosDec(0), cosLat(0), cosHA(0), sinHA(0), tanDec(0);
    double sinHauteur(0), hauteur(0), azimuth(0);

    radecCorriger = RaDecPrecession(date.year(), ra, dec); // OK

    angleHoraireDegre = toAngleHoraireDegree(date, heure, ra, longi);

    sinDec = sin(deg2rad(radecCorriger[1]));
    sinLat = sin(deg2rad(lat));
    cosDec = cos(deg2rad(radecCorriger[1]));
    tanDec = tan(deg2rad(radecCorriger[1]));
    cosLat = cos(deg2rad(lat));
    cosHA = cos(deg2rad(angleHoraireDegre));
    sinHA = sin(deg2rad(angleHoraireDegre));

    sinHauteur = (sinDec * sinLat) + (cosDec * cosLat * cosHA); // Formule (12.6) p.89 Astronomical Algorithms
    hauteur = asin(sinHauteur);

    azimuth = atan2(sinHA, (cosHA * sinLat - tanDec * cosLat)); // Formule (12.5) p.89

    hauteur = rad2deg(hauteur);
    azimuth = rad2deg(azimuth);
    azimuth = toZero360(azimuth + 180); // on rajoute 180

    QVector<double> altAz;
    altAz.push_back(hauteur);
    altAz.push_back(azimuth);
    altAz.push_back(angleHoraireDegre);

    return altAz;
}
std::vector<double> Calculastro::RaDecPrecession(int year, double ra, double dec)
{
    int deltaAnnee(0);
    double m(0), n(0), deltaRa(0), deltaDec(0);

    deltaAnnee = year - 2000;
    m = 3.075; // en seconde
    n = 20.04; // en seconde degree

    deltaRa = m + n * sin(deg2rad(ra)) * tan(deg2rad(dec));
    deltaDec = n * cos(deg2rad(ra));

    ra = ra + (deltaRa * deltaAnnee) / 3600.0;

    dec = dec + (deltaDec * deltaAnnee) / 3600.0;

    std::vector<double> RaDec;
    RaDec.push_back(ra);
    RaDec.push_back(dec);
    return RaDec;
}

QString Calculastro::abreviationToNom(QString abreviation, bool inverse)
{
    QMap<QString, QString> constellations;
    constellations.insert("Aql", QObject::tr("Aigle", "Constellation"));
    constellations.insert("And", QObject::tr("Andromède", "Constellation"));
    constellations.insert("Ara", QObject::tr("Autel", "Constellation"));
    constellations.insert("Lib", QObject::tr("Balance", "Constellation"));
    constellations.insert("Cet", QObject::tr("Baleine", "Constellation"));
    constellations.insert("Ari", QObject::tr("Bélier", "Constellation"));
    constellations.insert("Pyx", QObject::tr("Boussole", "Constellation"));
    constellations.insert("Boo", QObject::tr("Bouvier", "Constellation"));
    constellations.insert("Cae", QObject::tr("Burin", "Constellation"));
    constellations.insert("Cha", QObject::tr("Caméléon", "Constellation"));
    constellations.insert("Cnc", QObject::tr("Cancer", "Constellation"));
    constellations.insert("Cap", QObject::tr("Capricorne", "Constellation"));
    constellations.insert("Car", QObject::tr("Carène", "Constellation"));
    constellations.insert("Cas", QObject::tr("Cassiopée", "Constellation"));
    constellations.insert("Cen", QObject::tr("Centaure", "Constellation"));
    constellations.insert("Cep", QObject::tr("Céphée", "Constellation"));
    constellations.insert("Com", QObject::tr("Chevelure de Bérénice", "Constellation"));
    constellations.insert("CVn", QObject::tr("Chiens de chasse", "Constellation"));
    constellations.insert("Aur", QObject::tr("Cocher", "Constellation"));
    constellations.insert("Col", QObject::tr("Colombe", "Constellation"));
    constellations.insert("Cir", QObject::tr("Compas", "Constellation"));
    constellations.insert("Crv", QObject::tr("Corbeau", "Constellation"));
    constellations.insert("Crt", QObject::tr("Coupe", "Constellation"));
    constellations.insert("CrA", QObject::tr("Couronne australe", "Constellation"));
    constellations.insert("CrB", QObject::tr("Couronne boréale", "Constellation"));
    constellations.insert("Cru", QObject::tr("Croix du Sud", "Constellation"));
    constellations.insert("Cyg", QObject::tr("Cygne", "Constellation"));
    constellations.insert("Del", QObject::tr("Dauphin", "Constellation"));
    constellations.insert("Dor", QObject::tr("Dorade", "Constellation"));
    constellations.insert("Dra", QObject::tr("Dragon", "Constellation"));
    constellations.insert("Sct", QObject::tr("Ecu de Sobieski", "Constellation"));
    constellations.insert("Eri", QObject::tr("Eridan", "Constellation"));
    constellations.insert("Sge", QObject::tr("Flèche", "Constellation"));
    constellations.insert("For", QObject::tr("Fourneau", "Constellation"));
    constellations.insert("Gem", QObject::tr("Gémeaux", "Constellation"));
    constellations.insert("Cam", QObject::tr("Girafe", "Constellation"));
    constellations.insert("CMa", QObject::tr("Grand Chien", "Constellation"));
    constellations.insert("UMa", QObject::tr("Grande Ourse", "Constellation"));
    constellations.insert("Gru", QObject::tr("Grue", "Constellation"));
    constellations.insert("Her", QObject::tr("Hercule", "Constellation"));
    constellations.insert("Hor", QObject::tr("Horloge", "Constellation"));
    constellations.insert("Hya", QObject::tr("Hydre", "Constellation"));
    constellations.insert("Hyi", QObject::tr("Hydre mâle", "Constellation"));
    constellations.insert("Ind", QObject::tr("Indien", "Constellation"));
    constellations.insert("Lac", QObject::tr("Lézard", "Constellation"));
    constellations.insert("Mon", QObject::tr("Licorne", "Constellation"));
    constellations.insert("Lep", QObject::tr("Lièvre", "Constellation"));
    constellations.insert("Leo", QObject::tr("Lion", "Constellation"));
    constellations.insert("Lup", QObject::tr("Loup", "Constellation"));
    constellations.insert("Lyn", QObject::tr("Lynx", "Constellation"));
    constellations.insert("Lyr", QObject::tr("Lyre", "Constellation"));
    constellations.insert("Ant", QObject::tr("Machine Pneumatique", "Constellation"));
    constellations.insert("Mic", QObject::tr("Microscope", "Constellation"));
    constellations.insert("Mus", QObject::tr("Mouche", "Constellation"));
    constellations.insert("Oct", QObject::tr("Octant", "Constellation"));
    constellations.insert("Aps", QObject::tr("Oiseau de paradis", "Constellation"));
    constellations.insert("Oph", QObject::tr("Ophiuchus", "Constellation"));
    constellations.insert("Ori", QObject::tr("Orion", "Constellation"));
    constellations.insert("Pav", QObject::tr("Paon", "Constellation"));
    constellations.insert("Peg", QObject::tr("Pégase", "Constellation"));
    constellations.insert("Pic", QObject::tr("Peintre", "Constellation"));
    constellations.insert("Per", QObject::tr("Persée", "Constellation"));
    constellations.insert("Equ", QObject::tr("Petit Cheval", "Constellation"));
    constellations.insert("CMi", QObject::tr("Petit Chien", "Constellation"));
    constellations.insert("LMi", QObject::tr("Petit Lion", "Constellation"));
    constellations.insert("Vul", QObject::tr("Petit Renard", "Constellation"));
    constellations.insert("UMi", QObject::tr("Petite Ourse", "Constellation"));
    constellations.insert("Phe", QObject::tr("Phénix", "Constellation"));
    constellations.insert("PsA", QObject::tr("Poisson Austral", "Constellation"));
    constellations.insert("Vol", QObject::tr("Poisson volant", "Constellation"));
    constellations.insert("Psc", QObject::tr("Poissons", "Constellation"));
    constellations.insert("Pup", QObject::tr("Poupe", "Constellation"));
    constellations.insert("Nor", QObject::tr("Règle", "Constellation"));
    constellations.insert("Ret", QObject::tr("Réticule", "Constellation"));
    constellations.insert("Sgr", QObject::tr("Sagittaire", "Constellation"));
    constellations.insert("Sco", QObject::tr("Scorpion", "Constellation"));
    constellations.insert("Scl", QObject::tr("Sculpteur", "Constellation"));
    constellations.insert("Ser", QObject::tr("Serpent", "Constellation"));
    constellations.insert("Sex", QObject::tr("Sextant", "Constellation"));
    constellations.insert("Men", QObject::tr("Table", "Constellation"));
    constellations.insert("Tau", QObject::tr("Taureau", "Constellation"));
    constellations.insert("Tel", QObject::tr("Télescope", "Constellation"));
    constellations.insert("Tuc", QObject::tr("Toucan", "Constellation"));
    constellations.insert("Tri", QObject::tr("Triangle", "Constellation"));
    constellations.insert("TrA", QObject::tr("Triangle Austral", "Constellation"));
    constellations.insert("Aqr", QObject::tr("Verseau", "Constellation"));
    constellations.insert("Vir", QObject::tr("Vierge", "Constellation"));
    constellations.insert("Vel", QObject::tr("Voiles", "Constellation"));
    constellations.insert("-", "-");

    if (!inverse)
        return constellations[abreviation];
    else
        return constellations.key(abreviation);
}
QVector<double> Calculastro::nutationObliquity(double j2000)
{ // Toutes ces formules sont données à la page 132
    double t(0), omega(0), L(0), LP(0), psi(0), dEta(0), eta0(0), eta(0);

    t = j2000 / 36525.0;

    omega = 125.04452 - 1934.136261 * t + 0.0020708 * pow(t, 2.0) + pow(t, 3.0) / 450000;
    omega = toZero360(omega);

    omega = deg2rad(omega);

    L = deg2rad(280.4665 + 36000.7698 * t);
    LP = deg2rad(218.3165 + 481267.8813 * t);

    psi = -17.2 * sin(omega) - 1.32 * sin(2 * L) - 0.23 * sin(2 * LP) + 0.21 * sin(2 * omega);
    dEta = (9.20) * cos(omega) + (0.57) * cos(2 * L) + (0.10) * cos(2 * LP) - (0.09) * cos(2 * omega);
    eta0 = 23.439291 - (46.8150 / 3600) * t - (0.00059 / 36) * pow(t, 2.0) + (0.001813 / 3600) * pow(t, 3.0); // formule (21.2)
    eta = eta0 + (dEta / 3600);

    QVector<double> retour;
    retour.push_back(psi); // en secondes d'arc
    retour.push_back(eta0); // en degrés
    retour.push_back(eta); // en degrés

    return retour;
}
QVector<double> Calculastro::coordonneesPlanetes(QDate date, QTime heure, QString planete)
{
    double jj(0), x(0), y(0), z(0), delta(0), t(0), lambda(0), tanBeta(0), beta(0), alpha(0);
    double k(0), i(0), declinaison(0), mag(0);
    QVector<double> retour;
    QVector<double> nuOb;
    QMap<char, double> E, O;

    jj = julianDay2000(date, heure);
    O = getLBR(jj, "terre");
    E = getLBR(jj, planete);

    E['L'] = deg2rad(E['L']);
    E['B'] = deg2rad(E['B']);
    O['L'] = deg2rad(O['L']);
    O['B'] = deg2rad(O['B']);

    x = E['R'] * cos(E['B']) * cos(E['L']) - O['R'] * cos(O['B']) * cos(O['L']);
    y = E['R'] * cos(E['B']) * sin(E['L']) - O['R'] * cos(O['B']) * sin(O['L']);
    z = E['R'] * sin(E['B']) - O['R'] * sin(O['B']);
    // Formules (32.1) p.209

    delta = sqrt(pow(x, 2.0) + pow(y, 2.0) + pow(z, 2.0)); // distance à la terre - formule (32.4) p.210
    t = (delta * 0.0057755183); // Formule (32.3) p.210 Astronimal Algorithms

    // La correction de la vitesse de la lumière a été faite - On recommence les calculs
    jj -= t;
    O = getLBR(jj, "terre");
    E = getLBR(jj, planete);

    E['L'] = deg2rad(E['L']);
    E['B'] = deg2rad(E['B']);
    O['L'] = deg2rad(O['L']);
    O['B'] = deg2rad(O['B']);

    x = E['R'] * cos(E['B']) * cos(E['L']) - O['R'] * cos(O['B']) * cos(O['L']); // ok
    y = E['R'] * cos(E['B']) * sin(E['L']) - O['R'] * cos(O['B']) * sin(O['L']); // ok
    z = E['R'] * sin(E['B']) - O['R'] * sin(O['B']); // ok

    lambda = rad2deg(atan(y / x));
    if (x < 0) {
        lambda += 180;
    } // supprime une erreur avec l'arctangeant
    if (lambda < 0)
        lambda += 360;
    tanBeta = z / (sqrt(pow(x, 2.0) + pow(y, 2.0)));
    beta = atan(tanBeta); // ok

    nuOb = nutationObliquity(jj);
    lambda += nuOb[0] / 3600;
    lambda = deg2rad(lambda);
    nuOb[2] = deg2rad(nuOb[2]);

    alpha = atan((sin(lambda) * cos(nuOb[2]) - tan(beta) * sin(nuOb[2])) / cos(lambda)); // Formule (12.3) p.89
    if (cos(lambda) < 0)
        alpha += M_PI; // supprime une erreur avec l'arctangeant
    declinaison = asin(sin(beta) * cos(nuOb[2]) + cos(beta) * sin(nuOb[2]) * sin(lambda)); // Formule (12.4) p.89

    alpha = toZero360(rad2deg(alpha));
    declinaison = rad2deg(declinaison);

    k = (pow(E['R'] + delta, 2.0) - pow(O['R'], 2.0)) / (4 * E['R'] * delta) * 100; // formule (40.2) p.267
    i = rad2deg(acos((pow(E['R'], 2) + pow(delta, 2) - pow(O['R'], 2)) / (2 * E['R'] * delta))); // formule (40.1) p.267

    if (planete == "mercure")
        mag = 1.16 + 5 * log10(E['R'] * delta) + 0.02838 * (i - 50) + 0.0001023 * pow(i - 50, 2.0);
    else if (planete == "venus")
        mag = -4.40 + 5 * log10(E['R'] * delta) + 0.0009 * i + 0.000239 * pow(i, 2.0) - 0.00000065 * pow(i, 3.0);
    else if (planete == "mars")
        mag = -1.52 + 5 * log10(E['R'] * delta) + 0.016 * i;
    else if (planete == "jupiter")
        mag = -9.40 + 5 * log10(E['R'] * delta) + 0.005 * i;
    else if (planete == "saturne")
        mag = -8.88 + 5 * log10(E['R'] * delta) + 0.044 * i;
    else if (planete == "uranus")
        mag = -7.19 + 5 * log10(E['R'] * delta);
    else if (planete == "neptune")
        mag = -6.87 + 5 * log10(E['R'] * delta);
    else
        mag = 0; // p.269

    retour.push_back(alpha);
    retour.push_back(declinaison);
    retour.push_back(mag);
    retour.push_back(k);
    retour.push_back(delta);
    return retour;
}
QMap<char, double> Calculastro::getLBR(double j2000, QString planete)
{
    double t(0), L(0), B(0), R(0);
    QString ext;

    t = j2000 / 365250;

    if (planete == "mercure")
        ext = "mer";
    else if (planete == "venus")
        ext = "ven";
    else if (planete == "mars")
        ext = "mar";
    else if (planete == "jupiter")
        ext = "jup";
    else if (planete == "saturne")
        ext = "sat";
    else if (planete == "uranus")
        ext = "ura";
    else if (planete == "neptune")
        ext = "nep";
    else if (planete == "terre")
        ext = "ear";
    else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"), QObject::tr("La planète demandée n'existe pas. Le programme va fermer."));
        qApp->exit();
    }

    // Ouverture du fichier
    QFile fichier("VSOP87/VSOP87B." + ext);
    if (!fichier.open(QIODevice::ReadOnly | QIODevice::Text))
        QMessageBox::critical(nullptr, QObject::tr("Erreur d'ouverture", "N'arrive pas à ouvrir un fichier"), "Le fichier VSOP87/VSOP87B." + ext + " est impossible à ouvrir. Réésayer.");
    QTextStream flux(&fichier);
    QString ligne;

    QString lettres[3];
    lettres[0] = "L";
    lettres[1] = "B";
    lettres[2] = "R";
    QMap<QString, double> series;
    series.insert("L0", 0);
    series.insert("L1", 0);
    series.insert("L2", 0);
    series.insert("L3", 0);
    series.insert("L4", 0);
    series.insert("L5", 0);
    series.insert("B0", 0);
    series.insert("B1", 0);
    series.insert("B2", 0);
    series.insert("B3", 0);
    series.insert("B4", 0);
    series.insert("B5", 0);
    series.insert("R0", 0);
    series.insert("R1", 0);
    series.insert("R2", 0);
    series.insert("R3", 0);
    series.insert("R4", 0);
    series.insert("R5", 0);
    QStringList liste;
    int num(0);
    double A(0), B0(0), C(0), valeur(0);
    QString serie;

    while (!flux.atEnd()) {
        ligne = flux.readLine();
        //traitement de la ligne
        liste = ligne.split("|", Qt::SkipEmptyParts);
        if (liste[0] != "VSOP87") {
            num = liste.count();
            A = liste.at(num - 3).toDouble();
            B0 = liste.at(num - 2).toDouble();
            C = liste.at(num - 1).toDouble();
            valeur = A * cos(B0 + C * t);

            series[serie] += valeur;
        } else {
            serie = lettres[liste.at(5).toInt() - 1] + "" + liste.at(7).right(1);
        }
    }
    L = series["L0"] + series["L1"] * t + series["L2"] * pow(t, 2) + series["L3"] * pow(t, 3) + series["L4"] * pow(t, 4) + series["L5"] * pow(t, 5);
    B = series["B0"] + series["B1"] * t + series["B2"] * pow(t, 2) + series["B3"] * pow(t, 3) + series["B4"] * pow(t, 4) + series["B5"] * pow(t, 5);
    R = series["R0"] + series["R1"] * t + series["R2"] * pow(t, 2) + series["R3"] * pow(t, 3) + series["R4"] * pow(t, 4) + series["R5"] * pow(t, 5);

    L = toZero360(rad2deg(L)); // On convertit en degrés
    B = rad2deg(B); // On convertit en degrés

    QMap<char, double> retour;
    retour.insert('L', L);
    retour.insert('B', B);
    retour.insert('R', R);
    return retour;
}
double Calculastro::toZero360(double deg)
{
    while (deg > 360 || deg < 0) {
        if (deg > 360)
            deg -= 360;
        else
            deg += 360;
    }
    return deg;
}
QString Calculastro::degreeToDms(double val)
{
    int d(0), m(0), s(0);
    QString signe;

    if (val < 0) {
        signe = "-";
        val *= -1.0;
    } else
        signe = "";

    d = floor(val);
    m = floor((val - d) * 60);
    s = floor(((val - d) * 60 - m) * 60);

    QString retour;
    retour = signe + QString::number(d) + "° " + QString::number(m) + "' " + QString::number(s) + "\"";
    return retour;
}
QString Calculastro::degreeToHms(double val)
{
    int h(0), m(0), s(0);
    QString signe;

    val /= 15;

    if (val < 0) {
        signe = "-";
        val *= -1.0;
    } else
        signe = "";

    h = floor(val);
    m = floor((val - h) * 60);
    s = floor(((val - h) * 60 - m) * 60);

    QString retour;
    retour = signe + QString::number(h) + "h " + QString::number(m) + "min " + QString::number(s) + "s ";
    return retour;
}
double Calculastro::round(double n, unsigned d)
{
    return floor(n * pow(10., d) + .5) / pow(10., d);
}
QString Calculastro::referencePlaneteToNom(QString ref, bool read)
{
    QString retour;
    if (ref == "P1")
        retour = QObject::tr("mercure");
    else if (ref == "P2")
        retour = QObject::tr("venus");
    else if (ref == "P3")
        retour = QObject::tr("mars");
    else if (ref == "P4")
        retour = QObject::tr("jupiter");
    else if (ref == "P5")
        retour = QObject::tr("saturne");
    else if (ref == "P6")
        retour = QObject::tr("uranus");
    else if (ref == "P7")
        retour = QObject::tr("neptune");
    else {
        retour = "mercure";
        QMessageBox::critical(nullptr, QObject::tr("Problème de référence"), QObject::tr("La référence de la planète est inconnue. Contacter le developpeur."));
    }
    if (read)
        retour = retour.at(0).toUpper() + retour.right(retour.count() - 1); // On met en majuscule la 1ere lettre
    return retour;
}
HauteurMaxTemps Calculastro::hauteurMaxObjet(Objet *objet, QDateTime debut, QDateTime fin, double latitude, double longitude)
{
    // Cette fonction marche pour les objets
    double raDecimal(0), declinaisonDecimal(0);
    QDate dateTU1, dateTU2, dateTU3, dateTU4;
    QTime heureTU1, heureTU2, heureTU3, heureTU4;
    QVector<double> hauteurAzimut1, hauteurAzimut2, hauteurAzimut3, hauteurAzimut4, hauteurAzimut;

    raDecimal = objet->ascdrDouble();
    declinaisonDecimal = objet->decDouble();

    dateTU1 = debut.date();
    heureTU1 = debut.time();
    hauteurAzimut1 = hauteurAzimutDegree(dateTU1, heureTU1, raDecimal, declinaisonDecimal, latitude, longitude);

    dateTU2 = debut.addSecs(240).date();
    heureTU2 = debut.addSecs(240).time();
    hauteurAzimut2 = hauteurAzimutDegree(dateTU2, heureTU2, raDecimal, declinaisonDecimal, latitude, longitude);

    dateTU3 = fin.addSecs(-240).date();
    heureTU3 = fin.addSecs(-240).time();
    hauteurAzimut3 = hauteurAzimutDegree(dateTU3, heureTU3, raDecimal, declinaisonDecimal, latitude, longitude);

    dateTU4 = fin.date();
    heureTU4 = fin.time();
    hauteurAzimut4 = hauteurAzimutDegree(dateTU4, heureTU4, raDecimal, declinaisonDecimal, latitude, longitude);

    if (hauteurAzimut1[0] > hauteurAzimut2[0] && hauteurAzimut3[0] > hauteurAzimut4[0] && hauteurAzimut2[0] > hauteurAzimut3[0]) { // Si la hauteur décrôit avec le temps
        return HauteurMaxTemps(debut, hauteurAzimut1[0]);
    } else if (hauteurAzimut1[0] < hauteurAzimut2[0] && hauteurAzimut3[0] < hauteurAzimut4[0] && hauteurAzimut2[0] < hauteurAzimut3[0]) { // Si la hauteur croît avec le temps
        return HauteurMaxTemps(fin, hauteurAzimut4[0]);
    } else {
        HauteurMaxTemps hauteurMaxVar(debut, 0);
        QDateTime cursor;
        cursor = debut;

        while (cursor <= fin) {
            hauteurAzimut = hauteurAzimutDegree(cursor.date(), cursor.time(), raDecimal, declinaisonDecimal, latitude, longitude);
            if (hauteurMaxVar.hauteurMax() < hauteurAzimut[0]) {
                hauteurMaxVar.setHauteurMax(hauteurAzimut[0]);
                hauteurMaxVar.setDateTime(cursor);
            }
            cursor = cursor.addSecs(60);
        }
        return hauteurMaxVar;
    }
}
HauteurMaxTemps Calculastro::hauteurMaxObjet(QString const &planete, QDateTime debut, QDateTime fin, double latitude, double longitude)
{
    // Cette fonction marche pour les objets
    double raDecimal(0), declinaisonDecimal(0);
    QDate dateTU1, dateTU2, dateTU3, dateTU4;
    QTime heureTU1, heureTU2, heureTU3, heureTU4;
    QVector<double> hauteurAzimut1, hauteurAzimut2, hauteurAzimut3, hauteurAzimut4, hauteurAzimut;

    QVector<double> infosPlanete;
    infosPlanete = coordonneesPlanetes(debut.date(), debut.time(), planete);

    raDecimal = infosPlanete.at(0);
    declinaisonDecimal = infosPlanete.at(1);

    dateTU1 = debut.date();
    heureTU1 = debut.time();
    hauteurAzimut1 = hauteurAzimutDegree(dateTU1, heureTU1, raDecimal, declinaisonDecimal, latitude, longitude);

    dateTU2 = debut.addSecs(240).date();
    heureTU2 = debut.addSecs(240).time();
    hauteurAzimut2 = hauteurAzimutDegree(dateTU2, heureTU2, raDecimal, declinaisonDecimal, latitude, longitude);

    dateTU3 = fin.addSecs(-240).date();
    heureTU3 = fin.addSecs(-240).time();
    hauteurAzimut3 = hauteurAzimutDegree(dateTU3, heureTU3, raDecimal, declinaisonDecimal, latitude, longitude);

    dateTU4 = fin.date();
    heureTU4 = fin.time();
    hauteurAzimut4 = hauteurAzimutDegree(dateTU4, heureTU4, raDecimal, declinaisonDecimal, latitude, longitude);

    if (hauteurAzimut1[0] > hauteurAzimut2[0] && hauteurAzimut3[0] > hauteurAzimut4[0] && hauteurAzimut2[0] > hauteurAzimut3[0]) { // Si la hauteur décrôit avec le temps
        return HauteurMaxTemps(debut, hauteurAzimut1[0]);
    } else if (hauteurAzimut1[0] < hauteurAzimut2[0] && hauteurAzimut3[0] < hauteurAzimut4[0] && hauteurAzimut2[0] < hauteurAzimut3[0]) { // Si la hauteur croît avec le temps
        return HauteurMaxTemps(fin, hauteurAzimut4[0]);
    } else {
        HauteurMaxTemps hauteurMaxVar(debut, 0);
        QDateTime cursor;
        cursor = debut;

        while (cursor <= fin) {
            hauteurAzimut = hauteurAzimutDegree(cursor.date(), cursor.time(), raDecimal, declinaisonDecimal, latitude, longitude);
            if (hauteurMaxVar.hauteurMax() < hauteurAzimut[0]) {
                hauteurMaxVar.setHauteurMax(hauteurAzimut[0]);
                hauteurMaxVar.setDateTime(cursor);
            }
            cursor = cursor.addSecs(60);
        }
        return hauteurMaxVar;
    }
}
bool Calculastro::verifDisponibilite(QVector<ObjetObs *> const &liste_observation, QDateTime const &debut, QDateTime const &fin, int espace)
{
    for (int i(0); i < liste_observation.size(); i++) {
        if ((debut < liste_observation[i]->getFin().addSecs(espace) && debut > liste_observation[i]->getDebut().addSecs(-espace)) || (fin < liste_observation[i]->getFin().addSecs(espace) && fin > liste_observation[i]->getDebut().addSecs(-espace)))
            return false;
    }
    return true;
}
double Calculastro::noterObjetVisible(QString type, int interet, double magnitude, int diametre, QString niveau, double hauteurMax, int difficulte, QSettings *user)
{
    int nd(0), numNiveau(0);
    double note(0), magNiveau(0), magMax(0), ecart(0), noteVariable(0);

    // ON NOTE SUR LE TYPE
    if (type == "Amas Globulaire")
        note += user->value("generateur/amasGlobulaire", NOTE_AMAS_GLOBULAIRE).toInt();
    else if (type == "Amas+Nébuleuse")
        note += user->value("generateur/amasNebuleuse", NOTE_AMAS_NEBULEUSE).toInt();
    else if (type == "Nébuleuse planétaire")
        note += user->value("generateur/nebuleusePlanetaire", NOTE_NEBULEUSE_PLANETAIRE).toInt();
    else if (type == "Nébuleuse en réflection")
        note += user->value("generateur/nebuleuseReflection", NOTE_NEBULEUSE_REFLECTION).toInt();
    else if (type == "Etoile triple")
        note += user->value("generateur/etoileTriple", NOTE_ETOILE_TRIPLE).toInt();
    else if (type == "Etoile double")
        note += user->value("generateur/etoileDouble", NOTE_ETOILE_DOUBLE).toInt();
    else if (type == "Galaxie")
        note += user->value("generateur/galaxie", NOTE_GALAXIE).toInt();
    else if (type == "Amas ouverts")
        note += user->value("generateur/amasOuvert", NOTE_AMAS_OUVERT).toInt();

    // ON NOTE SUR L'INTERET
    switch (interet) {
    case 4:
        note += 20;
        break;
    case 3:
        note += 10;
        break;
    case 2:
        note += 5;
        break;
    case 1:
        note += 0;
        break;
    }

    // ON NOTE SUR LA HAUTEUR MAX
    if (hauteurMax > 0 && hauteurMax < 90)
        note += hauteurMax / 4.5; // Note sur 20 (0 => 0, 90 => 20)

    // ON NOTE SUR LA MAGNITUDE, LE NIVEAU ET LE DIAMETRE
    magMax = round(2 + 5 * log10(diametre), 1);

    if (niveau == "debutant") {
        magNiveau = magMax * 0.5;
        numNiveau = 1;
    }
    if (niveau == "amateur") {
        magNiveau = magMax * 0.6;
        numNiveau = 2;
    }
    if (niveau == "amateurc") {
        magNiveau = magMax * 0.7;
        numNiveau = 3;
    }
    if (niveau == "expert") {
        magNiveau = magMax * 0.8;
        numNiveau = 4;
    }

    if (magnitude > magMax) // Si la magnitude est plus grande que la magnitude max, on ne peut pas observer l'objet on renvoie la note actuelle
        return note;

    ecart = magnitude - magNiveau; // on calcule l'ecart entre la magnitude de l'objet la magnitude du niveau
    if (ecart < 0)
        ecart = -ecart;

    noteVariable = (1 - (ecart / magNiveau)) * 30;
    if (noteVariable > 30)
        noteVariable = 30;
    else if (noteVariable < 0)
        noteVariable = 0;
    note += noteVariable;

    // ON NOTE SUR LE NIVEAU ET LA DIFFICULTE
    if (difficulte > 0) {
        nd = numNiveau - difficulte;
        if (nd >= 0)
            note += 15;
        else
            note += (nd + 3) * 5;
    }
    return round(note, 2);
}
bool Calculastro::isObjetVisible(Objet *objet, QDateTime temps, double latitude, double longitude)
{
    QVector<double> hauAzi;
    hauAzi = hauteurAzimutDegree(temps.date(), temps.time(), objet->ascdrDouble(), objet->decDouble(), latitude, longitude);
    if (hauAzi[0] > 0)
        return true;
    else
        return false;
}
int Calculastro::miseEnTemperature(int diametre)
{
    if (diametre > 0 && diametre <= 50)
        return 15;
    else if (diametre <= 100)
        return 30;
    else if (diametre <= 150)
        return 45;
    else if (diametre <= 200)
        return 60;
    else if (diametre <= 250)
        return 80;
    else if (diametre <= 300)
        return 100;
    else if (diametre > 300)
        return 120;
    else
        return -1;
}
QString Calculastro::getOculaire(Objet *objet, int diametre, int focale, QVector<int> oculaires)
{
    double fd(0), f(0);

    if (diametre != 0) // On verifie qu'on effectue pas une division par 0
        fd = round(focale / diametre, 0);
    else
        return "Aucun";

    if (objet->taille() != 0 && objet->magnitude() != 0) {
        if (objet->magnitude() <= 5) // Si il est visible à l'oeil nu
        {
            if (objet->taille() >= 60)
                f = 5 * fd;
            else if (objet->taille() >= 30)
                f = 4 * fd;
            else if (objet->taille() >= 10)
                f = 2 * fd;
            else
                f = 1.5 * fd;
        } else if (objet->magnitude() <= 8) // Si il est très lumineux
        {
            if (objet->taille() >= 50)
                f = 3 * fd;
            else if (objet->taille() >= 20)
                f = 2 * fd;
            else
                f = 1.2 * fd;
        } else if (objet->magnitude() < 10) // Si il est lumineux
        {
            if (objet->taille() >= 100)
                f = 3 * fd;
            else if (objet->taille() >= 50) // Et grand
                f = 2 * fd;
            else
                f = 1.8 * fd;
        } else if (objet->magnitude() < 12) // SI il est moyennement lumineux
        {
            if (objet->taille() >= 50)
                f = 2.5 * fd;
            else
                f = 1.75 * fd;
        } else if (objet->magnitude() < 15) // Si il est peu lumineux
        {
            if (objet->taille() >= 50)
                f = 2.5 * fd;
            else
                f = 2 * fd;
        } else {
            if (objet->taille() >= 50)
                f = 2.3 * fd;
            else
                f = 2.1 * fd;
        }
        f = round(f, 3);
        int bonOculaire[2];
        bonOculaire[0] = 50;
        bonOculaire[1] = 50;
        int ecart(0);

        for (int i(0); i < oculaires.size(); i++) {
            ecart = qAbs(oculaires.at(i) - f);
            if (ecart < bonOculaire[1]) {
                bonOculaire[0] = oculaires.at(i);
                bonOculaire[1] = ecart;
            }
        }
        return QString::number(bonOculaire[0]) + " mm";
    }
    return "Aucun";
}
