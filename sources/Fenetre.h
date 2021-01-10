/* #ifndef FENETRE_H
#define FENETRE_H

#include <QtGui>
#include "ActionsFenetre.h"
#include "FenPrincipal.h"
#include "Soiree.h"

 Classe abstraite qui contient les fenêtres de type CREATION OU LECTURE/MODIFICATION
    - Les fenêtres contiennent une soirée et peuvent la modifier
    - Les fenêtres peuvent enregistrer une soirée
    - Les fenêtres peuvent afficher les infos d'un objet

class Fenetre : public QObject
{
    Q_OBJECT
    public:
        Fenetre(ActionsFenetre *listeActions, Soiree *soiree, QAbstractItemView *vue);

    signals:

    public slots:
        void afficherInfosObjet(Objet *objet);
        void enregistrerSoiree();
        void monterObjet();
        void descendreObjet();
        void supprimerObjet();
        void modifierObjet();

    private:
        Soiree *m_soiree;
        ActionsFenetre *m_listeActions;
        QAbstractItemView *m_vueSoiree;

};

#endif // FENETRE_H */
