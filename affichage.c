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
    printf("=== REGLES DU JEU ===\n\n");
    set_color(WHITE, BLACK);
    printf("1. Alignez 3 fruits identiques pour les recolter.\n");
    printf("2. CONTRAT : Vous devez collecter un nombre precis de fruits\n");
    printf("   (ex: 20 Fraises, 50 Oignons) pour gagner le niveau.\n");
    printf("3. CONDITIONS DE DEFAITE (Perte d'une vie) :\n");
    printf("   - Plus de coups disponibles.\n");
    printf("   - Temps ecoule (Chrono a 0).\n");
    printf("4. Victoire : Contrat rempli avant la fin.\n");
    printf("5. Sauvegarde possible a la fin d'un niveau.\n\n");
    printf("Appuyez sur une touche pour revenir...");
    getch();
}

void afficherJeu(Jeu *p) {
    hide_cursor();
    gotoxy(0, 0);

    // Calcul du temps
    int tempsEcoule = (int)(time(NULL) - p->tempsDebut);
    int tempsRestant = p->dureeMax - tempsEcoule;
    if (tempsRestant < 0) tempsRestant = 0;

    // Couleur d'urgence si peu de temps/coups
    if (tempsRestant < 10 || p->coupsRestants < 5) set_color(LIGHTRED, BLACK);
    else set_color(WHITE, BLACK);

    printf("NIV: %d | VIES: %d | COUPS: %2d | TEMPS: %3ds | SCORE: %d      \n",
           p->niveauActuel, p->vies, p->coupsRestants, tempsRestant, p->score);
    
    // --- Affichage du Contrat ---
    set_color(LIGHTCYAN, BLACK);
    printf("CONTRAT : ");
    // Affiche seulement les fruits requis par le niveau
    if (p->objectifs[FRAISE])    printf("F:%d/%d ", p->collecte[FRAISE], p->objectifs[FRAISE]);
    if (p->objectifs[OIGNON])    printf("O:%d/%d ", p->collecte[OIGNON], p->objectifs[OIGNON]);
    if (p->objectifs[MANDARINE]) printf("M:%d/%d ", p->collecte[MANDARINE], p->objectifs[MANDARINE]);
    if (p->objectifs[MYRTILLE])  printf("B:%d/%d ", p->collecte[MYRTILLE], p->objectifs[MYRTILLE]);
    if (p->objectifs[CITRON])    printf("C:%d/%d ", p->collecte[CITRON], p->objectifs[CITRON]);
    
    printf("                \n"); // Nettoyage de fin de ligne
    set_color(DARKGRAY, BLACK);
    printf("--------------------------------------------------\n");

    // Affichage de la grille
    for (int i = 0; i < LIGNES; i++) {
        for (int j = 0; j < COLONNES; j++) {
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

            int couleurFond = BLACK;
            if (p->plateau[i][j].estSelectionne) couleurFond = MAGENTA;
            if (i == p->curseurY && j == p->curseurX) {
                couleurFond = LIGHTGRAY;
                if (couleurTexte == LIGHTGRAY) couleurTexte = BLACK;
            }

            set_color(couleurTexte, couleurFond);
            printf("%c", symbole);
        }
        set_color(WHITE, BLACK);
        printf("\n");
    }

    printf("\n[ZQSD]: Deplacer | [ESPACE]: Permuter | [ESC]: Pause");
    delete_line();
}