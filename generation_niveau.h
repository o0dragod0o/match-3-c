#ifndef GENERATION_NIVEAU_H
#define GENERATION_NIVEAU_H

#include "noyau.h"

void genererNiveau(Jeu *p);
void placerMurs(Jeu *p);
int calculerTotalItems(int niveau);

// Initialize full game state for a given level
void initialiserJeu(Jeu *p, int niveau);

#endif // GENERATION_NIVEAU_H