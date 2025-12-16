#include "jeu.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>

void initialiserJeu(Jeu *p, int niveau) {
    // NOTE : On NE remet PAS p->score à 0 ici pour le conserver entre les niveaux.
    // L'initialisation du score à 0 se fait au tout début du main().
    
    p->niveauActuel = niveau;
    
    if (niveau == 1) {
        p->vies = 3;
    }

    p->curseurX = COLONNES / 2;
    p->curseurY = LIGNES / 2;
    p->aSelectionneUneCase = 0;

    // --- RESET COMPTEURS ---
    for(int i=0; i<6; i++) {
        p->objectifs[i] = 0;
        p->collecte[i] = 0;
    }

    // --- CONTRAT DYNAMIQUE ---
    int totalItemsRequis = 15 + (niveau * 5);
    int nbTypesCibles = (niveau >= 3) ? 3 : 2;
    if (niveau >= 6) nbTypesCibles = 4;

    int fruitsPossibles[5] = {FRAISE, OIGNON, MANDARINE, MYRTILLE, CITRON};
    for (int i = 0; i < 5; i++) {
        int r = rand() % 5;
        int temp = fruitsPossibles[i];
        fruitsPossibles[i] = fruitsPossibles[r];
        fruitsPossibles[r] = temp;
    }

    int reste = totalItemsRequis;
    for (int i = 0; i < nbTypesCibles; i++) {
        if (i == nbTypesCibles - 1) {
            p->objectifs[fruitsPossibles[i]] = reste;
        } else {
            int min = 5;
            int max = reste - (5 * (nbTypesCibles - 1 - i)); 
            if (max < min) max = min;
            int part = min + (rand() % (max - min + 1));
            p->objectifs[fruitsPossibles[i]] = part;
            reste -= part;
        }
    }

    int margeCoups = 15 - niveau; 
    if (margeCoups < 5) margeCoups = 5; 
    p->coupsRestants = totalItemsRequis + margeCoups;
    p->dureeMax = 30 + (totalItemsRequis * 3);

    p->tempsDebut = time(NULL);
    genererNiveau(p);

    // Nettoyage initial sans points
    while (verifierAlignements(p)) {
        for (int i = 0; i < LIGNES; i++) {
            for (int j = 0; j < COLONNES; j++) {
                if (p->plateau[i][j].aSupprimer) {
                    p->plateau[i][j].type = VIDE;
                    p->plateau[i][j].aSupprimer = 0;
                }
            }
        }
        appliquerGravite(p);
    }
}

void genererNiveau(Jeu *p) {
    for (int i = 0; i < LIGNES; i++) {
        for (int j = 0; j < COLONNES; j++) {
            p->plateau[i][j].type = (rand() % 5) + 1;
            p->plateau[i][j].estSelectionne = 0;
            p->plateau[i][j].aSupprimer = 0;
        }
    }
}

int permuterItems(Jeu *p, int x1, int y1, int x2, int y2) {
    TypeItem temp = p->plateau[y1][x1].type;
    p->plateau[y1][x1].type = p->plateau[y2][x2].type;
    p->plateau[y2][x2].type = temp;

    if (verifierAlignements(p)) {
        p->coupsRestants--; 
        return 1;
    } else {
        temp = p->plateau[y1][x1].type;
        p->plateau[y1][x1].type = p->plateau[y2][x2].type;
        p->plateau[y2][x2].type = temp;
        return 0;
    }
}

