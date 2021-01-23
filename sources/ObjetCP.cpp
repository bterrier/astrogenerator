#include "ObjetCP.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QVariant>

ObjetCP::ObjetCP()
{
    m_valid = false;
}
ObjetCP::ObjetCP(QString ref) : Objet()
{
    QString sql("WHERE reference = '"+ref+"'");

    if(ref.at(0) == 'M')
        sql = "WHERE messier = '"+ref+"'";

    QSqlQuery count("SELECT COUNT(*) as nbr FROM ngcic "+sql);
    count.next();
    if(count.value(0).toInt() > 0)
    {
        QSqlQuery requete("SELECT nom, reference, type, ascdr, declinaison, constellation, magnitude, messier, interet, taille, difficulte FROM ngcic "+sql);
        requete.next();

        m_valid = true;
        m_nom = requete.value(0).toString(); // nom
        m_type = requete.value(2).toString(); // type
        m_ascdr = hmsToDegree(requete.value(3).toString()); // ascdr
        m_dec = dmsToDegree(requete.value(4).toString()); // dec
        m_constellation = requete.value(5).toString(); // constellation
        m_magnitude = requete.value(6).toDouble(); // magnitude
        m_interet = requete.value(8).toString(); // interet
        m_difficulte = requete.value(10).toString(); // difficulte
        m_taille = requete.value(9).toDouble(); // taille

        if(requete.value(1).toString().left(3) == "NGC")
            m_ngc = requete.value(1).toString().rightRef(requete.value(1).toString().count()-3).toInt();
        else
            m_ngc = 0;
        if(requete.value(7).toString() != "0")
            m_messier = requete.value(7).toString().rightRef(requete.value(7).toString().count()-1).toInt(); // messier
        else
            m_messier = 0;

        m_refBdd = ref;
    }
    else
    {
        QMessageBox::critical(nullptr,tr("Objet introuvable"),tr("L'objet demandé est introuvable : ","Suivi de la référence") + ref);
        m_valid = false;
        m_nom = "";
        m_type = "";
        m_ascdr = 0;
        m_dec = 0;
        m_constellation = "";
        m_magnitude = 0;
        m_interet = "";
        m_difficulte = "";
        m_taille = 0;
    }
}
int ObjetCP::ngc() const
{
    return m_ngc;
}
QString ObjetCP::ref() const
{
    return m_refBdd;
}
int ObjetCP::messier() const
{
    return m_messier;
}
QString ObjetCP::nomComplet() const
{
    QString retour;

    if(m_nom != "")
    {
        retour = m_nom + " (";
        if(m_messier > 0)
            retour += "M"+QString::number(m_messier)+", "+m_refBdd+")";
        else
            retour += m_refBdd+")";
    }
    else if(m_messier > 0)
        retour = "M"+QString::number(m_messier)+" ("+m_refBdd+")";
    else
        retour = m_refBdd;

    return retour;
}
