#include "Interface.h"

Interface::Interface(ActionsFenetre *listeActions, QWidget *parent) :
    QWidget(parent)
{
    m_listeActions = listeActions;
}
void Interface::slotFermer()
{
    emit fermer(this);
}
void Interface::setActive(bool active)
{
    m_active = active;
    if (active) // Si l'interface devient active, on actualise les actions
        griserActions();
}
bool Interface::isActive()
{
    return m_active;
}
void Interface::infosSoiree() {}
void Interface::griserActions() {}
