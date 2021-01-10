#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include "FenPrincipal.h"
#include "Calculastro.h"
#include "Carteciel.h"
#include "Objet.h"
#include "Soiree.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    if(!QDir::setCurrent(QCoreApplication::applicationDirPath()+"/"))
    {
        QMessageBox::critical(0,QObject::tr("Résolution des liens"),QObject::tr("Problème lors de la résolution des liens, le programme ne peut pas fonctionner correctement."));
        app.quit();
    }

    QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    if(!translator.load(QString("qt_")+locale, "translations"))
       QMessageBox::critical(0,QObject::tr("Traduction"),QObject::tr("L'application ne fonctionnera pas correctement car le fichier : translations/qt_")+locale+QObject::tr(".qm est introuvable."));
    app.installTranslator(&translator);

    FenPrincipal fenetre;
    fenetre.show();

    return app.exec();
}
