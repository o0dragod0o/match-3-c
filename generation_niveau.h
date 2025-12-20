#ifndef GENERATION_NIVEAU_H
#define GENERATION_NIVEAU_H

#include "noyau.h"

void genererNiveau(Jeu *p);
void placerMurs(Jeu *p);
int calculerTotalItems(int niveau);

void initialiserJeu(Jeu *p, int niveau);

#endif