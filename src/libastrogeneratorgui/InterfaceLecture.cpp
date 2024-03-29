#include "InterfaceLecture.h"

#include <QStandardPaths>
#include <QStringBuilder>

#include "ActionsFenetre.h"
#include "Calculastro.h"
#include "Carteciel.h"
#include "Constantes.h"
#include "Diaporama.h"
#include "Soiree.h"

#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QLabel>
#include <QMessageBox>
#include <QPageLayout>
#include <QPrintDialog>
#include <QPrinter>
#include <QToolBar>

#include "nightprinter.h"
#include "skymapdialog.h"

#include <QDebug>

InterfaceLecture::InterfaceLecture(Soiree *soiree, ActionsFenetre *listeActionsParam, QWidget *parent) :
    Interface(listeActionsParam, parent),
    m_model(new NightModel(this))
{
	m_soiree = soiree;
	m_active = true; // On considère qu'à sa construction l'interface est active
	m_model->setNight(soiree);
	m_carte = new Carteciel(soiree);
	m_vue = new QTableView;
	m_vue->setModel(m_model);
	m_vue->resizeColumnsToContents(); // Adapte la largeur des colonnes en fonction de leur contenu
	m_vue->setCornerButtonEnabled(false);
	m_vue->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_vue->setSelectionMode(QAbstractItemView::SingleSelection);
	m_vue->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_vue->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	QHBoxLayout *layoutPrincipal = new QHBoxLayout;

	QToolBar *outilsOnglets = new QToolBar;
	outilsOnglets->addAction(m_listeActions->getActionAjouterObjet());
	outilsOnglets->addAction(m_listeActions->getActionSupprimerObjet());
	outilsOnglets->addAction(m_listeActions->getActionMonterObjet());
	outilsOnglets->addAction(m_listeActions->getActionDescendreObjet());
	outilsOnglets->addAction(m_listeActions->getActionChangerDureeObjet());
	outilsOnglets->addAction(m_listeActions->getActionAjouterPlanete());
	outilsOnglets->addAction(m_listeActions->getActionCarteCiel());
	outilsOnglets->addAction(m_listeActions->getActionDiaporama());
	outilsOnglets->addAction(m_listeActions->getActionExporterPDF());
	outilsOnglets->addAction(m_listeActions->getActionInfoSoiree());
	outilsOnglets->setOrientation(Qt::Vertical);
	layoutPrincipal->addWidget(outilsOnglets);

	QVBoxLayout *layoutDroiteContenu = new QVBoxLayout;
	layoutDroiteContenu->addWidget(m_vue);

	layoutPrincipal->addLayout(layoutDroiteContenu);

	setLayout(layoutPrincipal);

	// On fait toutes les connexions
	connect(m_vue, &QTableView::doubleClicked, this, &InterfaceLecture::afficherInfosObjet);
	connect(m_vue, &QTableView::clicked, this, &InterfaceLecture::griserActions);
	connect(m_listeActions->getActionMonterObjet(), &QAction::triggered, this, &InterfaceLecture::monterObjet);
	connect(m_listeActions->getActionDescendreObjet(), &QAction::triggered, this, &InterfaceLecture::descendreObjet);
	connect(m_listeActions->getActionSupprimerObjet(), &QAction::triggered, this, &InterfaceLecture::supprimerObjet);
	connect(m_listeActions->getActionChangerDureeObjet(), &QAction::triggered, this, &InterfaceLecture::modifierObjet);
	connect(m_listeActions->getActionAjouterObjet(), &QAction::triggered, this, &InterfaceLecture::ajouterObjet);
	connect(m_listeActions->getActionAjouterPlanete(), &QAction::triggered, this, &InterfaceLecture::ajouterPlanete);
	connect(m_listeActions->getCarteCouleurConstellation(), &QAction::triggered, this, &InterfaceLecture::setCouleurConstellations);
	connect(m_listeActions->getCarteCouleurEtoiles(), &QAction::triggered, this, &InterfaceLecture::setCouleurEtoiles);
	connect(m_listeActions->getCarteCouleurFond(), &QAction::triggered, this, &InterfaceLecture::setCouleurFond);
	connect(m_listeActions->getCarteCouleurLegende(), &QAction::triggered, this, &InterfaceLecture::setCouleurLegende);
	connect(m_listeActions->getCarteCouleurObjet(), &QAction::triggered, this, &InterfaceLecture::setCouleurObjet);
	//	connect(m_listeActions->getActionEnregistrer(), &QAction::triggered, this, &InterfaceLecture::enregistrerSoiree);
	connect(m_listeActions->getActionCarteCiel(), &QAction::triggered, this, &InterfaceLecture::carteCiel);
	connect(m_listeActions->getActionDiaporama(), &QAction::triggered, this, &InterfaceLecture::diaporama);
	connect(m_listeActions->getActionExporterXML(), &QAction::triggered, this, &InterfaceLecture::toXML);
	connect(m_listeActions->getActionExporterPDF(), &QAction::triggered, this, &InterfaceLecture::toPDF);
	connect(m_listeActions->getActionInfoSoiree(), &QAction::triggered, this, &InterfaceLecture::infosSoiree);
	connect(m_listeActions->getActionImprimer(), &QAction::triggered, this, &InterfaceLecture::imprimer);
}
void InterfaceLecture::afficherInfosObjet(QModelIndex cells)
{
	if (m_active) {
		QString ref = m_model->index(cells.row(), 1).data().toString();
		int index = m_soiree->indexFromRef(ref);

		ObjetObs *objet = m_soiree->getPlanning().at(index);

		QDialog fenetreInfos;
		fenetreInfos.setWindowTitle(objet->nomComplet());
		QHBoxLayout *layout = new QHBoxLayout;

		QLabel *image = new QLabel;

		if (QFile::exists("icones/" + objet->ref() + ".jpg"))
			image->setPixmap(QPixmap("icones/" + objet->ref() + ".jpg"));
		else
			image->setPixmap(QPixmap("icones/default.png"));

		// QMessageBox::information(this,"Lien absolu",QDir::currentPath());

		layout->addWidget(image);
		QVBoxLayout *layoutV = new QVBoxLayout;
		QLabel *l_ascdr = new QLabel(tr("<strong>Ascension droite</strong> : ") + objet->ascdr());
		QLabel *l_dec = new QLabel(tr("<strong>Déclinaison</strong> : ") + objet->declinaison());
		QLabel *l_type = new QLabel(tr("<strong>Type</strong> : ") + objet->type());
		QLabel *l_mag = new QLabel(tr("<strong>Magnitude</strong> : ") + QString::number(objet->magnitude()));
		QLabel *l_cons = new QLabel(tr("<strong>Constellation</strong> : ") + Calculastro::abreviationToNom(objet->constellation()));
		QLabel *l_taille = new QLabel(tr("<strong>Taille</strong> : ") + QString::number(objet->taille()) + "'");
		QLabel *l_hauteur = new QLabel(tr("<strong>Hauteur</strong> : ") + Calculastro::degreeToDms(m_soiree->hauteurAzimutObjet(index).at(0)));
		QLabel *l_azimut = new QLabel(tr("<strong>Azimut</strong> : ") + Calculastro::degreeToDms(m_soiree->hauteurAzimutObjet(index).at(1)));

		layoutV->addWidget(l_ascdr);
		layoutV->addWidget(l_dec);
		layoutV->addWidget(l_type);
		layoutV->addWidget(l_mag);
		layoutV->addWidget(l_cons);
		layoutV->addWidget(l_taille);
		layoutV->addWidget(l_hauteur);
		layoutV->addWidget(l_azimut);
		layout->addLayout(layoutV);

		fenetreInfos.setLayout(layout);
		fenetreInfos.exec();
	}
}
void InterfaceLecture::monterObjet()
{
	if (m_active) {
		if (m_vue->currentIndex().isValid()) {
			int reponse = QMessageBox::question(nullptr, tr("Confirmer"), tr("Voulez-vous vraiment monter l'objet ?"), QMessageBox::Yes | QMessageBox::No);

			if (reponse == QMessageBox::Yes) {
				int ligne = m_vue->currentIndex().row();
				QString ref = m_model->index(ligne, 1).data().toString();

				int index = m_soiree->indexFromRef(ref);

				if (index >= 0) {
					m_soiree->monterObjet(index);
					m_model->reload();
					griserActions();
					emit afficher(tr("L'objet a été monté"));
				}
			}
		}
	}
}
void InterfaceLecture::descendreObjet()
{
	if (m_active) {
		if (m_vue->currentIndex().isValid()) {
			int reponse = QMessageBox::question(nullptr, tr("Confirmer"), tr("Voulez-vous vraiment descendre l'objet ?"), QMessageBox ::Yes | QMessageBox::No);

			if (reponse == QMessageBox::Yes) {
				int ligne = m_vue->currentIndex().row();
				QString ref = m_model->index(ligne, 1).data().toString();

				int index = m_soiree->indexFromRef(ref);
				if (index > -1) {
					m_soiree->descendreObjet(index);
					m_model->reload();
					griserActions();
					emit afficher(tr("L'objet a été descendu"));
				}
			}
		}
	}
}
void InterfaceLecture::supprimerObjet()
{
	if (m_active) {
		if (m_vue->currentIndex().isValid()) {
			int reponse = QMessageBox::question(nullptr, tr("Confirmer"), tr("Voulez-vous vraiment supprimer l'objet ?"), QMessageBox ::Yes | QMessageBox::No);
			if (reponse == QMessageBox::Yes) {
				QString ref = m_model->index(m_vue->currentIndex().row(), 1).data().toString();

				int index = m_soiree->indexFromRef(ref);
				if (index > -1) {
					m_soiree->supprimerObjet(index);
					m_model->reload();
					griserActions();
					emit afficher(tr("L'objet a été supprimé"));
				}
			}
		}
	}
}
void InterfaceLecture::modifierObjet()
{
	if (m_active) {
		if (m_vue->currentIndex().isValid()) {
			bool *ok = new bool;
			int reponse = QInputDialog::getInt(nullptr, tr("Modifier la durée"), tr("Quelle est la nouvelle durée ? (min)"), 5, DUREE_OBJET_MIN, DUREE_OBJET_MAX, 1, ok);
			if (*ok == true) {
				QString ref = m_model->index(m_vue->currentIndex().row(), 1).data().toString();

				int index = m_soiree->indexFromRef(ref);
				if (index > -1) {
					m_soiree->modifierDuree(index, reponse);
					m_model->reload();
					griserActions();
					emit afficher(tr("L'objet a été modifié"));
				}
			}
		}
	}
}
void InterfaceLecture::ajouterPlanete()
{
	if (m_active) {
		if (m_vue->currentIndex().isValid()) {
			bool ok = true;
			QStringList planetes;
			planetes << tr("Mercure") << tr("Vénus") << tr("Mars") << tr("Jupiter") << tr("Saturne") << tr("Uranus") << tr("Neptune");
			QString reponse = QInputDialog::getItem(nullptr, tr("Ajouter une planète"), tr("Choisisssez la planète à ajouter"), planetes, 0, false, &ok);
			if (ok) {
				int index = planetes.indexOf(reponse);
				if (index == -1)
					qDebug() << "[ERREUR INTERNE] ajouterPlanete() dans InterfaceLecture";
				else {
					QString ref = m_model->index(m_vue->currentIndex().row(), 1).data().toString();
					int indexPlace = m_soiree->indexFromRef(ref);

					m_soiree->ajouterObjet(indexPlace, "P" + QString::number(index + 1), TEMPS_OBS_DEFAUT);

					m_model->reload();
					griserActions();
					emit afficher(tr("La planète a été ajoutée"));
				}
			}
		}
	}
}