int verifierAlignements(Jeu *p) {
    int matchTrouve = 0;

    // Réinitialisation des marqueurs
    for(int y=0; y<LIGNES; y++)
        for(int x=0; x<COLONNES; x++)
            p->plateau[y][x].aSupprimer = 0;

    // ==========================================================
    // 1. DETECTION DES FIGURES SPECIALES
    // ==========================================================

    // --- A. LIGNE DE 6 (Horizontal & Vertical) ---
    // Effet : Disparition de TOUS les items de ce type sur le plateau
    
    // Horizontal
    for (int y = 0; y < LIGNES; y++) {
        for (int x = 0; x <= COLONNES - 6; x++) {
            TypeItem t = p->plateau[y][x].type;
            if (t != VIDE && t != MUR) {
                int k = 1;
                while (x + k < COLONNES && p->plateau[y][x+k].type == t) k++;
                if (k >= 6) { // Suite de 6 ou plus trouvée
                    // Marquer tous les items de ce type sur tout le plateau
                    for(int i=0; i<LIGNES; i++)
                        for(int j=0; j<COLONNES; j++)
                            if(p->plateau[i][j].type == t) p->plateau[i][j].aSupprimer = 1;
                    matchTrouve = 1;
                }
            }
        }
    }
    // Vertical
    for (int x = 0; x < COLONNES; x++) {
        for (int y = 0; y <= LIGNES - 6; y++) {
            TypeItem t = p->plateau[y][x].type;
            if (t != VIDE && t != MUR) {
                int k = 1;
                while (y + k < LIGNES && p->plateau[y+k][x].type == t) k++;
                if (k >= 6) {
                    for(int i=0; i<LIGNES; i++)
                        for(int j=0; j<COLONNES; j++)
                            if(p->plateau[i][j].type == t) p->plateau[i][j].aSupprimer = 1;
                    matchTrouve = 1;
                }
            }
        }
    }

    // --- B. CROIX DE 9 (Centre + 4 dans chaque direction) ---
    // Effet : Disparition des items de ce type sur la ligne ET la colonne du centre
    // Le centre doit être à au moins 2 cases des bords (index 2 à 7 pour taille 10)
    for (int y = 2; y < LIGNES - 2; y++) {
        for (int x = 2; x < COLONNES - 2; x++) {
            TypeItem t = p->plateau[y][x].type;
            if (t != VIDE && t != MUR) {
                if (p->plateau[y][x-1].type == t && p->plateau[y][x-2].type == t &&
                    p->plateau[y][x+1].type == t && p->plateau[y][x+2].type == t &&
                    p->plateau[y-1][x].type == t && p->plateau[y-2][x].type == t &&
                    p->plateau[y+1][x].type == t && p->plateau[y+2][x].type == t) {
                    
                    // Marquer la ligne Y et la colonne X (pour ce type)
                    for(int k=0; k<COLONNES; k++) if(p->plateau[y][k].type == t) p->plateau[y][k].aSupprimer = 1;
                    for(int k=0; k<LIGNES; k++) if(p->plateau[k][x].type == t) p->plateau[k][x].aSupprimer = 1;
                    matchTrouve = 1;
                }
            }
        }
    }

    // --- C. CARRE 4x4 ---
    // Effet : Disparition des items du carré
    for (int y = 0; y <= LIGNES - 4; y++) {
        for (int x = 0; x <= COLONNES - 4; x++) {
            TypeItem t = p->plateau[y][x].type;
            if (t != VIDE && t != MUR) {
                int estCarre = 1;
                for(int dy=0; dy<4; dy++) {
                    for(int dx=0; dx<4; dx++) {
                        if (p->plateau[y+dy][x+dx].type != t) estCarre = 0;
                    }
                }
                if (estCarre) {
                    for(int dy=0; dy<4; dy++) 
                        for(int dx=0; dx<4; dx++) 
                            p->plateau[y+dy][x+dx].aSupprimer = 1;
                    matchTrouve = 1;
                }
            }
        }
    }

    // ==========================================================
    // 2. DETECTION STANDARD (Lignes de 3, 4, 5...)
    // ==========================================================
    // Note : Cela s'ajoute aux suppressions précédentes.
    // Une ligne de 4 est gérée ici (suppression standard).
    
    // Horizontal
    for (int y = 0; y < LIGNES; y++) {
        for (int x = 0; x < COLONNES - 2; x++) {
            TypeItem type = p->plateau[y][x].type;
            if (type != VIDE && type != MUR) {
                if (p->plateau[y][x+1].type == type && p->plateau[y][x+2].type == type) {
                    p->plateau[y][x].aSupprimer = 1;
                    p->plateau[y][x+1].aSupprimer = 1;
                    p->plateau[y][x+2].aSupprimer = 1;
                    matchTrouve = 1;
                }
            }
        }
    }

    // Vertical
    for (int x = 0; x < COLONNES; x++) {
        for (int y = 0; y < LIGNES - 2; y++) {
            TypeItem type = p->plateau[y][x].type;
            if (type != VIDE && type != MUR) {
                if (p->plateau[y+1][x].type == type && p->plateau[y+2][x].type == type) {
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
                TypeItem t = p->plateau[i][j].type;
                if (t >= 1 && t <= 5) {
                    p->collecte[t]++;
                }
                p->plateau[i][j].type = VIDE;
                p->plateau[i][j].aSupprimer = 0;
                p->score += 10;
            }
        }
    }
}

void appliquerGravite(Jeu *p) {
    for (int j = 0; j < COLONNES; j++) {
        int ecriture = LIGNES - 1;
        for (int lecture = LIGNES - 1; lecture >= 0; lecture--) {
            if (p->plateau[lecture][j].type != VIDE && p->plateau[lecture][j].type != MUR) {
                p->plateau[ecriture][j].type = p->plateau[lecture][j].type;
                if (ecriture != lecture) {
                    p->plateau[lecture][j].type = VIDE;
                }
                ecriture--;
            }
            else if (p->plateau[lecture][j].type == MUR) {
                ecriture = lecture - 1;
            }
        }
        while (ecriture >= 0) {
            p->plateau[ecriture][j].type = (rand() % 5) + 1;
            ecriture--;
        }
    }
}

// --- Mise à jour de la sauvegarde pour inclure le SCORE ---
void sauvegarderPartie(Jeu *p) {
    FILE *f = fopen("save.txt", "w");
    if (f) {
        fprintf(f, "%d %d %d", p->niveauActuel, p->vies, p->score);
        fclose(f);
    }
}

int chargerPartie(Jeu *p) {
    FILE *f = fopen("save.txt", "r");
    if (f) {
        // On essaye de lire 3 valeurs, sinon (vieux fichier) on met score à 0
        if (fscanf(f, "%d %d %d", &p->niveauActuel, &p->vies, &p->score) < 3) {
            p->score = 0; 
        }
        fclose(f);
        return 1;
    }
    return 0;
}