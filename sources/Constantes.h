#ifndef CONSTANTES_H
#define CONSTANTES_H

    #define NOM_PROGRAMME     "astroGenerator"
    #define NOM_EQUIPE        "ua"
    #define VERSION           "3.0.0"
    #define FICHIER_VERSION   "http://www.univers-astronomie.fr/generateur-soiree/astrogenerator/version.vs"
    #define PAGE_TELECHARGEMENT "http://www.univers-astronomie.fr/generateur-soiree/astrogenerator/telecharger.php"

    #define HAUTEUR_MIN_OBJET 10   // La hauteur minimum pour qu'un objet soit placé dans la soirée (en °)
    #define TEMPS_ESPACE      3    // Espace minimum entre deux objets dans la soirée
    #define PAUSE_MINIMUM_OBJET 0  // Temps de pause minimum entre deux objets (min) - Minimum réglable
    #define PAUSE_MAXIMUM_OBJET 20 // Temps de pause minimum entre deux objets (min) - Maximum réglable
    #define DIAMETRE_MAXIMUM  999  // En mm
    #define DIAMETRE_MINIMUM  10   // En mm
    #define DIAMETRE_DEFAULT  150  // En mm
    #define FOCALE_MAXIMUM    9999 // En mm
    #define FOCALE_MINIMUM    99   // En mm
    #define FOCALE_DEFAULT    750  // En mm
    #define DUREE_SOIREE_MAX  10   // En heures
    #define DUREE_SOIREE_MIN  1    // En heures
    #define DUREE_OBJET_MAX   30   // En minutes
    #define DUREE_OBJET_MIN   5    // En minutes
    #define TEMPS_OBS_DEFAUT  5    // En minutes
    #define NB_SOIREES_RECENTES 5  // Nombre de soirées récentes enregistrées et affichées dans le programme

    #define COULEUR_FOND          "#202151"
    #define COULEUR_LEGENDE       "#FFFFFF"
    #define COULEUR_OBJET         "#FF8000"
    #define COULEUR_ETOILE        "#FFFF00"
    #define COULEUR_CONSTELLATION "#FFFFFF"

    #define URL_UNIVERS_ASTRONOMIE "http://www.univers-astronomie.fr"
    #define URL_GENERATEUR         "http://www.univers-astronomie.fr/generateur-soiree/"
    #define URL_CDS                "http://cds.u-strasbg.fr/"
    #define DEBUT_URL_CDS_OBJET    "http://simbad.u-strasbg.fr/simbad/sim-id?Ident="
    #define URL_OBJET_REMARQUABLE  "http://www.univers-astronomie.fr/articles/techniques/121-50-objets-remarquables-du-ciel-profond.html"

constexpr auto URL_GITHUB = "https://github.com/bterrier/astrogenerator";

    #define HAUTEUR_COMPTE_REBOURS_DIAPO 80
    #define LARGEUR_COMPTE_REBOURS_DIAPO 200

    #define HAUTEUR_BLOC_PAGE_ACCUEIL 250
    #define LARGEUR_BLOC_PAGE_ACCUEIL 250

    #define HAUTEUR_MIN_FENETRE 700
    #define LARGEUR_MIN_FENETRE 700

    #define LARGEUR_LISTE_OBJET_FENETRE_CREATION 300
    #define COORDONNEES_INCONNU     "Unknow" // Remplace le nom de la ville et du pays quand il est inconnu

    #define NOTE_AMAS_GLOBULAIRE        15 // Note sur 15 des objets par défaut
    #define NOTE_AMAS_OUVERT            6
    #define NOTE_AMAS_NEBULEUSE         13
    #define NOTE_NEBULEUSE_REFLECTION   10
    #define NOTE_NEBULEUSE_PLANETAIRE   11
    #define NOTE_ETOILE_DOUBLE          10
    #define NOTE_ETOILE_TRIPLE          11
    #define NOTE_GALAXIE                8

    #define PAYS_DEFAUT                 "France"
    #define VILLE_DEFAUT                "Nantes"

    #define TELESCOPE_DEFAUT            " Omegon N 114/900 EQ-1"
    #define OCULAIREES_DEFAUT           "6|15|25"

    #define NOM_FICHIER_SOIREE_RECENTES "soireesRecentes.in"



#endif // CONSTANTES_H
