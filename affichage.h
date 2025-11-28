#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include "jeu.h"
// On inclut VOTRE bibliothèque imposée
#include "affichage_console.h"

// Affiche le menu principal
void afficherMenu();

// Affiche toute l'interface (HUD + Grille)
void afficherJeu(Jeu *p);

#endif
