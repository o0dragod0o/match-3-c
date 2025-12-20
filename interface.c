#include "interface.h"
#include "affichage_console.h"
#include <stdio.h>
#include <time.h>

void afficherMenu() {
    clrscr();
    set_color(LIGHTCYAN, BLACK);
    printf("\n MATCH-3\n");
    set_color(WHITE, BLACK);
    printf("1. Nouvelle partie\n2. Charger une partie\n3. Règles\n4. Quitter\nChoix: ");
}

void afficherRegles() {
    clrscr();
    printf("Règles: Remplacer les symboles pour remplir le contrat. Touche pour continuer...");
    getch();
}

void afficherJeu(Jeu *p) {
    hide_cursor();
    gotoxy(0,0);

    int tempsEcoule = (int)(time(NULL) - p->tempsDebut);
    int tempsRestant = p->dureeMax - tempsEcoule;
    if (tempsRestant < 0) tempsRestant = 0;

    // Status line (fixed width to avoid leftover chars)
    set_color(WHITE, BLACK);
    printf("NIV:%d  VIES:%d  COUPS:%2d  TEMPS:%3ds  SCORE:%d                \n",
           p->niveauActuel, p->vies, p->coupsRestants, tempsRestant, p->score);

    // Contract line
    set_color(LIGHTCYAN, BLACK);
    printf("CONTRAT: ");
    if (p->objectifs[FRAISE]) { set_color(LIGHTRED, BLACK); printf("🍪%d/%d ", p->collecte[FRAISE], p->objectifs[FRAISE]); set_color(LIGHTCYAN, BLACK); }
    if (p->objectifs[OIGNON]) { set_color(YELLOW, BLACK); printf("🍫%d/%d ", p->collecte[OIGNON], p->objectifs[OIGNON]); set_color(LIGHTCYAN, BLACK); }
    if (p->objectifs[MANDARINE]) { set_color(BROWN, BLACK); printf("🍬%d/%d ", p->collecte[MANDARINE], p->objectifs[MANDARINE]); set_color(LIGHTCYAN, BLACK); }
    if (p->objectifs[MYRTILLE]) { set_color(LIGHTBLUE, BLACK); printf("🍭%d/%d ", p->collecte[MYRTILLE], p->objectifs[MYRTILLE]); set_color(LIGHTCYAN, BLACK); }
    if (p->objectifs[CITRON]) { set_color(LIGHTGREEN, BLACK); printf("🍨%d/%d ", p->collecte[CITRON], p->objectifs[CITRON]); set_color(LIGHTCYAN, BLACK); }
    if (p->objectifMurs > 0) { set_color(LIGHTGRAY, BLACK); printf("🚧%d/%d ", p->mursCasses, p->objectifMurs); set_color(LIGHTCYAN, BLACK); }
    // pad the rest of the line
    printf("                              \n");

    set_color(DARKGRAY, BLACK);
    printf("--------------------------------------------------\n");

    // Board (overwrite in place)
    for (int i=0;i<LIGNES;i++) {
        for (int j=0;j<COLONNES;j++) {
            TypeItem t = p->plateau[i][j].type;
            const char *sym = "  ";
            int col = WHITE, bg = BLACK;
            switch(t) {
                case FRAISE: sym = "🍪"; col = LIGHTRED; break;
                case OIGNON: sym = "🍫"; col = YELLOW; break;
                case MANDARINE: sym = "🍬"; col = BROWN; break;
                case MYRTILLE: sym = "🍭"; col = LIGHTBLUE; break;
                case CITRON: sym = "🍨"; col = LIGHTGREEN; break;
                case JOKER: sym = "🪄"; col = LIGHTMAGENTA; break;
                case MUR: sym = "🚧"; col = LIGHTGRAY; break;
                case VIDE: default: sym = "."; col = DARKGRAY; break;
            }
            if (p->plateau[i][j].estSelectionne) bg = MAGENTA;
            if (i == p->curseurY && j == p->curseurX) { bg = LIGHTGRAY; if (col == LIGHTGRAY) col = BLACK; }
            set_color(col, bg);
            printf("%s ", sym);
        }
        set_color(WHITE, BLACK);
        printf("\n");
    }

    int baseRow = 4; // board starts at this row (0-based)

    // Footer (fixed position) - clear then print to avoid duplicates
    int footerRow = baseRow + LIGNES + 1;
    gotoxy(0, footerRow);
    // clear previous content (fixed width)
    set_color(WHITE, BLACK);
    printf("                                                            ");
    gotoxy(0, footerRow);
    printf("[ZQSD]: Bouger | [ESPACE]: Action | [ESC]: Pause               ");
}

