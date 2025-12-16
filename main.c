#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "jeu.h"
#include "affichage.h"
#include "affichage_console.h"

#define RES_VICTOIRE 1
#define RES_DEFAITE -1
#define RES_PAUSE 2

int verifierVictoire(Jeu *p) {
    for (int i = 1; i <= 5; i++)
        if ((*p).collecte[i] < (*p).objectifs[i]) return 0;
    return 1;
}

// Fonction isolée pour gérer une partie
int jouerNiveau(Jeu *partie) {
    int niveauTermine = 0;
    clrscr();

    while (niveauTermine == 0) {
        afficherJeu(partie);

        // Temps & Coups
        if ((time(NULL) - (*partie).tempsDebut) > (*partie).dureeMax) return RES_DEFAITE;
        if ((*partie).coupsRestants <= 0) return RES_DEFAITE;
        if (verifierVictoire(partie)) return RES_VICTOIRE;

        // Combos
        if (verifierAlignements(partie)) {
            Sleep(500);
            supprimerItems(partie);
            afficherJeu(partie);
            Sleep(500);
            appliquerGravite(partie);
            continue;
        }

        // Input
        if (kbhit()) {
            int t = getch();
            switch(t) {
                case 'z': case 'Z': if ((*partie).curseurY > 0) (*partie).curseurY--; break;
                case 's': case 'S': if ((*partie).curseurY < LIGNES - 1) (*partie).curseurY++; break;
                case 'q': case 'Q': if ((*partie).curseurX > 0) (*partie).curseurX--; break;
                case 'd': case 'D': if ((*partie).curseurX < COLONNES - 1) (*partie).curseurX++; break;
                case 27: return RES_PAUSE; // ESC

                case ' ':
                    if (!(*partie).aSelectionneUneCase) {
                        (*partie).plateau[(*partie).curseurY][(*partie).curseurX].estSelectionne = 1;
                        (*partie).selectX = (*partie).curseurX;
                        (*partie).selectY = (*partie).curseurY;
                        (*partie).aSelectionneUneCase = 1;
                    } else {
                        // Desélection
                        if ((*partie).curseurX == (*partie).selectX && (*partie).curseurY == (*partie).selectY) {
                            (*partie).plateau[(*partie).selectY][(*partie).selectX].estSelectionne = 0;
                            (*partie).aSelectionneUneCase = 0;
                        }
                        // Permutation
                        else if (abs((*partie).curseurX - (*partie).selectX) + abs((*partie).curseurY - (*partie).selectY) == 1) {
                            permuterItems(partie, (*partie).selectX, (*partie).selectY, (*partie).curseurX, (*partie).curseurY);
                            (*partie).plateau[(*partie).selectY][(*partie).selectX].estSelectionne = 0;
                            (*partie).aSelectionneUneCase = 0;
                        }
                    }
                    break;
            }
        }
        Sleep(50);
    }
    return niveauTermine;
}

int main() {
    srand((unsigned)time(NULL));
    Jeu partie;
    int choix = 0;

    do {
        afficherMenu();
        if (scanf("%d", &choix) != 1) { while(getchar() != '\n'); choix = 0; }

        if (choix == 3) afficherRegles();
        else if (choix == 4) break;
        else if (choix == 1 || choix == 2) {

            if (choix == 1) {
                partie.score = 0;
                initialiserJeu(&partie, 1);
            } else {
                if (!chargerPartie(&partie)) {
                    printf("\nAucune sauvegarde !"); getch(); continue;
                }
                // Sauvegarde temporaire des stats globales
                int s = partie.score, v = partie.vies;
                initialiserJeu(&partie, partie.niveauActuel);
                partie.score = s; partie.vies = v;
            }

            int jeuEnCours = 1;
            while (jeuEnCours && partie.vies > 0) {
                int res = jouerNiveau(&partie);
                clrscr();

                if (res == RES_VICTOIRE) {
                    set_color(LIGHTGREEN, BLACK);
                    printf("\n VICTOIRE ! Niveau %d fini. Score: %d\n", partie.niveauActuel, partie.score);
                    printf(" [C] Continuer | [Q] Quitter (Sauver)\n Choix: ");
                    char r; do r=getch(); while(r!='c' && r!='C' && r!='q' && r!='Q');

                    partie.niveauActuel++;
                    if (r=='q' || r=='Q') { sauvegarderPartie(&partie); jeuEnCours=0; }
                    else initialiserJeu(&partie, partie.niveauActuel);

                } else if (res == RES_DEFAITE) {
                    partie.vies--;
                    set_color(LIGHTRED, BLACK);
                    printf("\n ECHEC. Vies restantes: %d\n", partie.vies);
                    if (partie.vies > 0) {
                        printf(" [R] Reessayer | [Q] Quitter (Sauver)\n Choix: ");
                        char r; do r=getch(); while(r!='r' && r!='R' && r!='q' && r!='Q');
                        if (r=='q' || r=='Q') { sauvegarderPartie(&partie); jeuEnCours=0; }
                        else initialiserJeu(&partie, partie.niveauActuel);
                    } else {
                        printf(" GAME OVER. Score: %d. Touche pour menu...", partie.score);
                        getch();
                        jeuEnCours = 0;
                    }

                } else if (res == RES_PAUSE) {
                    printf("\n PAUSE. Sauver et Quitter ? (O/N) ");
                    char r; do r=getch(); while(r!='o' && r!='O' && r!='n' && r!='N');
                    if (r=='o' || r=='O') { sauvegarderPartie(&partie); jeuEnCours=0; }
                }
            }
        }
    } while (choix != 4);

    set_color(WHITE, BLACK); clrscr(); show_cursor();
    return 0;
}
