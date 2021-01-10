#include "ActionsFenetre.h"

ActionsFenetre::ActionsFenetre(QWidget *parent)
{
    actionInfoSoiree = new QAction(tr("Infos sur la soirée"),parent);
        actionInfoSoiree->setIcon(QIcon("images/info.png"));
    actionCreerSoiree = new QAction(tr("Générer une soirée"), parent);
        actionCreerSoiree->setShortcut(QKeySequence(tr("Ctrl+N")));
        actionCreerSoiree->setIcon(QIcon("images/generate.png"));
    actionFabriquerSoiree = new QAction(tr("Créer une soirée"), parent);
        actionFabriquerSoiree->setIcon(QIcon("images/creer-soiree.png"));
    actionOuvrirSoiree = new QAction(tr("Ouvrir une soirée"), parent);
        actionOuvrirSoiree->setShortcut(QKeySequence(tr("Ctrl+O")));
        actionOuvrirSoiree->setIcon(QIcon("images/open.png"));
    actionEnregistrer = new QAction(tr("Enregistrer"), parent);
        actionEnregistrer->setShortcut(QKeySequence(tr("Ctrl+S")));
        actionEnregistrer->setIcon(QIcon("images/save.png"));
    actionSoireesRecentes = new QAction(tr("Soirées récentes"), parent);
    actionImprimer = new QAction(tr("Imprimer"), parent);
        actionImprimer->setShortcut(QKeySequence(tr("Ctrl+P")));
        actionImprimer->setIcon(QIcon("images/print.png"));
    actionQuitter = new QAction(tr("Quitter"), parent);
        actionQuitter->setShortcut(QKeySequence(tr("Alt+F4")));
    actionFermer = new QAction(tr("Fermer la soirée"), parent);
        actionFermer->setIcon(QIcon("images/close.png"));
    actionToutFermer = new QAction(tr("Tout fermer"), parent);

    actionDiaporama = new QAction(tr("Diaporama"), parent);
        actionDiaporama->setText(tr("Présenter la soirée sous forme de diaporama"));
        actionDiaporama->setShortcut(QKeySequence(tr("F5")));
        actionDiaporama->setIcon(QIcon("images/slideshow.png"));
    actionBarreStatut = new QAction(tr("Barre de statut"), parent);
        actionBarreStatut->setCheckable(true);
    actionCarteCiel = new QAction(tr("Carte du ciel"), parent);
        actionCarteCiel->setIcon(QIcon("images/carteciel.png"));
            carteCouleurFond = new QAction(tr("Couleur du fond"),parent);
            carteCouleurEtoiles = new QAction(tr("Couleur des étoiles"),parent);
            carteCouleurLegende = new QAction(tr("Couleur des légendes"),parent);
            carteCouleurConstellation = new QAction(tr("Couleur des constellations"),parent);
            carteCouleurObjet = new QAction(tr("Couleur des objets"),parent);

    actionMonterObjet = new QAction(tr("Monter l'objet"), parent);
        actionMonterObjet->setIcon(QIcon("images/arrow-up.png"));
    actionDescendreObjet = new QAction(tr("Descendre l'objet"), parent);
        actionDescendreObjet->setIcon(QIcon("images/arrow-down.png"));
    actionSupprimerObjet = new QAction(tr("Supprimer l'objet"), parent);
        actionSupprimerObjet->setIcon(QIcon("images/list-remove2.png"));
    actionChangerDureeObjet = new QAction(tr("Changer la durée de l'objet"), parent);
        actionChangerDureeObjet->setIcon(QIcon("images/chronometer.png"));
    actionExporterXML = new QAction(tr("XML"), parent);
        actionExporterXML->setIcon(QIcon("images/xml.png"));
    actionExporterPDF = new QAction(tr("PDF"), parent);
        actionExporterPDF->setIcon(QIcon("images/pdf.png"));
    actionExporterJSON = new QAction(tr("JSON"), parent);
    actionPartager = new QAction(tr("Partager..."), parent);
    actionAjouterObjet = new QAction(tr("Ajouter un objet"), parent);
        actionAjouterObjet->setIcon(QIcon("images/add.png"));
    actionAjouterPlanete = new QAction(tr("Ajouter une planète"),parent);
        actionAjouterPlanete->setIcon(QIcon("images/planete.png"));

    actionPreferencesTelescope = new QAction(tr("Télescope"), parent);
    actionPreferencesOculaires = new QAction(tr("Oculaires"), parent);
    actionPreferencesLocalisation = new QAction(tr("Localisation"),parent);
    actionPreferencesCarteCiel = new QAction(tr("Carte du ciel"),parent);

    actionConnexion = new QAction(tr("Se connecter"), parent);
        actionConnexion->setIcon(QIcon("images/sign-in.png"));
    actionCalendrier = new QAction(tr("Calendrier"), parent);
    actionRechercheAstronomes = new QAction(tr("Rechercher des astronomes..."), parent);
    actionPersonaliserGenerateur = new QAction(tr("Personaliser le générateur..."), parent);

    actionBDD = new QAction(tr("Accéder à la base de données"), parent);
    actionCDS = new QAction(tr("CDS - Données astronomiques"), parent);
    actionObjetsRemarquables = new QAction(tr("Objets remarquables"), parent);

    actionAPropos = new QAction(tr("A propos d'astroGenerator"), parent);
    actionAProposGenerateur = new QAction(tr("A propos du générateur"), parent);
    actionUpdate = new QAction(tr("Mise à jour"), parent);
    actionAide = new QAction(tr("Aide"), parent);
    actionSiteUniversAstronomie = new QAction(tr("Site de l'éditeur"), parent);
}