void InterfaceLecture::ajouterObjet()
{
	if (m_active) {
		if (m_vue->currentIndex().isValid()) {
			bool ok = true;
			QString reponse = QInputDialog::getText(nullptr, tr("Ajouter un objet"), tr("Quel objet ajouter ? (NGC****, M** ou P*)"), QLineEdit::Normal, "", &ok);
			if (ok == true) {
				if (reponse.at(0) == 'P') {
					QString ref = m_model->index(m_vue->currentIndex().row(), 1).data().toString();

					int index = m_soiree->indexFromRef(ref);
					if (index > -1) {
						m_soiree->ajouterObjet(index, reponse, TEMPS_OBS_DEFAUT);
						m_model->reload();
						griserActions();
						emit afficher(tr("La planète a été ajoutée"));
					}
				} else {
					ObjetCP *objet = new ObjetCP(reponse);
					if (objet->isValid()) {
						QString ref = m_model->index(m_vue->currentIndex().row(), 1).data().toString();

						int index = m_soiree->indexFromRef(ref);
						if (index > -1) {
							m_soiree->ajouterObjet(index, objet, TEMPS_OBS_DEFAUT);
							m_model->reload();
							griserActions();
							emit afficher(tr("L'objet a été ajouté"));
						}
					}
				}
			}
		}
	}
}
void InterfaceLecture::setCouleurConstellations()
{
	if (m_active)
		m_carte->setCouleurConstellation();
}
void InterfaceLecture::setCouleurEtoiles()
{
	if (m_active)
		m_carte->setCouleurEtoile();
}
void InterfaceLecture::setCouleurFond()
{
	if (m_active)
		m_carte->setCouleurFond();
}
void InterfaceLecture::setCouleurLegende()
{
	if (m_active)
		m_carte->setCouleurLegende();
}
void InterfaceLecture::setCouleurObjet()
{
	if (m_active)
		m_carte->setCouleurObjet();
}
// void InterfaceLecture::enregistrerSoiree()
//{
//	if (m_active) {
//		emit afficher(tr("Enregistrement de la soirée en cours..."));
//		if (m_soiree->shouldBeSaved()) // Si on doit réellement l'enregistrer
//		{
//			m_soiree->enregistrerSoiree(); // On l'enregistre
//			QMessageBox::information(this, tr("Succès"), tr("Enregistrement réussi."));
//			griserActions();
//		}
//	}
// }
void InterfaceLecture::carteCiel()
{
	if (m_active) {
		emit afficher(tr("Création de la carte du ciel..."));
		SkyMapDialog dialog(m_carte, this);
		dialog.exec();
	}
}
void InterfaceLecture::diaporama()
{
	if (m_active) {
		bool ok;
		QStringList reponses;
		reponses << tr("1 - Lire la soirée maintenant", "Important : laisser le 1 comme premier caractère") << tr("2 - Démarrer la soirée à l'heure prévue", "Important : laisser le 2 comme premier caractère") << tr("3 - Ne rien faire", "Important : laisser le 3 comme premier caractère");
		QString reponse = QInputDialog::getItem(nullptr, tr("Choisissez le mode"), tr("Comment voulez-vous démarrer la soirée ?"), reponses, 0, false, &ok);
		if (ok) {
			if (reponse.at(0) == '1') // Si on demarre tout de suite
			{
				emit afficher(tr("Initialisation du diaporama en cours..."));
				Diaporama *diapo = new Diaporama(m_soiree);
				diapo->demarrer();
			} else if (reponse.at(0) == '2') // Si on décide de démarrer la soirée à l'heure prévue
			{
				if (m_soiree->getDebut().toLocalTime().date() == QDate::currentDate() && m_soiree->getDebut().toSecsSinceEpoch() > QDateTime::currentDateTimeUtc().toSecsSinceEpoch()) // Si on est sur une soirée qui commence aujourd'hui et après maintenant
				{
					int secondesRestantes(0);
					secondesRestantes = m_soiree->getDebut().toLocalTime().toSecsSinceEpoch() - QDateTime::currentDateTimeUtc().toSecsSinceEpoch();
					Diaporama *diapo = new Diaporama(m_soiree);

					QTimer::singleShot(secondesRestantes * 1000, diapo, &Diaporama::demarrer);
				} else
					QMessageBox::warning(this, tr("Impossible d'effectuer l'action"), tr("Il est impossible de programmer le lancement de cette soirée, elle ne correspond pas à l'heure et à la date actuelle."));
			}
		}
	}
}

