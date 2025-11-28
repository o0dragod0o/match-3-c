#include <stdio.h>
#include "affichage.h"

void afficherMenu() {
    clrscr(); // Fonction de votre lib

    // Exemple d'utilisation de vos couleurs
    set_color(LIGHTCYAN, BLACK);
    printf("======================================\n");
    printf("       ECE HEROES - MATCH 3           \n");
    printf("======================================\n");

    set_color(WHITE, BLACK);
    printf("1. Jouer\n");
    printf("2. Regles\n");
    printf("3. Quitter\n");
    printf("Votre choix : ");
}

void afficherJeu(Jeu *p) {
    // On cache le curseur pour éviter le clignotement
    hide_cursor();
    gotoxy(0, 0); // On repart de 0,0 sans effacer tout l'écran (plus rapide)

    set_color(WHITE, BLACK);
    printf("NIVEAU: %d | VIES: %d | COUPS: %d | SCORE: %d      \n",
           p->niveauActuel, p->vies, p->coupsRestants, p->score);
    printf("--------------------------------------------------\n");

    for (int i = 0; i < LIGNES; i++) {
        for (int j = 0; j < COLONNES; j++) {

            // 1. Déterminer la couleur de l'item
            int couleurTexte = WHITE;
            char symbole = ' ';

            switch(p->plateau[i][j].type) {
                case FRAISE:    couleurTexte = LIGHTRED;     symbole = 'F'; break;
                case OIGNON:    couleurTexte = YELLOW;       symbole = 'O'; break;
                case MANDARINE: couleurTexte = BROWN;        symbole = 'M'; break;
                case MYRTILLE:  couleurTexte = LIGHTBLUE;    symbole = 'B'; break;
                case CITRON:    couleurTexte = LIGHTGREEN;   symbole = 'C'; break;
                case MUR:       couleurTexte = LIGHTGRAY;    symbole = '#'; break;
                default:        couleurTexte = DARKGRAY;     symbole = '.'; break;
            }

            // 2. Déterminer la couleur de fond (Curseur ou Sélection)
            int couleurFond = BLACK;

            // Si la case a été verrouillée par une première sélection
            if (p->plateau[i][j].estSelectionne) {
                couleurFond = MAGENTA;
            }
            // Si le curseur du joueur est sur cette case (prioritaire visuellement)
            if (i == p->curseurY && j == p->curseurX) {
                couleurFond = LIGHTGRAY;
                if (couleurTexte == LIGHTGRAY) couleurTexte = BLACK; // Contraste
            }

            // 3. Affichage avec votre bibliothèque
            set_color(couleurTexte, couleurFond);
            printf("%c", symbole);
        }
        // Retour à la ligne : reset des couleurs impératif
        set_color(WHITE, BLACK);
        printf("\n");
    }

    printf("\n[ZQSD]: Deplacer | [ESPACE]: Selectionner/Permuter | [ESC]: Quitter");

    // Nettoyer le reste de l'écran si nécessaire
    delete_line();
}