QAction* ActionsFenetre::getActionInfoSoiree() { return actionInfoSoiree; }
QAction* ActionsFenetre::getActionCreerSoiree() { return actionCreerSoiree; }
QAction* ActionsFenetre::getActionFabriquerSoiree() { return actionFabriquerSoiree; }
QAction* ActionsFenetre::getActionOuvrirSoiree() { return actionOuvrirSoiree; }
QAction* ActionsFenetre::getActionEnregistrer() { return actionEnregistrer; }
QAction* ActionsFenetre::getActionSoireesRecentes() { return actionSoireesRecentes; }
QAction* ActionsFenetre::getActionImprimer() { return actionImprimer; }
QAction* ActionsFenetre::getActionQuitter() { return actionQuitter; }
QAction* ActionsFenetre::getActionFermer() { return actionFermer; }
QAction* ActionsFenetre::getActionToutFermer() { return actionToutFermer; }
QAction* ActionsFenetre::getActionDiaporama() { return actionDiaporama; }
QAction* ActionsFenetre::getActionBarreStatut() { return actionBarreStatut; }
QAction* ActionsFenetre::getActionCarteCiel() { return actionCarteCiel; }
    QAction* ActionsFenetre::getCarteCouleurFond() { return carteCouleurFond; }
    QAction* ActionsFenetre::getCarteCouleurEtoiles() { return carteCouleurEtoiles; }
    QAction* ActionsFenetre::getCarteCouleurLegende() { return carteCouleurLegende; }
    QAction* ActionsFenetre::getCarteCouleurConstellation() { return carteCouleurConstellation; }
    QAction* ActionsFenetre::getCarteCouleurObjet() { return carteCouleurObjet; }
QAction* ActionsFenetre::getActionMonterObjet() { return actionMonterObjet; }
QAction* ActionsFenetre::getActionDescendreObjet() { return actionDescendreObjet; }
QAction* ActionsFenetre::getActionSupprimerObjet() { return actionSupprimerObjet; }
QAction* ActionsFenetre::getActionChangerDureeObjet() { return actionChangerDureeObjet; }
QAction* ActionsFenetre::getActionExporterXML() { return actionExporterXML; }
QAction* ActionsFenetre::getActionExporterPDF() { return actionExporterPDF; }
QAction* ActionsFenetre::getActionExporterJSON() { return actionExporterJSON; }
QAction* ActionsFenetre::getActionPartager() { return actionPartager; }
QAction* ActionsFenetre::getActionAjouterObjet() { return actionAjouterObjet; }
QAction* ActionsFenetre::getActionAjouterPlanete() { return actionAjouterPlanete; }
QAction* ActionsFenetre::getActionPreferencesTelescope() { return actionPreferencesTelescope; }
QAction* ActionsFenetre::getActionPreferencesOculaires() { return actionPreferencesOculaires; }
QAction* ActionsFenetre::getActionPreferencesCarteCiel() { return actionPreferencesCarteCiel; }
QAction* ActionsFenetre::getActionPreferencesLocalisation() { return actionPreferencesLocalisation; }
QAction* ActionsFenetre::getActionConnexion() { return actionConnexion; }
QAction* ActionsFenetre::getActionCalendrier() { return actionCalendrier; }
QAction* ActionsFenetre::getActionRechercheAstronomes() { return actionRechercheAstronomes; }
QAction* ActionsFenetre::getActionPersonaliserGenerateur() { return actionPersonaliserGenerateur; }
QAction* ActionsFenetre::getActionBDD() { return actionBDD; }
QAction* ActionsFenetre::getActionCDS() { return actionCDS; }
QAction* ActionsFenetre::getActionObjetsRemarquables() { return actionObjetsRemarquables; }
QAction* ActionsFenetre::getActionAPropos() { return actionAPropos; }
QAction* ActionsFenetre::getActionAProposGenerateur() { return actionAProposGenerateur; }
QAction* ActionsFenetre::getActionUpdate() { return actionUpdate; }
QAction* ActionsFenetre::getActionAide() { return actionAide; }
QAction* ActionsFenetre::getActionSiteUniversAstronomie() { return actionSiteUniversAstronomie; }

