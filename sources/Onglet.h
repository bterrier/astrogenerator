#ifndef ONGLET_H
#define ONGLET_H

#include <QObject>
#include <QWidget>

#include "Carteciel.h"
#include "Soiree.h"

class Onglet : public QObject
{
    Q_OBJECT

public:
    Onglet(Soiree *soiree);

    Soiree *soiree() const;
    QWidget *getWidget(QString ref);
    QWidget *current() const;
    Carteciel *carte() const;

private:
    Soiree *m_soiree;
    Carteciel *m_carte;
    QWidget *m_current;
};

#endif // ONGLET_H
