#include "effets.h"
#include <stdlib.h>

void marquerToutType(Jeu *p, TypeItem t) {
    // Marque toutes les cases contenant le type 't' pour suppression (effet wildcard JOKER)
    for(int i=0; i<LIGNES; i++){
        for(int j=0; j<COLONNES; j++){
            if(p->plateau[i][j].type == t) p->plateau[i][j].aSupprimer = 1;
        }
    }
}

int verifierAlignements(Jeu *p) {
    int match = 0;
    for(int i=0; i<LIGNES; i++)
        for(int j=0; j<COLONNES; j++) p->plateau[i][j].aSupprimer = 0;  // Réinitialise les marques

    // Ligne de 6 horizontal
    for (int y=0; y<LIGNES; y++) {
        for (int x=0; x<COLONNES; x++) {
            int k = 0; TypeItem base = VIDE;
            for (int nx = x; nx < COLONNES; nx++) {
                TypeItem cur = p->plateau[y][nx].type;
                if (cur == MUR || cur == VIDE) break;
                if (cur == JOKER) {
                    k++;
                    continue;
                }
                if (base == VIDE) {
                    base = cur;
                    k++;
                    continue;
                }
                if (cur == base) {
                    k++;
                    continue;
                }
                break;
            }
            if (base != VIDE && k >= 6) {
                for (int nx = x; nx < x + k; nx++) p->plateau[y][nx].aSupprimer = 1;
                marquerToutType(p, base);
                match = 1;
                x += k - 1;
            }
        }
    }
    // Verticale de 6
    for (int x=0; x<COLONNES; x++) {
        for (int y=0; y<LIGNES; y++) {
            int k = 0; TypeItem base = VIDE;
            for (int ny = y; ny < LIGNES; ny++) {
                TypeItem cur = p->plateau[ny][x].type;
                if (cur == MUR || cur == VIDE) break;
                if (cur == JOKER) {
                    k++;
                    continue;
                }
                if (base == VIDE) {
                    base = cur;
                    k++;
                    continue;
                }
                if (cur == base) {
                    k++;
                    continue;
                }
                break;
            }
            if (base != VIDE && k >= 6) {
                for (int ny = y; ny < y + k; ny++) p->plateau[ny][x].aSupprimer = 1;
                marquerToutType(p, base);
                match = 1;
                y += k - 1;
            }
        }
    }

    // Croix de 9
    for (int y=2; y<LIGNES-2; y++) {
        for (int x=2; x<COLONNES-2; x++) {
            // determine base type
            TypeItem base = VIDE;
            TypeItem checks[] = { p->plateau[y][x].type, p->plateau[y][x-1].type, p->plateau[y][x-2].type, p->plateau[y][x+1].type, p->plateau[y][x+2].type, p->plateau[y-1][x].type, p->plateau[y-2][x].type, p->plateau[y+1][x].type, p->plateau[y+2][x].type };
            int ok = 1;
            for (int c=0;c<9;c++) {
                if (checks[c] == MUR || checks[c] == VIDE) { ok = 0; break; }
                if (checks[c] != JOKER && base == VIDE) base = checks[c];
            }
            if (!ok || base == VIDE) continue;
            for (int c=0;c<9;c++){
                if (checks[c] != JOKER && checks[c] != base) ok = 0;
            }
            if (!ok) continue;

            // marque la ligne et la colonne
            for(int k=0; k<COLONNES; k++) {
                if(p->plateau[y][k].type == base || p->plateau[y][k].type == JOKER) p->plateau[y][k].aSupprimer=1;
            }
            for(int k=0; k<LIGNES; k++) {
                if(p->plateau[k][x].type == base || p->plateau[k][x].type == JOKER) p->plateau[k][x].aSupprimer=1;
            }
            match = 1;
        }
    }

    // Carre 4x4
    for (int y=0; y<=LIGNES-4; y++) {
        for (int x=0; x<=COLONNES-4; x++) {
            TypeItem base = VIDE; int ok = 1;
            for(int dy=0; dy<4; dy++) for(int dx=0; dx<4; dx++) {
                TypeItem cur = p->plateau[y+dy][x+dx].type;
                if (cur == MUR || cur == VIDE) ok = 0;
                if (cur != JOKER && base == VIDE) base = cur;
            }
            if (!ok || base == VIDE) continue;
            for(int dy=0; dy<4; dy++) for(int dx=0; dx<4; dx++) {
                TypeItem cur = p->plateau[y+dy][x+dx].type;
                if (cur != JOKER && cur != base) ok = 0;
            }
            if (!ok) continue;
            for(int dy=0; dy<4; dy++) for(int dx=0; dx<4; dx++) p->plateau[y+dy][x+dx].aSupprimer=1;
            match = 1;
        }
    }

    // Match 3 classique
    for (int y=0; y<LIGNES; y++) {
        for (int x=0; x<COLONNES-2; x++) {
            // Match 3 horizontal
            TypeItem base = VIDE; int ok = 1;
            for (int k=0;k<3;k++) {
                TypeItem cur = p->plateau[y][x+k].type;
                if (cur == MUR || cur == VIDE) { ok = 0; break; }
                if (cur != JOKER && base == VIDE) base = cur;
            }
            if (!ok || base == VIDE) continue;
            for (int k=0;k<3;k++) {
                TypeItem cur = p->plateau[y][x+k].type;
                if (cur != JOKER && cur != base) { ok = 0; break; }
            }
            if (ok) { p->plateau[y][x].aSupprimer = 1; p->plateau[y][x+1].aSupprimer = 1; p->plateau[y][x+2].aSupprimer = 1; match = 1; }
        }
    }
    for (int x=0; x<COLONNES; x++) {
        for (int y=0; y<LIGNES-2; y++) {
            // Match 3 vertical
            TypeItem base = VIDE; int ok = 1;
            for (int k=0;k<3;k++) {
                TypeItem cur = p->plateau[y+k][x].type;
                if (cur == MUR || cur == VIDE) { ok = 0; break; }
                if (cur != JOKER && base == VIDE) base = cur;
            }
            if (!ok || base == VIDE) continue;
            for (int k=0;k<3;k++) {
                TypeItem cur = p->plateau[y+k][x].type;
                if (cur != JOKER && cur != base) { ok = 0; break; }
            }
            if (ok) { p->plateau[y][x].aSupprimer = 1; p->plateau[y+1][x].aSupprimer = 1; p->plateau[y+2][x].aSupprimer = 1; match = 1; }
        }
    }
    return match;
}

