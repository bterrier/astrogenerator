#ifndef SOIREE_H
#define SOIREE_H

#include <QtGui>
#include "ObjetObs.h"
#include "ObjetCPObs.h"
#include "ObjetCP.h"
#include <QtSql>

class Soiree : public QObject
{
    Q_OBJECT

public:
    Soiree();
    void genererSoiree(double lat, double longi, QDateTime debut, QDateTime fin, int duree_une, QString constellation, QString niveau, unsigned int diametre, unsigned int focale, QSettings *user, bool boolPlanete = true);
    static QVector<ObjetObs *> trierPlanning(QVector<ObjetObs *> planning);
    static int trouverPlusPetit(const QVector<ObjetObs *> &planning);
    static Soiree *soaToSoiree(const QString &fileName);
    static bool ajouterSoireeRecentes(const QString &fileName);

    bool soireeToSoa(QString const& fileName);

    // M�thodes de la classe
    void ajouterObjet(int index, ObjetCP *objetParam, int duree);
    void ajouterObjet(int index, QString refPlanete, int duree);
    void ajouterObjet(ObjetObs *objet);
    void supprimerObjet(int index);
    void modifierDuree(int index, int duree);
    void descendreObjet(int index);
    void monterObjet(int index);
    QStandardItemModel* toModele() const;
    QString retourDebut() const;
    QString retourFin() const;
    QString duree() const;
    int indexFromRef(QString ref) const;
    QVector<double> hauteurAzimutObjet(int index) const; // Renvoie hauteur et azimut d'un objet

    // Accesseurs
    void setDebut(QDateTime debut);
    void setFin(QDateTime fin);
    void setLat(double lat);
    void setLongi(double longi);
    void setNiveau(QString niveau);
    void setDiametre(unsigned int diametre);
    void setFocale(unsigned int focale);
    void setBoolPlanete(bool planete);
    void setPlanning(QVector<ObjetObs *> liste);
    void setPlanningOld(QVector<ObjetObs *> liste);
    void setFile(QString file);
    void setVille(QString ville);
    void setPays(QString pays);


    QString getPays() const;
    QString getVille() const;
    QDateTime getDebut() const;
    QDateTime getFin() const;
    int getDureeUne() const;
    double getLat() const;
    double getLongi() const;
    QString getNiveau() const;
    int getDiametre() const;
    int getFocale() const;
    bool getBoolPlanete() const;
    QVector<ObjetObs *> getPlanning() const;
    QVector<ObjetObs *> getPlanningOld() const;
    QString getFile() const;

    void toXML() const;
    void toPDF();
    bool paintPdf(QPrinter *printer);

    bool hasChanged(); // Si la soir�e a chang�
    bool shouldBeSaved(); // Si la soir�e a chang� ou qu'elle n'est pas enregistr�
    bool enregistrerSoiree();

signals:
    void generation(int);

private:
    QVector<ObjetObs *> m_listeObjets;
    QVector<ObjetObs *> m_listeObjetsOld;
    QString m_pays;
    QString m_ville;
    QDateTime m_debut;
    QDateTime m_fin;
    int m_duree_une;
    double m_lat;
    double m_longi;
    QString m_niveau;
    int m_diametre;
    int m_focale;
    bool m_boolPlanete;
    QString m_file;

};

#endif // SOIREE_H
