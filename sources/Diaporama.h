#ifndef DIAPORAMA_H
#define DIAPORAMA_H

#include <QTimer>
#include <QVector>
#include <QWidget>

#include "Soiree.h"
#include "CompteRebours.h"

class Diaporama : public QWidget
{
    Q_OBJECT

    public:
        explicit Diaporama(Soiree* soiree);
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
