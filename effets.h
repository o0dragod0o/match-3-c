#ifndef EFFETS_H
#define EFFETS_H

#include "noyau.h"

void marquerToutType(Jeu *p, TypeItem t);
int verifierAlignements(Jeu *p);
void supprimerItems(Jeu *p);

// Game actions
int verifierVictoire(Jeu *p);
int permuterItems(Jeu *p, int x1, int y1, int x2, int y2);

#endif // EFFETS_H