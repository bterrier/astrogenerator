#include "WidgetHeure.h"

WidgetHeure::WidgetHeure()
{
    setDigitCount(5);
    tempsActuel = QTime::currentTime();
    QString texte = tempsActuel.toString("hh:mm");
    display(texte);


    timer = new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(ajouterMin()));
    timer->start(60000);
}
void WidgetHeure::ajouterMin()
{
    tempsActuel = tempsActuel.addSecs(60);
    QString texte = tempsActuel.toString("hh:mm");
    display(texte);
}
