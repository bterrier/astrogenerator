#include "ObjetPlaneteObs.h"
#include "ObjetObs.h"
#include "Calculastro.h"

ObjetPlaneteObs::ObjetPlaneteObs(QString nomPlanete, QDateTime debut, QDateTime fin) : ObjetObs(debut,fin)
{
    m_nom = Calculastro::referencePlaneteToNom(nomPlanete,true);
    QVector<double> infosPlanete;
    infosPlanete = Calculastro::coordonneesPlanetes(debut.date(),debut.time(),Calculastro::referencePlaneteToNom(nomPlanete));
    m_ref = nomPlanete;
    m_type = "Planète";
    m_ascdr = infosPlanete.at(0);
    m_dec = infosPlanete.at(1);
    m_constellation = "-";
    m_magnitude = Calculastro::round(infosPlanete.at(2),1);
    m_interet = "4";
    m_difficulte = "1";
    m_taille = 0;
    m_note = 0;
    m_illumination = infosPlanete.at(3);
    m_distance = infosPlanete.at(4);
    m_valid = true;
}
QString ObjetPlaneteObs::nomComplet(bool abrege) const
{
    return m_nom;
}
double ObjetPlaneteObs::distance() const
{
    return m_distance;
}
double ObjetPlaneteObs::illumination() const
{
    return m_illumination;
}
