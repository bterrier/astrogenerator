#include "skymapdialog.h"
#include "ui_skymapdialog.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>

SkyMapDialog::SkyMapDialog(Carteciel *map, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SkyMapDialog),
    m_map(map)
{
	ui->setupUi(this);
	ui->graphicsView->setScene(m_map);
	connect(ui->pbSave, &QPushButton::clicked, this, &SkyMapDialog::saveImage);
}

SkyMapDialog::~SkyMapDialog()
{
	delete ui;
}

void SkyMapDialog::saveImage()
{
	QString fichier = QFileDialog::getSaveFileName(this, "Enregistrer la carte",
	                                               QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/carte.png",
	                                               "Images (*.png *.gif *.jpg *.jpeg)");

	if (fichier.isEmpty()) {
		return;
	}

	QScopedPointer<QGraphicsView> vue{new QGraphicsView(m_map)};
	vue->setBackgroundBrush(QColor(255, 255, 255));
	vue->setFixedSize(800, 800);

	QPixmap *pixmap = new QPixmap(800, 800);
	QPainter *painter = new QPainter(pixmap);

	painter->setRenderHint(QPainter::Antialiasing);
	vue->render(painter, QRectF(0, 0, 800, 800), QRect(0, 0, 800, 800));
	painter->end();

	if (pixmap->save(fichier))
		QMessageBox::information(this, "Enregistrement réussi", "La carte du ciel a bien été enregistrée");
	else
		QMessageBox::critical(this, "Erreur d'enregistrement", "L'enregistrement de l'image a échoué");
}
