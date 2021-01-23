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
    const QFileInfo fi{ dbPath };
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
                                          "Error: %2").arg(db.lastError().text(), QDir::toNativeSeparators(db.databaseName())));
        qFatal("Failed to open database!");
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName(NOM_PROGRAMME);
    QCoreApplication::setApplicationVersion(VERSION_STRING);
    QCoreApplication::setOrganizationName(NOM_EQUIPE);

    QApplication app(argc, argv);

    if(!QDir::setCurrent(QCoreApplication::applicationDirPath()+"/"))
    {
        QMessageBox::critical(nullptr,QObject::tr("Résolution des liens"),QObject::tr("Problème lors de la résolution des liens, le programme ne peut pas fonctionner correctement."));
        app.quit();
    }

    QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    if(!translator.load(QString("qt_")+locale, "translations"))
        QMessageBox::critical(nullptr,QObject::tr("Traduction"),QObject::tr("L'application ne fonctionnera pas correctement car le fichier : translations/qt_")+locale+QObject::tr(".qm est introuvable."));
    app.installTranslator(&translator);

    initDatabase();

    FenPrincipal fenetre;
    fenetre.show();

    return app.exec();
}
