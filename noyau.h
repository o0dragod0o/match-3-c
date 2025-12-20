#ifndef NOYAU_H
#define NOYAU_H

#include <time.h>

#define LIGNES 10
#define COLONNES 20

typedef enum {
    VIDE = 0, COOKIE = 1, CHOCOLAT = 2, BONBON = 3, SUCETTE = 4, GLACE = 5, JOKER = 6, MUR = 99
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

    int objectifs[6];
    int collecte[6];
    int objectifMurs;
    int mursCasses;

    time_t tempsDebut;
    int dureeMax;

    int curseurX, curseurY;
    int aSelectionneUneCase;
    int selectionX, selectionY;
} Jeu;

#endif