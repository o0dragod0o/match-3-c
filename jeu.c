#include "jeu.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

void initialiserJeu(Jeu *p, int niveau) {
    p->score = 0;
    p->vies = 3;
    p->niveauActuel = niveau;
    p->coupsRestants = 20; // Réduit car la grille est plus petite
    p->curseurX = COLONNES / 2;
    p->curseurY = LIGNES / 2;
    p->aSelectionneUneCase = 0;
    genererNiveau(p);

    // On s'assure qu'au début, il n'y a pas déjà des alignements
    while (verifierAlignements(p)) {
        supprimerItems(p);
        appliquerGravite(p);
        p->score = 0; // On remet le score à 0 après le nettoyage initial
    }
}

void genererNiveau(Jeu *p) {
    srand((unsigned)time(NULL));
    for (int i = 0; i < LIGNES; i++) {
        for (int j = 0; j < COLONNES; j++) {
            p->plateau[i][j].type = (rand() % 5) + 1;
            p->plateau[i][j].estSelectionne = 0;
            p->plateau[i][j].aSupprimer = 0;
        }
    }
}

int permuterItems(Jeu *p, int x1, int y1, int x2, int y2) {
    // 1. On fait l'échange
    TypeItem temp = p->plateau[y1][x1].type;
    p->plateau[y1][x1].type = p->plateau[y2][x2].type;
    p->plateau[y2][x2].type = temp;

    // 2. On vérifie si cet échange crée un alignement
    if (verifierAlignements(p)) {
        // C'est un bon coup !
        p->coupsRestants--;
        return 1; // Succès
    } else {
        // Mauvais coup : on annule l'échange (retour à la position initiale)
        temp = p->plateau[y1][x1].type;
        p->plateau[y1][x1].type = p->plateau[y2][x2].type;
        p->plateau[y2][x2].type = temp;
        return 0; // Echec
    }
}

// Détecte les lignes de 3 ou plus
int verifierAlignements(Jeu *p) {
    int matchTrouve = 0;

    // Réinitialiser les marqueurs
    for(int y=0; y<LIGNES; y++)
        for(int x=0; x<COLONNES; x++)
            p->plateau[y][x].aSupprimer = 0;

    // 1. Vérification Horizontale
    for (int y = 0; y < LIGNES; y++) {
        for (int x = 0; x < COLONNES - 2; x++) {
            TypeItem type = p->plateau[y][x].type;
            if (type != VIDE && type != MUR) {
                if (p->plateau[y][x+1].type == type && p->plateau[y][x+2].type == type) {
                    // On marque les 3 cases
                    p->plateau[y][x].aSupprimer = 1;
                    p->plateau[y][x+1].aSupprimer = 1;
                    p->plateau[y][x+2].aSupprimer = 1;
                    matchTrouve = 1;
                }
            }
        }
    }

    // 2. Vérification Verticale
    for (int x = 0; x < COLONNES; x++) {
        for (int y = 0; y < LIGNES - 2; y++) {
            TypeItem type = p->plateau[y][x].type;
            if (type != VIDE && type != MUR) {
                if (p->plateau[y+1][x].type == type && p->plateau[y+2][x].type == type) {
                    // On marque les 3 cases
                    p->plateau[y][x].aSupprimer = 1;
                    p->plateau[y+1][x].aSupprimer = 1;
                    p->plateau[y+2][x].aSupprimer = 1;
                    matchTrouve = 1;
                }
            }
        }
    }

    return matchTrouve;
}

void supprimerItems(Jeu *p) {
    for (int i = 0; i < LIGNES; i++) {
        for (int j = 0; j < COLONNES; j++) {
            if (p->plateau[i][j].aSupprimer) {
                p->plateau[i][j].type = VIDE; // On vide la case
                p->plateau[i][j].aSupprimer = 0;
                p->score += 10; // Gain de points ici
            }
        }
    }
}

void appliquerGravite(Jeu *p) {
    // Pour chaque colonne...
    for (int j = 0; j < COLONNES; j++) {
        int ecriture = LIGNES - 1; // Position où on doit écrire le prochain item

        // Etape 1 : On fait tomber les items existants
        // On parcourt la colonne de bas en haut
        for (int lecture = LIGNES - 1; lecture >= 0; lecture--) {
            if (p->plateau[lecture][j].type != VIDE && p->plateau[lecture][j].type != MUR) {
                // Si on trouve un item, on le place à la position d'écriture (tout en bas possible)
                p->plateau[ecriture][j].type = p->plateau[lecture][j].type;

                // Si l'item a bougé (lecture != ecriture), on vide l'ancienne case
                if (ecriture != lecture) {
                    p->plateau[lecture][j].type = VIDE;
                }
                ecriture--; // On remonte d'un cran pour le prochain item
            }
            else if (p->plateau[lecture][j].type == MUR) {
                // Les murs bloquent la gravité, on réinitialise l'écriture au dessus du mur
                ecriture = lecture - 1;
            }
        }

        // Etape 2 : On remplit les trous restants en haut avec de l'aléatoire
        while (ecriture >= 0) {
            p->plateau[ecriture][j].type = (rand() % 5) + 1;
            ecriture--;
        }
    }
}
