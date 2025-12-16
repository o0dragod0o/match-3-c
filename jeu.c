#include "jeu.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

void initialiserJeu(Jeu *p, int niveau) {
    (*p).niveauActuel = niveau;
    if (niveau == 1) (*p).vies = 3;

    (*p).curseurX = COLONNES / 2;
    (*p).curseurY = LIGNES / 2;
    (*p).aSelectionneUneCase = 0;

    // Reset objectifs et collecte
    for(int i=0; i<6; i++) {
        (*p).objectifs[i] = 0;
        (*p).collecte[i] = 0;
    }

    // Génération du contrat
    int totalItems = 15 + (niveau * 5);
    int nbTypes = (niveau >= 6) ? 4 : (niveau >= 3 ? 3 : 2);
    int fruits[5] = {FRAISE, OIGNON, MANDARINE, MYRTILLE, CITRON};

    // Mélange aléatoire des types de fruits
    for (int i=0; i<5; i++) {
        int r = rand()%5, t = fruits[i];
        fruits[i] = fruits[r]; fruits[r] = t;
    }

    int reste = totalItems;
    for (int i = 0; i < nbTypes; i++) {
        int part = (i == nbTypes - 1) ? reste : (5 + rand() % (reste - 5 * (nbTypes - 1 - i) - 5 + 1));
        (*p).objectifs[fruits[i]] = part;
        reste -= part;
    }

    (*p).coupsRestants = totalItems + (15 - niveau < 5 ? 5 : 15 - niveau);
    (*p).dureeMax = 30 + (totalItems * 3);
    (*p).tempsDebut = time(NULL);

    genererNiveau(p);

    // Suppression des matchs initiaux sans points
    while (verifierAlignements(p)) {
        for (int i=0; i<LIGNES; i++)
            for (int j=0; j<COLONNES; j++)
                if ((*p).plateau[i][j].aSupprimer) (*p).plateau[i][j].type = VIDE;
        appliquerGravite(p);
    }
}

void genererNiveau(Jeu *p) {
    for (int i=0; i<LIGNES; i++) {
        for (int j=0; j<COLONNES; j++) {
            (*p).plateau[i][j].type = (rand() % 5) + 1;
            (*p).plateau[i][j].estSelectionne = 0;
            (*p).plateau[i][j].aSupprimer = 0;
        }
    }
}

int permuterItems(Jeu *p, int x1, int y1, int x2, int y2) {
    TypeItem tmp = (*p).plateau[y1][x1].type;
    (*p).plateau[y1][x1].type = (*p).plateau[y2][x2].type;
    (*p).plateau[y2][x2].type = tmp;

    if (verifierAlignements(p)) {
        (*p).coupsRestants--;
        return 1;
    }

    // Annulation si pas de match
    tmp = (*p).plateau[y1][x1].type;
    (*p).plateau[y1][x1].type = (*p).plateau[y2][x2].type;
    (*p).plateau[y2][x2].type = tmp;
    return 0;
}

// Helper pour marquer tout un type de fruit
void marquerToutType(Jeu *p, TypeItem t) {
    for(int i=0; i<LIGNES; i++)
        for(int j=0; j<COLONNES; j++)
            if((*p).plateau[i][j].type == t) (*p).plateau[i][j].aSupprimer = 1;
}

