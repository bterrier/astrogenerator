#ifndef DIAPORAMA_H
#define DIAPORAMA_H

#include <QtGui>
#include "Soiree.h"
#include "CompteRebours.h"

class Diaporama : public QWidget
{
    Q_OBJECT

    public:
        Diaporama(Soiree* soiree);
        void keyPressEvent(QKeyEvent *event);
    public slots:
        void afficher(int id);
        void suivant();
        void precedent();
        void pause(int secondes);
        void demarrer();

    private:
        QVector<QWidget*> widgets;
        int diapoCurrent;
        QTimer *timer;
        Soiree *m_soiree;

        QWidget *widgetPause;
        CompteRebours *lcd_pause;

        bool m_pause;
};

#endif // DIAPORAMA_H
