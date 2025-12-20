#ifndef NOYAU_H
#define NOYAU_H

#include <time.h>

#define LIGNES 10
#define COLONNES 20

typedef enum {
    VIDE = 0, FRAISE = 1, OIGNON = 2, MANDARINE = 3, MYRTILLE = 4, CITRON = 5, JOKER = 6, MUR = 99
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
    int objectifMurs;
    int mursCasses;

    time_t tempsDebut;
    int dureeMax;

    // Contrôles
    int curseurX, curseurY;
    int aSelectionneUneCase;
    int selectionX, selectionY;
} Jeu; 

#endif // NOYAU_H