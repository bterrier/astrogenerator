#ifndef WIDGETHEURE_H
#define WIDGETHEURE_H

#include <QLCDNumber>
#include <QTime>
#include <QTimer>

class WidgetHeure : public QLCDNumber
{
    Q_OBJECT

    public:
        WidgetHeure();
    public slots :
        void ajouterMin();
    private :
         QTime tempsActuel;
         QTimer *timer;
    
};

#endif // WIDGETHEURE_H
