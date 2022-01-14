#include <QApplication>
#include <QDir>
#include <QLibraryInfo>
#include <QLocale>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QStringBuilder>
#include <QTranslator>

#include "Calculastro.h"
#include "Carteciel.h"
#include "Constantes.h"
#include "FenPrincipal.h"
#include "Objet.h"
#include "Soiree.h"

static void initDatabase()
{
	const QString dbPath = QCoreApplication::applicationDirPath() % "/dbastrogenerator";
	const QFileInfo fi{dbPath};
	if (!fi.isReadable()) {
		QMessageBox::critical(nullptr, QObject::tr("Error"),
		                      QObject::tr("Missing database: %1").arg(QDir::toNativeSeparators(dbPath)));
		qFatal("Database file does not exist!");
	}

	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setHostName("localhost");
	db.setDatabaseName(dbPath);
	db.setUserName("univers2");
	db.setPassword("iwxldmkdgpf");

	if (!db.open()) {
		QMessageBox::critical(nullptr, QObject::tr("Error"),
		                      QObject::tr("Failed to open database: %1\n"
		                                  "Error: %2")
		                          .arg(db.lastError().text(), QDir::toNativeSeparators(db.databaseName())));
		qFatal("Failed to open database!");
	}
}

static void initTranslation()
{
	QTranslator *translator = new QTranslator(qApp);
	if (translator->load(QLocale(), "qt", "_", QLibraryInfo::path(QLibraryInfo::TranslationsPath))) {
		qApp->installTranslator(translator);
	}

	translator = new QTranslator(qApp);
	if (translator->load(QLocale(), "astroGenerator", "_", ":/i18n")) {
		qApp->installTranslator(translator);
	}
}

int main(int argc, char *argv[])
{
	QCoreApplication::setApplicationName(NOM_PROGRAMME);
	QCoreApplication::setApplicationVersion(VERSION_STRING);
	QCoreApplication::setOrganizationName(NOM_EQUIPE);

	QApplication app(argc, argv);

	if (!QDir::setCurrent(QCoreApplication::applicationDirPath() + "/")) {
		QMessageBox::critical(nullptr, QObject::tr("Résolution des liens"), QObject::tr("Problème lors de la résolution des liens, le programme ne peut pas fonctionner correctement."));
		app.quit();
	}

	initTranslation();
	initDatabase();

	FenPrincipal fenetre;
	fenetre.show();

	return app.exec();
}