int verifierVictoire(Jeu *p) {
    // Vérifie si tous les objectifs sont atteints
        for (int i = COOKIE; i <= GLACE; i++)
        if (p->collecte[i] < p->objectifs[i]) return 0;  // Manque des bonbons
    if (p->objectifMurs > 0 && p->mursCasses < p->objectifMurs) return 0;  // Manque des murs cassés
    return 1;  //objectifs atteints
}

int permuterItems(Jeu *p, int x1, int y1, int x2, int y2) {
    if (p->plateau[y1][x1].type == MUR || p->plateau[y2][x2].type == MUR) return 0;  // Pas de swap avec murs

    // Échange temporaire
    TypeItem tmp = p->plateau[y1][x1].type;
    p->plateau[y1][x1].type = p->plateau[y2][x2].type;
    p->plateau[y2][x2].type = tmp;

    // Teste si cet échange crée un match
    if (verifierAlignements(p)) {
        p->coupsRestants--;  // Consomme un coup
        return 1;
    }

    // Pas de match : annule l'échange
    tmp = p->plateau[y1][x1].type;
    p->plateau[y1][x1].type = p->plateau[y2][x2].type;
    p->plateau[y2][x2].type = tmp;
    return 0;
}

void supprimerItems(Jeu *p) {
    for (int i=0; i<LIGNES; i++) {
        for (int j=0; j<COLONNES; j++) {
            if (p->plateau[i][j].aSupprimer) {
                TypeItem t = p->plateau[i][j].type;
                // Incrémente le compteur de collecte pour ce type
                if (t >= COOKIE && t <= GLACE) p->collecte[t]++;

                p->plateau[i][j].type = VIDE;
                p->plateau[i][j].aSupprimer = 0;
                p->plateau[i][j].estSelectionne = 0;
                p->score += 10;  // Gagne 10 points par item supprimé

                // Casse les murs adjacents
                const int di[4] = {-1,1,0,0};
                const int dj[4] = {0,0,-1,1};
                for (int k=0;k<4;k++) {
                    int ni = i + di[k], nj = j + dj[k];
                    if (ni>=0 && ni<LIGNES && nj>=0 && nj<COLONNES) {
                        if (p->plateau[ni][nj].type == MUR) {
                            p->plateau[ni][nj].type = VIDE;
                            p->plateau[ni][nj].aSupprimer = 0;
                            p->plateau[ni][nj].estSelectionne = 0;
                            p->mursCasses++;
                        }
                    }
                }
            }
        }
    }
}