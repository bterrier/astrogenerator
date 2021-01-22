#include "Objet.h"

Objet::Objet()
{
    m_valid = false;
}
bool Objet::isValid() const
{
    return m_valid;
}
QString Objet::constellation()
{
    return m_constellation;
}
QString Objet::nom()
{
    return m_nom;
}
QString Objet::type()
{
    return m_type;
}
QString Objet::ref() const
{
    return m_ref;
}
QString Objet::ascdr()
{
    double ascdrNew(m_ascdr);

    double h(0), m(0), s(0);

    h = (double) floor(ascdrNew/15);
    m = (double) floor((ascdrNew - h*15)/0.25);
    s = (double) round((ascdrNew - h*15 - m*0.25)/(0.25/60));

    if(h==24)
        h=0;

    return QString(QString::number(h) + "h " + QString::number(m) + "m " + QString::number(s) + "s");
}
double Objet::ascdrDouble()
{
    return m_ascdr;
}
QString Objet::declinaison()
{
    double d(0), m(0), s(0);
    QString signe("+");
    double decNew(m_dec);
    if(decNew < 0)
    {
        decNew *= -1.0; // On le met supérieur à 0
        signe = "-";
    }

    d = floor(decNew);
    m = floor((decNew-d)*60);
    s = floor(((decNew-d)*60-m)*60);

    return QString(signe + QString::number(d) + "° " + QString::number(m) + "' " + QString::number(s) + "\"");
}

double Objet::decDouble()
{
    return m_dec;
}
double Objet::magnitude()
{
    return m_magnitude;
}
QString Objet::interet(bool explicite)
{
    if(!explicite)
        return m_interet;
    else
    {
        switch(m_interet.toInt())
        {
            case 1 : return tr("Sans interêt"); break;
            case 2 : return tr("Peu interessant"); break;
            case 3 : return tr("Interessant"); break;
            case 4 : return tr("Remarquable"); break;
            default: return tr("Inconnu"); break;
        }
    }
}
QString Objet::difficulte(bool explicite)
{
    if(!explicite)
        return m_difficulte;
    else
    {
        switch(m_difficulte.toInt())
        {
            case 1 : return tr("Très facile"); break;
            case 2 : return tr("Facile"); break;
            case 3 : return tr("Moyen"); break;
            case 4 : return tr("Difficile"); break;
            case 0 : return tr("Inconnue"); break;
            default: return tr("Inconnue"); break;
        }
    }
}
double Objet::taille()
{
    return m_taille;
}
int Objet::note()
{
    return m_note;
}
double Objet::dmsToDegree(QString val)
{
    double degree;

    degree = val.midRef(1,2).toDouble();
    degree += val.midRef(5,2).toDouble() / 60;

    if(val.at(0) == '-')
        return degree*(-1.0);
    else
        return degree;
}
double Objet::hmsToDegree(QString val)
{
    double degree;

    QStringList list = val.split(' ');

    degree = list.value(0).leftRef(list.value(0).count()-1).toDouble()*15;
    degree += list.value(1).leftRef(list.value(1).count()-3).toDouble()*15/60;
    degree += list.value(2).leftRef(list.value(2).count()-1).toDouble()*15/3600;

    return degree;
}
QString Objet::nomComplet(bool abrege) const
{
    Q_UNUSED(abrege)

    return m_nom;
}
