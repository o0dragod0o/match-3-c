#ifndef JEU_H
#define JEU_H

#include <time.h>

#define LIGNES 10
#define COLONNES 20

typedef enum {
    VIDE = 0, FRAISE = 1, OIGNON = 2, MANDARINE = 3, MYRTILLE = 4, CITRON = 5, MUR = 99
} TypeItem;

typedef struct {
    TypeItem type;
    int estSelectionne;
    int aSupprimer;
} Case;

typedef struct {
    Case plateau[LIGNES][COLONNES];
    int score;
    int vies;
    int coupsRestants;
    int niveauActuel;

    // Contrat et Temps
    int objectifs[6];
    int collecte[6];
    time_t tempsDebut;
    int dureeMax;

    // Contrôles
    int curseurX, curseurY;
    int aSelectionneUneCase;
    int selectX, selectY;
} Jeu;

// Prototypes
void initialiserJeu(Jeu *p, int niveau);
void genererNiveau(Jeu *p);
int permuterItems(Jeu *p, int x1, int y1, int x2, int y2);
int verifierAlignements(Jeu *p);
void supprimerItems(Jeu *p);
void appliquerGravite(Jeu *p);
void sauvegarderPartie(Jeu *p);
int chargerPartie(Jeu *p);

#endif