void ActionsFenetre::griserActionInfoSoiree(bool ok) { actionInfoSoiree->setDisabled(ok); }
void ActionsFenetre::griserActionCreerSoiree(bool ok) { actionCreerSoiree->setDisabled(ok); }
void ActionsFenetre::griserActionFabriquerSoiree(bool ok) { actionFabriquerSoiree->setDisabled(ok); }
void ActionsFenetre::griserActionOuvrirSoiree(bool ok) { actionOuvrirSoiree->setDisabled(ok); }
void ActionsFenetre::griserActionEnregistrer(bool ok) { actionEnregistrer->setDisabled(ok); }
void ActionsFenetre::griserActionSoireesRecentes(bool ok) { actionSoireesRecentes->setDisabled(ok); }
void ActionsFenetre::griserActionImprimer(bool ok) { actionImprimer->setDisabled(ok); }
void ActionsFenetre::griserActionQuitter(bool ok) { actionQuitter->setDisabled(ok); }
void ActionsFenetre::griserActionFermer(bool ok) { actionFermer->setDisabled(ok); }
void ActionsFenetre::griserActionToutFermer(bool ok) { actionToutFermer->setDisabled(ok); }
void ActionsFenetre::griserActionDiaporama(bool ok) { actionDiaporama->setDisabled(ok); }
void ActionsFenetre::griserActionBarreStatut(bool ok) { actionBarreStatut->setDisabled(ok); }
void ActionsFenetre::griserActionCarteCiel(bool ok) { actionCarteCiel->setDisabled(ok); }
    void ActionsFenetre::griserCarteCouleurFond(bool ok) { carteCouleurFond->setDisabled(ok); }
    void ActionsFenetre::griserCarteCouleurEtoiles(bool ok) { carteCouleurEtoiles->setDisabled(ok); }
    void ActionsFenetre::griserCarteCouleurLegende(bool ok) { carteCouleurLegende->setDisabled(ok); }
    void ActionsFenetre::griserCarteCouleurConstellation(bool ok) { carteCouleurConstellation->setDisabled(ok); }
    void ActionsFenetre::griserCarteCouleurObjet(bool ok) { carteCouleurObjet->setDisabled(ok); }
void ActionsFenetre::griserActionMonterObjet(bool ok) { actionMonterObjet->setDisabled(ok); }
void ActionsFenetre::griserActionDescendreObjet(bool ok) { actionDescendreObjet->setDisabled(ok); }
void ActionsFenetre::griserActionSupprimerObjet(bool ok) { actionSupprimerObjet->setDisabled(ok); }
void ActionsFenetre::griserActionChangerDureeObjet(bool ok) { actionChangerDureeObjet->setDisabled(ok); }
void ActionsFenetre::griserActionExporterXML(bool ok) { actionExporterXML->setDisabled(ok); }
void ActionsFenetre::griserActionExporterPDF(bool ok) { actionExporterPDF->setDisabled(ok); }
void ActionsFenetre::griserActionExporterJSON(bool ok) { actionExporterJSON->setDisabled(ok); }
void ActionsFenetre::griserActionPartager(bool ok) { actionPartager->setDisabled(ok); }
void ActionsFenetre::griserActionAjouterObjet(bool ok) { actionAjouterObjet->setDisabled(ok); }
void ActionsFenetre::griserActionAjouterPlanete(bool ok) { actionAjouterPlanete->setDisabled(ok); }
void ActionsFenetre::griserActionPreferencesTelescope(bool ok) { actionPreferencesTelescope->setDisabled(ok); }
void ActionsFenetre::griserActionPreferencesOculaires(bool ok) { actionPreferencesOculaires->setDisabled(ok); }
void ActionsFenetre::griserActionPreferencesCarteCiel(bool ok) { actionPreferencesCarteCiel->setDisabled(ok); }
void ActionsFenetre::griserActionPreferencesLocalisation(bool ok) { actionPreferencesLocalisation->setDisabled(ok); }
void ActionsFenetre::griserActionConnexion(bool ok) { actionConnexion->setDisabled(ok); }
void ActionsFenetre::griserActionCalendrier(bool ok) { actionCalendrier->setDisabled(ok); }
void ActionsFenetre::griserActionRechercheAstronomes(bool ok) { actionRechercheAstronomes->setDisabled(ok); }
void ActionsFenetre::griserActionPersonaliserGenerateur(bool ok) { actionPersonaliserGenerateur->setDisabled(ok); }
void ActionsFenetre::griserActionBDD(bool ok) { actionBDD->setDisabled(ok); }
void ActionsFenetre::griserActionCDS(bool ok) { actionCDS->setDisabled(ok); }
void ActionsFenetre::griserActionObjetsRemarquables(bool ok) { actionObjetsRemarquables->setDisabled(ok); }
void ActionsFenetre::griserActionAPropos(bool ok) { actionAPropos->setDisabled(ok); }
void ActionsFenetre::griserActionAProposGenerateur(bool ok) { actionAProposGenerateur->setDisabled(ok); }
void ActionsFenetre::griserActionUpdate(bool ok) { actionUpdate->setDisabled(ok); }
void ActionsFenetre::griserActionAide(bool ok) { actionAide->setDisabled(ok); }
void ActionsFenetre::griserActionSiteUniversAstronomie(bool ok) { actionSiteUniversAstronomie->setDisabled(ok); }
