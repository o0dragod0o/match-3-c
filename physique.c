#include "physique.h"
#include <stdlib.h>

int glisserDiagonale(Jeu *p) {
    int moved = 0;
    for (int i = LIGNES - 1; i >= 1; i--) {
        for (int j = 0; j < COLONNES; j++) {
            if (p->plateau[i][j].type != VIDE) continue;
            int canFromAbove = 0;
            for (int k = i - 1; k >= 0; k--) {
                if (p->plateau[k][j].type == MUR) {
                    canFromAbove = 0;
                    break;
                }
                if (p->plateau[k][j].type != VIDE) {
                    canFromAbove = 1;
                    break;
                }
            }
            if (canFromAbove) continue;

            if (j > 0 && p->plateau[i-1][j-1].type != VIDE && p->plateau[i-1][j-1].type != MUR) {
                p->plateau[i][j].type = p->plateau[i-1][j-1].type;
                p->plateau[i][j].estSelectionne = 0;
                p->plateau[i][j].aSupprimer = 0;
                p->plateau[i-1][j-1].type = VIDE;
                p->plateau[i-1][j-1].estSelectionne = 0;
                p->plateau[i-1][j-1].aSupprimer = 0;
                moved++;
                continue;
            }
            if (j < COLONNES - 1 && p->plateau[i-1][j+1].type != VIDE && p->plateau[i-1][j+1].type != MUR) {
                p->plateau[i][j].type = p->plateau[i-1][j+1].type;
                p->plateau[i][j].estSelectionne = 0;
                p->plateau[i][j].aSupprimer = 0;
                p->plateau[i-1][j+1].type = VIDE;
                p->plateau[i-1][j+1].estSelectionne = 0;
                p->plateau[i-1][j+1].aSupprimer = 0;
                moved++;
                continue;
            }
        }
    }
    return moved;
}

void appliquerGravite(Jeu *p) {
    for (int j=0; j<COLONNES; j++) {
        int w = LIGNES - 1;
        // Déplace items vers le bas
        for (int r = LIGNES - 1; r >= 0; r--) {
            if (p->plateau[r][j].type == MUR) {
                w = r - 1;
            } else if (p->plateau[r][j].type != VIDE) {
                p->plateau[w][j].type = p->plateau[r][j].type;
                p->plateau[w][j].estSelectionne = 0;
                p->plateau[w][j].aSupprimer = 0;
                if (w != r) {
                    p->plateau[r][j].type = VIDE;
                    p->plateau[r][j].estSelectionne = 0;
                    p->plateau[r][j].aSupprimer = 0;
                }
                w--;
            }
        }
        while (w >= 0) {
            int attempt;
            TypeItem chosen = VIDE;
            for (attempt = 0; attempt < 20; attempt++) {
                // joker chance faible
                TypeItem cand = ((rand() % 100) < 4) ? JOKER : (TypeItem)((rand() % 5) + 1);
                if (cand != JOKER && w+2 < LIGNES && p->plateau[w+1][j].type == cand && p->plateau[w+2][j].type == cand) continue;
                if (cand != JOKER && j >= 2 && p->plateau[w][j-1].type == cand && p->plateau[w][j-2].type == cand) continue;
                if (cand != JOKER && j+2 < COLONNES && p->plateau[w][j+1].type == cand && p->plateau[w][j+2].type == cand) continue;
                if (cand != JOKER && j >=1 && j+1 < COLONNES && p->plateau[w][j-1].type == cand && p->plateau[w][j+1].type == cand) continue;
                chosen = cand; break;
            }
            if (chosen == VIDE) chosen = ((rand() % 100) < 4) ? JOKER : (TypeItem)((rand() % 5) + 1);
            p->plateau[w][j].type = chosen;
            p->plateau[w][j].estSelectionne = 0;
            p->plateau[w][j].aSupprimer = 0;
            w--;
        }
    }

    int iterations = 0;
    // Permet items de glisser en diagonale si aucun item au dessus
    while (glisserDiagonale(p) && iterations < 100) {
        for (int j=0; j<COLONNES; j++) {
            int w = LIGNES - 1;
            for (int r = LIGNES - 1; r >= 0; r--) {
                if (p->plateau[r][j].type == MUR) { w = r - 1; }
                else if (p->plateau[r][j].type != VIDE) {
                    p->plateau[w][j].type = p->plateau[r][j].type;
                    p->plateau[w][j].estSelectionne = 0;
                    p->plateau[w][j].aSupprimer = 0;
                    if (w != r) {
                        p->plateau[r][j].type = VIDE;
                        p->plateau[r][j].estSelectionne = 0;
                        p->plateau[r][j].aSupprimer = 0;
                    }
                    w--;
                }
            }
            while (w >= 0) {
                int attempt;
                TypeItem chosen = VIDE;
                for (attempt = 0; attempt < 20; attempt++) {
                    TypeItem cand = ((rand() % 100) < 4) ? JOKER : (TypeItem)((rand() % 5) + 1);
                    if (cand != JOKER && w+2 < LIGNES && p->plateau[w+1][j].type == cand && p->plateau[w+2][j].type == cand) continue;
                    if (cand != JOKER && j >= 2 && p->plateau[w][j-1].type == cand && p->plateau[w][j-2].type == cand) continue;
                    if (cand != JOKER && j+2 < COLONNES && p->plateau[w][j+1].type == cand && p->plateau[w][j+2].type == cand) continue;
                    if (cand != JOKER && j >=1 && j+1 < COLONNES && p->plateau[w][j-1].type == cand && p->plateau[w][j+1].type == cand) continue;
                    chosen = cand; break;
                }
                if (chosen == VIDE) chosen = (TypeItem)((rand() % 5) + 1);
                p->plateau[w][j].type = chosen;
                p->plateau[w][j].estSelectionne = 0;
                p->plateau[w][j].aSupprimer = 0;
                w--;
            }
        }
        iterations++;
    }
}