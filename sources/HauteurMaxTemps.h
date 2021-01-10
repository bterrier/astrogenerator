#ifndef HAUTEURMAXTEMPS_H
#define HAUTEURMAXTEMPS_H

#include <QDateTime>

class HauteurMaxTemps
{
    public:
        HauteurMaxTemps();
        HauteurMaxTemps(QDateTime temps, double hauteurMax);
        QDateTime temps() const;
        double hauteurMax() const;
        void setDateTime(QDateTime date);
        void setHauteurMax(double hauteur);
    private:
        QDateTime m_temps;
        double m_hauteurMax;
};

#endif // HAUTEURMAXTEMPS_H
