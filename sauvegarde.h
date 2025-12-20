#ifndef SAUVEGARDE_H
#define SAUVEGARDE_H

#include "noyau.h"

// Return codes: 0 = fail, 1 = old-format load, 2 = v2 full load
int sauvegarderPartie(Jeu *p);
int chargerPartie(Jeu *p);

#endif // SAUVEGARDE_H