int verifierAlignements(Jeu *p) {
    int match = 0;

    // Reset flags suppression
    for(int i=0; i<LIGNES; i++)
        for(int j=0; j<COLONNES; j++) (*p).plateau[i][j].aSupprimer = 0;

    // --- 1. LIGNE DE 6 (Bonus Global) ---
    // Horizontal
    for (int y=0; y<LIGNES; y++) {
        for (int x=0; x<=COLONNES-6; x++) {
            TypeItem t = (*p).plateau[y][x].type;
            if (t<=0 || t==MUR) continue;
            int k=1; while(x+k < COLONNES && (*p).plateau[y][x+k].type == t) k++;
            if (k>=6) { marquerToutType(p, t); match=1; }
        }
    }
    // Vertical
    for (int x=0; x<COLONNES; x++) {
        for (int y=0; y<=LIGNES-6; y++) {
            TypeItem t = (*p).plateau[y][x].type;
            if (t<=0 || t==MUR) continue;
            int k=1; while(y+k < LIGNES && (*p).plateau[y+k][x].type == t) k++;
            if (k>=6) { marquerToutType(p, t); match=1; }
        }
    }

    // --- 2. CROIX DE 9 (Bonus Croix) ---
    for (int y=2; y<LIGNES-2; y++) {
        for (int x=2; x<COLONNES-2; x++) {
            TypeItem t = (*p).plateau[y][x].type;
            if (t<=0 || t==MUR) continue;
            if ((*p).plateau[y][x-1].type==t && (*p).plateau[y][x-2].type==t &&
                (*p).plateau[y][x+1].type==t && (*p).plateau[y][x+2].type==t &&
                (*p).plateau[y-1][x].type==t && (*p).plateau[y-2][x].type==t &&
                (*p).plateau[y+1][x].type==t && (*p).plateau[y+2][x].type==t) {

                for(int k=0; k<COLONNES; k++) if((*p).plateau[y][k].type==t) (*p).plateau[y][k].aSupprimer=1;
                for(int k=0; k<LIGNES; k++) if((*p).plateau[k][x].type==t) (*p).plateau[k][x].aSupprimer=1;
                match = 1;
            }
        }
    }

    // --- 3. CARRE 4x4 (Bonus Zone) ---
    for (int y=0; y<=LIGNES-4; y++) {
        for (int x=0; x<=COLONNES-4; x++) {
            TypeItem t = (*p).plateau[y][x].type;
            if (t<=0 || t==MUR) continue;
            int carre = 1;
            for(int dy=0; dy<4; dy++) for(int dx=0; dx<4; dx++)
                if((*p).plateau[y+dy][x+dx].type != t) carre=0;
            if (carre) {
                for(int dy=0; dy<4; dy++) for(int dx=0; dx<4; dx++) (*p).plateau[y+dy][x+dx].aSupprimer=1;
                match = 1;
            }
        }
    }

    // --- 4. MATCH 3 CLASSIQUE ---
    // Horizontal
    for (int y=0; y<LIGNES; y++) {
        for (int x=0; x<COLONNES-2; x++) {
            TypeItem t = (*p).plateau[y][x].type;
            if (t<=0 || t==MUR) continue;
            if ((*p).plateau[y][x+1].type == t && (*p).plateau[y][x+2].type == t) {
                (*p).plateau[y][x].aSupprimer = 1;
                (*p).plateau[y][x+1].aSupprimer = 1;
                (*p).plateau[y][x+2].aSupprimer = 1;
                match = 1;
            }
        }
    }
    // Vertical
    for (int x=0; x<COLONNES; x++) {
        for (int y=0; y<LIGNES-2; y++) {
            TypeItem t = (*p).plateau[y][x].type;
            if (t<=0 || t==MUR) continue;
            if ((*p).plateau[y+1][x].type == t && (*p).plateau[y+2][x].type == t) {
                (*p).plateau[y][x].aSupprimer = 1;
                (*p).plateau[y+1][x].aSupprimer = 1;
                (*p).plateau[y+2][x].aSupprimer = 1;
                match = 1;
            }
        }
    }
    return match;
}

void supprimerItems(Jeu *p) {
    for (int i=0; i<LIGNES; i++) {
        for (int j=0; j<COLONNES; j++) {
            if ((*p).plateau[i][j].aSupprimer) {
                TypeItem t = (*p).plateau[i][j].type;
                if (t >= 1 && t <= 5) (*p).collecte[t]++;

                (*p).plateau[i][j].type = VIDE;
                (*p).plateau[i][j].aSupprimer = 0;
                (*p).plateau[i][j].estSelectionne = 0;
                (*p).score += 10;
            }
        }
    }
}

void appliquerGravite(Jeu *p) {
    for (int j=0; j<COLONNES; j++) {
        int w = LIGNES - 1; // Curseur écriture
        for (int r = LIGNES - 1; r >= 0; r--) { // Curseur lecture
            if ((*p).plateau[r][j].type == MUR) {
                w = r - 1;
            } else if ((*p).plateau[r][j].type != VIDE) {
                (*p).plateau[w][j].type = (*p).plateau[r][j].type;
                if (w != r) (*p).plateau[r][j].type = VIDE;
                w--;
            }
        }
        // Remplissage
        while (w >= 0) {
            (*p).plateau[w][j].type = (rand() % 5) + 1;
            (*p).plateau[w][j].estSelectionne = 0;
            (*p).plateau[w][j].aSupprimer = 0;
            w--;
        }
    }
}

void sauvegarderPartie(Jeu *p) {
    FILE *f = fopen("save.txt", "w");
    if (f) {
        fprintf(f, "%d %d %d", (*p).niveauActuel, (*p).vies, (*p).score);
        fclose(f);
    }
}

int chargerPartie(Jeu *p) {
    FILE *f = fopen("save.txt", "r");
    if (!f) return 0;
    if (fscanf(f, "%d %d %d", &(*p).niveauActuel, &(*p).vies, &(*p).score) < 3) {
        (*p).score = 0;
    }
    fclose(f);
    return 1;
}
