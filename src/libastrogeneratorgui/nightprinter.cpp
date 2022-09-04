#include "nightprinter.h"

#include <QRandomGenerator>
#include <QStandardPaths>
#include <QString>

#include <QDesktopServices>
#include <QPainter>

#include <QApplication>
#include <QFileDialog>
#include <QGraphicsView>
#include <QMessageBox>

#include <QPrinter>

#include "Calculastro.h"
#include "Constantes.h"
#include "ObjetPlaneteObs.h"

#include "Carteciel.h"

void NightPrinter::toPDF(Soiree *night)
{
	QString fileName = QFileDialog::getSaveFileName(nullptr, tr("Exporter la soirée en PDF"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/soiree-pdf.pdf", "*.pdf");
	if (!fileName.isEmpty()) {
		QApplication::setOverrideCursor(Qt::WaitCursor);
		int mT(7), mG(7), mD(7), mB(7); // On crée les marges (en millimètres)

		if (QFileInfo(fileName).suffix().isEmpty())
			fileName.append(".pdf");

		QPrinter printer(QPrinter::ScreenResolution);
		printer.setOutputFormat(QPrinter::PdfFormat);
		printer.setOutputFileName(fileName);
		printer.setPageMargins(QMarginsF(mG, mT, mD, mB), QPageLayout::Millimeter);

		QApplication::restoreOverrideCursor();

		if (printer.isValid()) {
			if (paintPdf(night, &printer)) // ON DIT QUE TOUT S'EST BIEN PASSE
			{
				QMessageBox::information(nullptr, tr("Enregistrement réussi"), tr("La soirée a bien été enregistrée au format PDF"));
				QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
			}
		} else {
			qCritical() << "Erreur d'ouverture, le fichier est-il lisible ?";
		}
	}
}

bool NightPrinter::paintPdf(Soiree *night, QPrinter *printer)
{
	const auto margins = printer->pageLayout().margins(QPageLayout::Millimeter);
	qreal mT = margins.top();
	qreal mG = margins.left();
	qreal mD = margins.right();

	if (!printer || !printer->isValid()) {
		return false;
	}
	QSettings user(NOM_EQUIPE, NOM_PROGRAMME);
	// ON CREE LE PDF
	double k(printer->resolution() / 25.4); // On crée le facteur de positionnement pour la résolution
	QPainter painter(printer);
	if (painter.isActive()) {
		QStringList listeConseils;
		listeConseils << tr("pensez à régler votre télescope") << tr("profitez-en pour prendre une boisson une chaude") << tr("profitez-en pour prendre un goûter") << tr("observez le ciel à l'oeil nu pendant ce temps") << tr("remplissez la fiche de note avec vos impressions d'observations") << tr("si besoin, réalignez votre chercheur");

		// PAGE DE GARDE
		QRectF rect;
		painter.drawImage(52 * k - mG * k, 101 * k - mT * k, QImage(":/icons/bandeau-pdf"));
		QFont font = painter.font();
		font.setPointSize(7 * k);
		font.setFamily("Arial");
		font.setWeight(QFont::Bold);
		painter.setFont(font);
		// On écrit le titre de la soirée
		painter.drawText(QRectF(52.1 * k - mG * k, 132.5 * k - mT * k, 105 * k, 20 * k), Qt::AlignCenter, tr("Soirée d'observation\n") + night->getDebut().toLocalTime().toString("d") + " " + QLocale().monthName(night->getDebut().toLocalTime().date().month()) + " " + night->getDebut().toLocalTime().toString("yyyy"));
		// On écrit l'horaire
		font.setWeight(QFont::Normal);
		font.setPointSize(3 * k);
		painter.setFont(font);
		painter.drawText(QRectF(52.1 * k - mG * k, 152.5 * k - mT * k, 105 * k, 7 * k), Qt::AlignCenter, tr("De ") + night->getDebut().toLocalTime().toString(tr("hh'h'mm")) + tr(" à ") + night->getFin().toLocalTime().toString(tr("hh'h'mm")));
		// On écrit le nombre d'objets
		painter.drawText(QRectF(52.1 * k - mG * k, 159.5 * k - mT * k, 105 * k, 7 * k), Qt::AlignCenter, QString::number(night->getPlanning().size()) + " " + tr("objets"));
		// On écrit la ville et le pays
		painter.drawText(QRectF(0, 250 * k - mT * k, (210 - mD - mG) * k, 7 * k), Qt::AlignRight, tr("Près de ") + night->getVille() + ", " + night->getPays());
		// On écrit les coordonnées
		painter.drawText(QRectF(0, 257 * k - mT * k, (210 - mD - mG) * k, 7 * k), Qt::AlignRight, Calculastro::degreeToDms(night->getLat()) + " / " + Calculastro::degreeToDms(night->getLongi()));
		// On écrit le numéro de page
		font.setPointSize(2 * k);
		painter.setFont(font);
		painter.drawText(QRectF(0, 0, (210 - mD - mG) * k, 7 * k), Qt::AlignRight, "1");

		// PAGE 2
		printer->newPage();
		// On écrit le numéro de page
		font.setPointSize(2 * k);
		painter.setFont(font);
		painter.drawText(QRectF(0, 0, (210 - mD - mG) * k, 7 * k), Qt::AlignRight, "2");
		// On écrit l'intro
		font.setPointSize(3.5 * k);
		painter.setFont(font);
		painter.drawText(QRectF(13 * k - mG * k, 17 * k - mT * k, (210 - mD - mG) * k, 25 * k), Qt::AlignJustify, tr("Avec les informations que vous avez rentrées, nous avons préparé un dossier avec toutes les\ninformations pour réussir votre soirée. Dans ce dossier vous trouverez beaucoup d'infos : une\nliste de conseils, le planning d'observation, une carte du ciel et enfin une page de notes.\nCe dossier est destiné à être imprimé. Bonne observation !"));
		// On écrit le titre
		font.setPointSize(6 * k);
		font.setWeight(QFont::Bold);
		painter.setFont(font);
		painter.setPen(QColor(36, 42, 112));
		painter.drawImage(QRectF(13 * k - mG * k, 45 * k - mT * k, 7 * k, 7 * k), QImage(":/icons/title-pdf.png"));
		painter.drawText(QRectF(22 * k - mG * k, 45 * k - mT * k, (210 - mD - mG) * k, 10 * k), Qt::AlignLeft, tr("Checklist"));
		painter.drawLine(13 * k - mG * k, 55 * k - mT * k, (210 - mD - mG - 13) * k, 55 * k - mT * k);
		// On écrit le texte de la checklist
		font.setPointSize(3 * k);
		font.setWeight(QFont::Normal);
		painter.setFont(font);
		painter.setPen(QColor(0, 0, 0));
		painter.drawText(QRectF(13 * k - mG * k, 59 * k - mT * k, (210 - mD - mG) * k, 5 * k), Qt::AlignLeft, tr("Avant de commencer à observer, pensez à prendre les objets suivants (cochez quand c'est fait) : "));
		// On écrit les items
		painter.drawRect(QRectF(15 * k, 68 * k - mT * k, 5 * k, 5 * k));
		painter.drawText(QRectF(22 * k, 68 * k - mT * k, (210 - mD - mG) * k, 5 * k), Qt::AlignLeft, tr("Une lampe rouge"));

		painter.drawRect(QRectF(15 * k, 75 * k - mT * k, 5 * k, 5 * k));
		painter.drawText(QRectF(22 * k, 75 * k - mT * k, (210 - mD - mG) * k, 5 * k), Qt::AlignLeft, tr("Un laser"));

		painter.drawRect(QRectF(15 * k, 82 * k - mT * k, 5 * k, 5 * k));
		painter.drawText(QRectF(22 * k, 82 * k - mT * k, (210 - mD - mG) * k, 5 * k), Qt::AlignLeft, tr("Des vêtements chauds"));

		painter.drawRect(QRectF(15 * k, 89 * k - mT * k, 5 * k, 5 * k));
		painter.drawText(QRectF(22 * k, 89 * k - mT * k, (210 - mD - mG) * k, 5 * k), Qt::AlignLeft, tr("Une boisson chaude (ex : café)"));

		painter.drawRect(QRectF(15 * k, 96 * k - mT * k, 5 * k, 5 * k));
		painter.drawText(QRectF(22 * k, 96 * k - mT * k, (210 - mD - mG) * k, 5 * k), Qt::AlignLeft, tr("Un goûter"));

		painter.drawRect(QRectF(15 * k, 103 * k - mT * k, 5 * k, 5 * k));
		painter.drawText(QRectF(22 * k, 103 * k - mT * k, (210 - mD - mG) * k, 5 * k), Qt::AlignLeft, tr("Un stylo (pour noter vos observations)"));

		painter.drawRect(QRectF(15 * k, 110 * k - mT * k, 5 * k, 5 * k));
		painter.drawText(QRectF(22 * k, 110 * k - mT * k, (210 - mD - mG) * k, 5 * k), Qt::AlignLeft, tr("Les accessoires du télescope (oculaires, barlow, filtres etc.)"));
		// On écrit l'autre titre (planning d'observation de la soirée)
		font.setPointSize(6 * k);
		font.setWeight(QFont::Bold);
		painter.setFont(font);
		painter.setPen(QColor(36, 42, 112));
		painter.drawImage(QRectF(13 * k - mG * k, 125 * k - mT * k, 7 * k, 7 * k), QImage(":/icons/title-pdf.png"));
		painter.drawText(QRectF(22 * k - mG * k, 125 * k - mT * k, (210 - mD - mG) * k, 10 * k), Qt::AlignLeft, tr("Planning d'observation de la soirée"));
		painter.drawLine(13 * k - mG * k, 135 * k - mT * k, (210 - mD - mG - 13) * k, 135 * k - mT * k);
		// On écrit la légende
		font.setPointSize(3.7 * k);
		font.setWeight(QFont::Normal);
		painter.setFont(font);
		painter.setPen(QColor(0, 0, 0));
		painter.drawText(QRectF(13 * k - mG * k, 140 * k - mT * k, (210 - mD - mG) * k, 50 * k), Qt::AlignLeft, tr("Le planning suivant donne plusieurs informations pratiques pour faciliter le repérage et\nl'observation des objets. Au dessus vous avez le nom principal de l'objet suivi si nécessaire\ndes autres références. Les deux colonnes de gauche vous indiquent les informations fixes\nsur l'objet (AD, DEC, magnitude etc...). La troisième colonne fournit l'heure d'observation\nconseillée. Enfin la dernière colonne renseigne sur l'azimut et la hauteur de l'objet au début\nde son observation. Vous avez également un conseil sur l'oculaire à utiliser. Il peut arriver\nque des pauses s'intercalent entre deux objets, dans ces cas-là, un conseil vous est donné\npour patienter. Bonne observation !"));
		// On écrit le bloc de mise en température
		painter.setPen(QColor(136, 136, 136));
		rect = QRectF(13 * k - mG * k, 192 * k - mT * k, (210 - mD - mG - 13) * k, 8 * k);
		painter.fillRect(rect, QColor(238, 238, 238));
		painter.drawLine(rect.topLeft(), rect.topRight());
		painter.drawLine(rect.topRight(), rect.bottomRight());
		painter.drawLine(rect.bottomRight(), rect.bottomLeft());
		painter.drawLine(rect.bottomLeft(), rect.topLeft());
		painter.setPen(QColor(0, 0, 0));
		painter.drawText(QRectF(13 * k - mG * k, 192.3 * k - mT * k, (210 - mD - mG - 13) * k, 8 * k), Qt::AlignCenter, tr("Conseil : Sortez votre télescope vers ") + night->getDebut().addSecs(-Calculastro::miseEnTemperature(night->getDiametre()) * 60).toString("hh'h'mm") + tr(" pour le mettre en température"));
		// On écrit tous les autres blocs dans une boucle

		int tL(204), page(2); // Le point en haut à gauche du premier bloc
		QString icone;
		QVector<double> hauAzi;
		for (int i(0); i < night->getPlanning().count(); i++) { // [FAIRE]  oculaires
			if (tL + 28 > 285) {
				printer->newPage();
				page++;
				tL = 27;
				// On écrit le numéro de page
				font.setPointSize(2 * k);
				painter.setFont(font);
				painter.drawText(QRectF(0, 0, (210 - mD - mG) * k, 7 * k), Qt::AlignRight, QString::number(page));
			}
			if (i > 0 && (night->getPlanning().at(i)->getDebut().toSecsSinceEpoch() - night->getPlanning().at(i - 1)->getFin().toSecsSinceEpoch()) > user.value("generateur/pauseMin", TEMPS_ESPACE).toUInt() * 60) {
				// Si on est pas au premier objet et que le temps entre cet objet et celui d'avant est supérieur à TEMPS_ESPACE, alors on affiche une pause
				int pause = night->getPlanning().at(i)->getDebut().toSecsSinceEpoch() - night->getPlanning().at(i - 1)->getFin().toSecsSinceEpoch();
				pause = (int)pause / 60;
				painter.setPen(QColor(136, 136, 136));
				rect = QRectF(13 * k - mG * k, tL * k - mT * k, (210 - mD - mG - 13) * k, 6 * k);
				painter.fillRect(rect, QColor(238, 238, 238));
				painter.drawLine(rect.topLeft(), rect.topRight());
				painter.drawLine(rect.topRight(), rect.bottomRight());
				painter.drawLine(rect.bottomRight(), rect.bottomLeft());
				painter.drawLine(rect.bottomLeft(), rect.topLeft());
				painter.setPen(QColor(0, 0, 0));
				font.setPointSize(2.2 * k);
				painter.setFont(font);
				painter.drawText(QRectF(13 * k - mG * k, tL * k - mT * k, (210 - mD - mG - 13) * k, 6 * k), Qt::AlignCenter, tr("Pause de ") + QString::number(pause) + tr(" min. Conseil : ") + listeConseils.at(QRandomGenerator::global()->bounded(listeConseils.size())));
				tL += 10;
				if (tL + 28 > 285) {
					printer->newPage();
					page++;

					// On écrit le numéro de page
					font.setPointSize(2 * k);
					painter.setFont(font);
					painter.drawText(QRectF(0, 0, (210 - mD - mG) * k, 7 * k), Qt::AlignRight, QString::number(page));

					tL = 27;
				}
			}
			if (QFile::exists("icones/" + night->getPlanning().at(i)->ref() + ".jpg"))
				icone = "icones/" + night->getPlanning().at(i)->ref() + ".jpg";
			else
				icone = "icones/default.png";

			painter.setPen(QColor(136, 136, 136));
			// Rectangle principal
			painter.drawRect(QRectF(13 * k - mG * k, tL * k - mT * k, (210 - mD - mG - 13) * k, 28 * k));
			// Image
			painter.drawImage(QRectF(13 * k - mG * k, tL * k - mT * k, 28 * k, 28 * k), QImage(icone));
			// Nom de l'objet
			font.setPointSize(2.8 * k);
			painter.setFont(font);
			rect = QRectF(45 * k - mG * k, (tL + 1) * k - mT * k, (210 - mD - mG - 45) * k, 5 * k);
			painter.fillRect(rect, QColor(238, 238, 238));
			painter.drawLine(rect.topLeft(), rect.topRight());
			painter.drawLine(rect.topRight(), rect.bottomRight());
			painter.drawLine(rect.bottomRight(), rect.bottomLeft());
			painter.drawLine(rect.bottomLeft(), rect.topLeft());
			painter.setPen(QColor(0, 0, 0));
			painter.drawText(QRectF(46 * k - mG * k, (tL + 1.3) * k - mT * k, (210 - mD - mG - 46) * k, 5 * k), Qt::AlignLeft, night->getPlanning().at(i)->nomComplet());
			// Constellation
			font.setPointSize(1.6 * k);
			painter.setFont(font);
			painter.drawText(QRectF(46 * k - mG * k, (tL + 1.75) * k - mT * k, (210 - mD - mG - 48) * k, 5 * k), Qt::AlignRight, Calculastro::abreviationToNom(night->getPlanning().at(i)->constellation()));
			// On écrit les infos de l'objet
			font.setPointSize(2.2 * k);
			painter.setFont(font);
			painter.drawText(QRectF(48 * k - mG * k, (tL + 9) * k - mT * k, 30 * k, 4 * k), Qt::AlignLeft, tr("AD : ") + night->getPlanning().at(i)->ascdr());
			painter.drawText(QRectF(48 * k - mG * k, (tL + 14) * k - mT * k, 30 * k, 4 * k), Qt::AlignLeft, tr("DEC : ") + night->getPlanning().at(i)->declinaison());
			painter.drawText(QRectF(48 * k - mG * k, (tL + 19) * k - mT * k, 30 * k, 4 * k), Qt::AlignLeft, tr("Magnitude : ") + QString::number(night->getPlanning().at(i)->magnitude()));

			if (night->getPlanning().at(i)->type() == "Planète") {
				ObjetPlaneteObs *planete = qobject_cast<ObjetPlaneteObs *>(night->getPlanning().at(i));
				painter.drawText(QRectF(80 * k - mG * k, (tL + 9) * k - mT * k, 40 * k, 4 * k), Qt::AlignLeft, tr("Illumination : ") + QString::number(planete->illumination()) + "%");
				painter.drawText(QRectF(80 * k - mG * k, (tL + 14) * k - mT * k, 40 * k, 4 * k), Qt::AlignLeft, tr("Distance : ") + QString::number(planete->distance()) + " UA");
				painter.drawText(QRectF(80 * k - mG * k, (tL + 19) * k - mT * k, 40 * k, 4 * k), Qt::AlignLeft, tr("Type : ") + night->getPlanning().at(i)->type());
			} else {
				painter.drawText(QRectF(80 * k - mG * k, (tL + 9) * k - mT * k, 40 * k, 4 * k), Qt::AlignLeft, tr("Interêt : ") + night->getPlanning().at(i)->interet(true));
				painter.drawText(QRectF(80 * k - mG * k, (tL + 14) * k - mT * k, 40 * k, 4 * k), Qt::AlignLeft, tr("Taille : ") + QString::number(night->getPlanning().at(i)->taille()) + "'");
				painter.drawText(QRectF(80 * k - mG * k, (tL + 19) * k - mT * k, 40 * k, 4 * k), Qt::AlignLeft, tr("Type : ") + night->getPlanning().at(i)->type());
			}

			painter.drawText(QRectF(124 * k - mG * k, (tL + 9) * k - mT * k, 30 * k, 4 * k), Qt::AlignLeft, tr("Observer entre"));
			font.setPointSize(3 * k);
			painter.setFont(font);
			painter.drawText(QRectF(124 * k - mG * k, (tL + 14) * k - mT * k, 30 * k, 7 * k), Qt::AlignLeft, night->getPlanning().at(i)->getDebut().toLocalTime().toString("hh'h'mm") + tr(" et ") + night->getPlanning().at(i)->getFin().toLocalTime().toString("hh'h'mm"));
			// On trace la ligne de séparation
			painter.setPen(QColor(136, 136, 136));
			painter.drawLine(158 * k - mG * k, (tL + 9) * k - mT * k, 158 * k - mG * k, (tL + 23) * k - mT * k);
			// On écrit les infos de droite (hauteur, azimut,oculaire)
			painter.setPen(QColor(0, 0, 0));
			font.setPointSize(2.2 * k);
			painter.setFont(font);
			hauAzi = night->hauteurAzimutObjet(i);
			painter.drawText(QRectF(163 * k - mG * k, (tL + 9) * k - mT * k, 40 * k, 4 * k), Qt::AlignLeft, tr("Hauteur : ") + Calculastro::degreeToDms(hauAzi.at(0)));
			painter.drawText(QRectF(163 * k - mG * k, (tL + 14) * k - mT * k, 40 * k, 4 * k), Qt::AlignLeft, tr("Azimut : ") + Calculastro::degreeToDms(hauAzi.at(1)));
			painter.drawText(QRectF(163 * k - mG * k, (tL + 19) * k - mT * k, 40 * k, 4 * k),
			                 Qt::AlignLeft, tr("Oculaire : ") + Calculastro::getOculaire(night->getPlanning().at(i), night->getDiametre(), night->getFocale(), Settings::instance().eyepieces()));

			tL += 32;
		}
		printer->newPage(); // On change de page
		page++;
		// On écrit le numéro de page
		font.setPointSize(2 * k);
		painter.setFont(font);
		painter.drawText(QRectF(0, 0, (210 - mD - mG) * k, 7 * k), Qt::AlignRight, QString::number(page));
		// On écrit le titre : CARTE DU CIEL
		font.setPointSize(6 * k);
		font.setWeight(QFont::Bold);
		painter.setFont(font);
		painter.setPen(QColor(36, 42, 112));
		painter.drawImage(QRectF(13 * k - mG * k, 10 * k - mT * k, 7 * k, 7 * k), QImage(":/icons/title-pdf.png"));
		painter.drawText(QRectF(22 * k - mG * k, 10 * k - mT * k, (210 - mD - mG) * k, 10 * k), Qt::AlignLeft, tr("Carte du ciel"));
		painter.drawLine(13 * k - mG * k, 20 * k - mT * k, (210 - mD - mG - 13) * k, 20 * k - mT * k);
		// On écrit la légende
		font.setPointSize(3.7 * k);
		font.setWeight(QFont::Normal);
		painter.setFont(font);
		painter.setPen(QColor(0, 0, 0));
		QDateTime moitieObs;
		moitieObs.setSecsSinceEpoch((night->getDebut().toSecsSinceEpoch() + night->getFin().toSecsSinceEpoch()) / 2);
		painter.drawText(QRectF(13 * k - mG * k, 25 * k - mT * k, (210 - mD - mG) * k, 10 * k), Qt::AlignLeft, tr("Carte du ciel à la moitié de la soirée d'observation(") + moitieObs.toString("hh'h'mm") + tr("). Seuls les objets de la soirée sont\nplacés."));
		// On dessine l'image
		Carteciel skymap(night);
		QGraphicsView vue(&skymap);
		vue.setBackgroundBrush(QColor(255, 255, 255));
		vue.setFixedSize(800, 800);
		QPixmap *pixmap = new QPixmap(800, 800);
		QPainter *painterImage = new QPainter(pixmap);
		painterImage->setRenderHint(QPainter::Antialiasing);
		vue.render(painterImage, QRectF(0, 0, 800, 800), QRect(0, 0, 800, 800));
		painterImage->end();
		painter.drawImage(QRectF(13 * k - mG * k, 45 * k - mT * k, 185 * k, 185 * k), pixmap->toImage());

		// ON FAIT LES DEUX PAGES DE NOTES
		printer->newPage();
		page++;
		// On écrit le numéro de page
		font.setPointSize(2 * k);
		painter.setFont(font);
		painter.drawText(QRectF(0, 0, (210 - mD - mG) * k, 7 * k), Qt::AlignRight, QString::number(page));
		// Ecriture du titre
		font.setPointSize(6 * k);
		font.setWeight(QFont::Bold);
		painter.setFont(font);
		painter.setPen(QColor(36, 42, 112));
		painter.drawImage(QRectF(13 * k - mG * k, 10 * k - mT * k, 7 * k, 7 * k), QImage(":/icons/title-pdf.png"));
		painter.drawText(QRectF(22 * k - mG * k, 10 * k - mT * k, (210 - mD - mG) * k, 10 * k), Qt::AlignLeft, tr("Pages de notes"));
		painter.drawLine(13 * k - mG * k, 20 * k - mT * k, (210 - mD - mG - 13) * k, 20 * k - mT * k);
		// Ecriture des pointillés
		font.setPointSize(3.7 * k);
		font.setWeight(QFont::Normal);
		painter.setFont(font);
		painter.setPen(QColor(200, 200, 200));
		QString pointilles;
		for (int j(1); j <= 5217; j++) {
			pointilles += "-";
			if (j % 111 == 0 && j > 0)
				pointilles += "\n";
		}
		painter.drawText(QRectF(13 * k - mG * k, 25 * k - mT * k, (210 - mD - mG) * k, 250 * k), Qt::AlignLeft, pointilles);
		printer->newPage();
		page++;
		// On écrit le numéro de page
		painter.setPen(QColor(0, 0, 0));
		font.setPointSize(2 * k);
		painter.setFont(font);
		painter.drawText(QRectF(0, 0, (210 - mD - mG) * k, 7 * k), Qt::AlignRight, QString::number(page));
		// On met les pointillés
		painter.setPen(QColor(200, 200, 200));
		font.setPointSize(3.7 * k);
		painter.setFont(font);
		painter.drawText(QRectF(13 * k - mG * k, 10 * k - mT * k, (210 - mD - mG) * k, 250 * k), Qt::AlignLeft, pointilles);

		return true;
	} else
		return false;
}
