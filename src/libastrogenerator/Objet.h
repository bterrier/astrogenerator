#ifndef OBJET_H
#define OBJET_H

#include <cmath>

#include <QObject>

class Objet : public QObject
{
    Q_OBJECT

public:
    Objet();
    QString nom();
    QString type();
    QString ascdr();
    double ascdrDouble();
    QString declinaison();
    double decDouble();
    QString constellation();
    double magnitude();
    QString interet(bool explicite = false);
    QString difficulte(bool explicite = false);
    double taille();
    int note();
    static double dmsToDegree(QString val);
    static double hmsToDegree(QString val);
    bool isValid() const;
    virtual QString nomComplet(bool abrege) const;
    QString nomComplet() const { return nomComplet(false); }
    QString ref() const;

protected:
    QString m_nom;
    QString m_ref;
    QString m_type;
    double m_ascdr;
    double m_dec;
    QString m_constellation;
    double m_magnitude;
    QString m_interet;
    QString m_difficulte;
    double m_taille;
    int m_note;
    bool m_valid;
};

#endif // OBJET_H
