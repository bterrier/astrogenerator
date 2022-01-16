#ifndef ACTIONSFENETRE_H
#define ACTIONSFENETRE_H

#include <QAction>
#include <QObject>

class ActionsFenetre : public QObject
{
	Q_OBJECT

public:
	explicit ActionsFenetre(QWidget *parent = nullptr);

	QAction *getActionInfoSoiree();
	QAction *getActionCreerSoiree();
	QAction *getActionFabriquerSoiree();
	QAction *getActionOuvrirSoiree();
	QAction *getActionEnregistrer();
	QAction *getActionSoireesRecentes();
	QAction *getActionImprimer();
	QAction *getActionQuitter();
	QAction *getActionFermer();
	QAction *getActionToutFermer();
	QAction *getActionDiaporama();
	QAction *getActionBarreStatut();
	QAction *getActionCarteCiel();
	QAction *getCarteCouleurFond();
	QAction *getCarteCouleurEtoiles();
	QAction *getCarteCouleurLegende();
	QAction *getCarteCouleurConstellation();
	QAction *getCarteCouleurObjet();
	QAction *getActionMonterObjet();
	QAction *getActionDescendreObjet();
	QAction *getActionSupprimerObjet();
	QAction *getActionChangerDureeObjet();
	QAction *getActionExporterXML();
	QAction *getActionExporterPDF();
	QAction *getActionExporterJSON();
	QAction *getActionPartager();
	QAction *getActionAjouterObjet();
	QAction *getActionAjouterPlanete();
	QAction *getActionPreferencesTelescope();
	QAction *getActionPreferencesOculaires();
	QAction *getActionPreferencesLocalisation();
	QAction *getActionPreferencesCarteCiel();
	QAction *getActionConnexion();
	QAction *getActionCalendrier();
	QAction *getActionRechercheAstronomes();
	QAction *getActionPersonaliserGenerateur();
	QAction *getActionBDD();
	QAction *getActionCDS();
	QAction *getActionObjetsRemarquables();
	QAction *getActionAPropos();
	QAction *getActionAide();

	void griserActionInfoSoiree(bool ok);
	void griserActionCreerSoiree(bool ok);
	void griserActionFabriquerSoiree(bool ok);
	void griserActionOuvrirSoiree(bool ok);
	void griserActionEnregistrer(bool ok);
	void griserActionSoireesRecentes(bool ok);
	void griserActionImprimer(bool ok);
	void griserActionQuitter(bool ok);
	void griserActionFermer(bool ok);
	void griserActionToutFermer(bool ok);
	void griserActionDiaporama(bool ok);
	void griserActionBarreStatut(bool ok);
	void griserActionCarteCiel(bool ok);
	void griserCarteCouleurFond(bool ok);
	void griserCarteCouleurEtoiles(bool ok);
	void griserCarteCouleurLegende(bool ok);
	void griserCarteCouleurConstellation(bool ok);
	void griserCarteCouleurObjet(bool ok);
	void griserActionMonterObjet(bool ok);
	void griserActionDescendreObjet(bool ok);
	void griserActionSupprimerObjet(bool ok);
	void griserActionChangerDureeObjet(bool ok);
	void griserActionExporterXML(bool ok);
	void griserActionExporterPDF(bool ok);
	void griserActionExporterJSON(bool ok);
	void griserActionPartager(bool ok);
	void griserActionAjouterObjet(bool ok);
	void griserActionAjouterPlanete(bool ok);
	void griserActionPreferencesTelescope(bool ok);
	void griserActionPreferencesOculaires(bool ok);
	void griserActionPreferencesCarteCiel(bool ok);
	void griserActionPreferencesLocalisation(bool ok);
	void griserActionConnexion(bool ok);
	void griserActionCalendrier(bool ok);
	void griserActionRechercheAstronomes(bool ok);
	void griserActionPersonaliserGenerateur(bool ok);
	void griserActionBDD(bool ok);
	void griserActionCDS(bool ok);
	void griserActionObjetsRemarquables(bool ok);
	void griserActionAPropos(bool ok);
	void griserActionAide(bool ok);

private:
	QAction *actionInfoSoiree;
	QAction *actionCreerSoiree;
	QAction *actionFabriquerSoiree;
	QAction *actionOuvrirSoiree;
	QAction *actionEnregistrer;
	QAction *actionSoireesRecentes;
	QAction *actionImprimer;
	QAction *actionQuitter;
	QAction *actionFermer;
	QAction *actionToutFermer;
	QAction *actionDiaporama;
	QAction *actionBarreStatut;
	QAction *actionCarteCiel;
	QAction *carteCouleurFond;
	QAction *carteCouleurEtoiles;
	QAction *carteCouleurLegende;
	QAction *carteCouleurConstellation;
	QAction *carteCouleurObjet;
	QAction *actionMonterObjet;
	QAction *actionDescendreObjet;
	QAction *actionSupprimerObjet;
	QAction *actionChangerDureeObjet;
	QAction *actionExporterXML;
	QAction *actionExporterPDF;
	QAction *actionExporterJSON;
	QAction *actionPartager;
	QAction *actionAjouterObjet;
	QAction *actionAjouterPlanete;
	QAction *actionPreferencesLocalisation;
	QAction *actionPreferencesTelescope;
	QAction *actionPreferencesOculaires;
	QAction *actionPersonaliserGenerateur;
	QAction *actionPreferencesCarteCiel;
	QAction *actionConnexion;
	QAction *actionCalendrier;
	QAction *actionRechercheAstronomes;
	QAction *actionBDD;
	QAction *actionCDS;
	QAction *actionObjetsRemarquables;
	QAction *actionAPropos;
	QAction *actionAide;
};

#endif // ACTIONSFENETRE_H