void InterfaceLecture::toXML()
{
	if (m_active) {
		QString filename = QFileDialog::getSaveFileName(nullptr, tr("Sauver la soirée au format XML"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/soiree.xml", "Extensible Markup Language (*.xml)");
		if (!filename.isEmpty()) {
			QFile file(filename);
			if (file.open(QIODevice::WriteOnly)) {
				m_soiree->toXML(&file);
				file.close();
			} else {
				qCritical() << "Could not open file.\n" % file.errorString();
			}
		}
	}
}
void InterfaceLecture::toPDF()
{
	if (m_active)
		NightPrinter::toPDF(m_soiree);
}

Soiree *InterfaceLecture::getSoiree()
{
	return m_soiree;
}

void InterfaceLecture::infosSoiree()
{
	if (m_active) {
		QDialog fenetreInfos;
		QLabel *ville = new QLabel(tr("<strong>Ville</strong> : ") + m_soiree->getVille());
		QLabel *pays = new QLabel(tr("<strong>Pays</strong> : ") + m_soiree->getPays());
		QLabel *latitude = new QLabel(tr("<strong>Latitude</strong> : ") + Calculastro::degreeToDms(m_soiree->getLat()));
		QLabel *longitude = new QLabel(tr("<strong>Longitude</strong> : ") + Calculastro::degreeToDms(m_soiree->getLongi()));
		QLabel *debut = new QLabel(tr("<strong>Début</strong> : ") + m_soiree->getDebut().toLocalTime().toString("dd/MM/yyyy hh:mm"));
		QLabel *fin = new QLabel(tr("<strong>Fin</strong> : ") + m_soiree->getFin().toLocalTime().toString("dd/MM/yyyy hh:mm"));
		QLabel *nbObjets = new QLabel(tr("<strong>Nombre d'objets</strong> : ") + QString::number(m_soiree->getPlanning().count()));

		QVBoxLayout *layout = new QVBoxLayout;
		layout->addWidget(pays);
		layout->addWidget(ville);
		layout->addWidget(latitude);
		layout->addWidget(longitude);
		layout->addWidget(debut);
		layout->addWidget(fin);
		layout->addWidget(nbObjets);

		fenetreInfos.setLayout(layout);
		fenetreInfos.setWindowTitle(tr("Infos sur la soirée"));

		fenetreInfos.exec();
	}
}
void InterfaceLecture::griserActions()
{
	if (m_active) {
		if (m_vue->currentIndex().isValid()) // Si on est sur un index valide
		{
			if (m_vue->currentIndex().row() + 1 == m_soiree->getPlanning().count()) // Si on est au dernier item
				m_listeActions->griserActionDescendreObjet(true);
			else
				m_listeActions->griserActionDescendreObjet(false);
			if (m_vue->currentIndex().row() == 0)
				m_listeActions->griserActionMonterObjet(true);
			else
				m_listeActions->griserActionMonterObjet(false);

			m_listeActions->griserActionChangerDureeObjet(false);
			m_listeActions->griserActionSupprimerObjet(false);
			m_listeActions->griserActionAjouterObjet(false);
			m_listeActions->griserActionAjouterPlanete(false);
		} else // Sinon
		{
			m_listeActions->griserActionDescendreObjet(true);
			m_listeActions->griserActionMonterObjet(true);
			m_listeActions->griserActionChangerDureeObjet(true);
			m_listeActions->griserActionSupprimerObjet(true);
			m_listeActions->griserActionAjouterObjet(true);
			m_listeActions->griserActionAjouterPlanete(true);
		}
		if (m_soiree->shouldBeSaved())
			m_listeActions->griserActionEnregistrer(false);
		else
			m_listeActions->griserActionEnregistrer(true);
	}
}
void InterfaceLecture::imprimer()
{
	QPrinter *printer = new QPrinter(QPrinter::ScreenResolution);
	printer->setPageMargins(QMarginsF(7, 7, 7, 7), QPageLayout::Millimeter);

	QPrintDialog *dialog = new QPrintDialog(printer, this);
	dialog->setWindowTitle(tr("Imprimer la soirée"));
	dialog->setOption(QAbstractPrintDialog::PrintSelection);

	if (dialog->exec() != QDialog::Accepted)
		return;

	// m_soiree->paintPdf(printer);
}
