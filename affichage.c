#include <stdio.h>
#include <time.h>
#include "affichage.h"

void afficherMenu() {
    clrscr();
    set_color(LIGHTCYAN, BLACK);
    printf("======================================\n");
    printf("       ECE HEROES - MATCH 3           \n");
    printf("======================================\n");
    set_color(WHITE, BLACK);
    printf("1. Jouer (Nouvelle Partie)\n");
    printf("2. Charger Partie\n");
    printf("3. Regles\n");
    printf("4. Quitter\n");
    printf("Votre choix : ");
}

void afficherRegles() {
    clrscr();
    set_color(YELLOW, BLACK);
    printf("=== REGLES ===\n\n");
    set_color(WHITE, BLACK);
    printf("1. Alignez 3 fruits pour collecter.\n");
    printf("2. Remplissez le CONTRAT pour gagner.\n");
    printf("3. Defaite si : Temps ecoule OU 0 coup.\n\n");
    printf("Touche pour retour...");
    getch();
}

void afficherJeu(Jeu *p) {
    hide_cursor();
    gotoxy(0, 0);

    int tempsEcoule = (int)(time(NULL) - (*p).tempsDebut);
    int tempsRestant = (*p).dureeMax - tempsEcoule;
    if (tempsRestant < 0) tempsRestant = 0;

    // Alerte couleur
    if (tempsRestant < 10 || (*p).coupsRestants < 5) set_color(LIGHTRED, BLACK);
    else set_color(WHITE, BLACK);

    printf("NIV: %d | VIES: %d | COUPS: %2d | TEMPS: %3ds | SCORE: %d      \n",
           (*p).niveauActuel, (*p).vies, (*p).coupsRestants, tempsRestant, (*p).score);

    // Contrat
    set_color(LIGHTCYAN, BLACK);
    printf("CONTRAT : ");
    if ((*p).objectifs[FRAISE])    printf("F:%d/%d ", (*p).collecte[FRAISE], (*p).objectifs[FRAISE]);
    if ((*p).objectifs[OIGNON])    printf("O:%d/%d ", (*p).collecte[OIGNON], (*p).objectifs[OIGNON]);
    if ((*p).objectifs[MANDARINE]) printf("M:%d/%d ", (*p).collecte[MANDARINE], (*p).objectifs[MANDARINE]);
    if ((*p).objectifs[MYRTILLE])  printf("B:%d/%d ", (*p).collecte[MYRTILLE], (*p).objectifs[MYRTILLE]);
    if ((*p).objectifs[CITRON])    printf("C:%d/%d ", (*p).collecte[CITRON], (*p).objectifs[CITRON]);
    printf("                \n");

    set_color(DARKGRAY, BLACK);
    printf("--------------------------------------------------\n");

    // Grille
    for (int i = 0; i < LIGNES; i++) {
        for (int j = 0; j < COLONNES; j++) {
            int colTxt = WHITE, colBg = BLACK;
            char sym = ' ';
            TypeItem type = (*p).plateau[i][j].type;

            switch(type) {
                case FRAISE:    colTxt = LIGHTRED;   sym = 'F'; break;
                case OIGNON:    colTxt = YELLOW;     sym = 'O'; break;
                case MANDARINE: colTxt = BROWN;      sym = 'M'; break;
                case MYRTILLE:  colTxt = LIGHTBLUE;  sym = 'B'; break;
                case CITRON:    colTxt = LIGHTGREEN; sym = 'C'; break;
                case MUR:       colTxt = LIGHTGRAY;  sym = '#'; break;
                default:        colTxt = DARKGRAY;   sym = '.'; break;
            }

            if ((*p).plateau[i][j].estSelectionne) colBg = MAGENTA;

            // Surbrillance curseur
            if (i == (*p).curseurY && j == (*p).curseurX) {
                colBg = LIGHTGRAY;
                if (colTxt == LIGHTGRAY) colTxt = BLACK;
            }

            set_color(colTxt, colBg);
            printf("%c", sym);
        }
        set_color(WHITE, BLACK);
        printf("\n");
    }
    printf("\n[ZQSD]: Bouger | [ESPACE]: Action | [ESC]: Pause");
    delete_line();
}
