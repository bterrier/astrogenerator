#include "ObjetCPObs.h"
#include "ObjetObs.h"
#include "ObjetCP.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QVariant>

ObjetCPObs::ObjetCPObs(ObjetCP *objet, QDateTime debut, QDateTime fin) : ObjetObs(debut,fin)
{
    initialiser(objet->ref());
}
ObjetCPObs::ObjetCPObs(QString ref, QDateTime debut, QDateTime fin) : ObjetObs(debut,fin)
{
    initialiser(ref);
}
void ObjetCPObs::initialiser(QString ref)
{
    QString sql("WHERE reference = '"+ref+"'");

    if(ref.left(1) == "M")
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
            m_ngc = requete.value(1).toString().right(requete.value(1).toString().count()-3).toInt();
        else
            m_ngc = 0;
        if(requete.value(7).toString() != "0")
            m_messier = requete.value(7).toString().right(requete.value(7).toString().count()-1).toInt(); // messier
        else
            m_messier = 0;

        m_ref = ref;
    }
    else
    {
        QMessageBox::critical(0,tr("Objet introuvable"),tr("L'objet demandé est introuvable : ", "Suivi de l'objet introuvable") + ref);
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
int ObjetCPObs::ngc() const
{
    return m_ngc;
}
int ObjetCPObs::messier() const
{
    return m_messier;
}
QString ObjetCPObs::nomComplet(bool abrege) const
{
    QString retour;

    if(!abrege)
    {
        if(m_nom != "")
        {
            retour = m_nom + " (";
            if(m_messier > 0)
                retour += "M"+QString::number(m_messier)+", "+m_ref+")";
            else
                retour += m_ref+")";
        }
        else if(m_messier > 0)
            retour = "M"+QString::number(m_messier)+" ("+m_ref+")";
        else
            retour = m_ref;
    }
    else
    {
        if(m_messier > 0)
            retour = "M" + QString::number(m_messier);
        else
            retour = m_ref;
    }

    return retour;
}
