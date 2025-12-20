#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "noyau.h"
#include "interface.h"
#include "affichage_console.h"
#include "physique.h"
#include "effets.h"
#include "sauvegarde.h"
#include "generation_niveau.h"

#define RES_VICTOIRE 1
#define RES_DEFAITE -1
#define RES_PAUSE 2

int jouerNiveau(Jeu *partie) {
    int niveauTermine = 0;
    clrscr();

    while (niveauTermine == 0) {
        afficherJeu(partie);

        // Temps et coups
        if ((time(NULL) - partie->tempsDebut) > partie->dureeMax) return RES_DEFAITE;
        if (partie->coupsRestants <= 0) return RES_DEFAITE;
        if (verifierVictoire(partie)) return RES_VICTOIRE;

        if (verifierAlignements(partie)) {
            int iterations = 0;
            int changed;
            do {
                Sleep(250);
                TypeItem snapshot[LIGNES][COLONNES];
                for (int si = 0; si < LIGNES; si++){
                    for (int sj = 0; sj < COLONNES; sj++) snapshot[si][sj] = partie->plateau[si][sj].type;
                }

                supprimerItems(partie);
                afficherJeu(partie);
                Sleep(250);
                appliquerGravite(partie);

                changed = 0;
                for (int si = 0; si < LIGNES && !changed; si++){
                    for (int sj = 0; sj < COLONNES; sj++){
                        if (snapshot[si][sj] != partie->plateau[si][sj].type) {
                            changed = 1;
                            break;
                        }
                    }
                }

                iterations++;
                if (iterations > 200) break;
            } while (verifierAlignements(partie) && changed);//Gérer match en cascade

            if (!changed && verifierAlignements(partie)) {
                for (int i=0;i<LIGNES;i++){
                    for (int j=0;j<COLONNES;j++) partie->plateau[i][j].aSupprimer = 0;
                }
            }

            continue;
        }

        if (kbhit()) {
            //gere les inputs utilisateur
            int t = getch();
            switch(t) {
                case 'z': case 'Z': if (partie->curseurY > 0){
                    partie->curseurY--;
                    break;
                }
                case 's': case 'S': if (partie->curseurY < LIGNES - 1) {
                    partie->curseurY++;
                    break;
                }
                case 'q': case 'Q': if (partie->curseurX > 0) {
                    partie->curseurX--;
                    break;
                }
                case 'd': case 'D': if (partie->curseurX < COLONNES - 1) {
                    partie->curseurX++;
                    break;
                }
                case 27: return RES_PAUSE;

                case ' ':
                    if (!partie->aSelectionneUneCase) {
                        if (partie->plateau[partie->curseurY][partie->curseurX].type != MUR) {
                            partie->plateau[partie->curseurY][partie->curseurX].estSelectionne = 1;
                            partie->selectionX = partie->curseurX;
                            partie->selectionY = partie->curseurY;
                            partie->aSelectionneUneCase = 1;
                        }
                    } else {
                        if (partie->curseurX == partie->selectionX && partie->curseurY == partie->selectionY) {
                            partie->plateau[partie->selectionY][partie->selectionX].estSelectionne = 0;
                            partie->aSelectionneUneCase = 0;
                        }
                        else if (abs(partie->curseurX - partie->selectionX) + abs(partie->curseurY - partie->selectionY) == 1) {
                            if (partie->plateau[partie->curseurY][partie->curseurX].type != MUR && partie->plateau[partie->selectionY][partie->selectionX].type != MUR) {
                                permuterItems(partie, partie->selectionX, partie->selectionY, partie->curseurX, partie->curseurY);
                            }
                            partie->plateau[partie->selectionY][partie->selectionX].estSelectionne = 0;
                            partie->aSelectionneUneCase = 0;
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
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    srand((unsigned)time(NULL));
    Jeu partie;
    int choix = 0;

    do {
        afficherMenu();
        if (scanf("%d", &choix) != 1) {
            while(getchar() != '\n');
            choix = 0;
        }

        if (choix == 3) afficherRegles();
        else if (choix == 4) break;
        else if (choix == 1 || choix == 2) {

            if (choix == 1) {
                partie.score = 0;
                initialiserJeu(&partie, 1);
            } else {
                int loadRes = chargerPartie(&partie);
                if (!loadRes) { printf("\nAucune sauvegarde !");
                    getch();
                    continue;
                }
                
                // Vérifier si la save est expirée (pas de coups ou temps écoulé)
                if (partie.coupsRestants <= 0 || (time(NULL) - partie.tempsDebut) > partie.dureeMax) {
                    remove("save.txt");
                    printf("\nSauvegarde expirée (temps ou coups écoulés). Supprimée.\n");
                    getch();
                    continue;
                }
                
                remove("save.txt");
                if (loadRes == 1) {
                    int s = partie.score, v = partie.vies;
                    initialiserJeu(&partie, partie.niveauActuel);
                    partie.score = s; partie.vies = v;
                }
            }

            int jeuEnCours = 1;
            while (jeuEnCours && partie.vies > 0) {
                int res = jouerNiveau(&partie);
                clrscr();

                //victoire
                if (res == RES_VICTOIRE) {
                    //si niveau final terminé
                    if (partie.niveauActuel >= 5) {
                        set_color(LIGHTGREEN, BLACK);
                        printf("\n FÉLICITATIONS ! Vous avez terminé tous les niveaux. Score final: %d\n", partie.score);
                        printf("Touche pour retour au menu...");
                        Sleep(2000);
                        while (kbhit()) getch();
                        getch();
                        jeuEnCours = 0;
                    } else {
                        //passer au niveau suivant
                        set_color(LIGHTGREEN, BLACK);
                        printf("\n VICTOIRE ! Niveau %d fini. Score: %d\n", partie.niveauActuel, partie.score);
                        Sleep(2000);
                        while (kbhit()) getch();
                        printf(" [C] Continuer | [Q] Quitter (Sauvegarder)\n Choix: ");
                        char r;
                        do{
                            r=getch();
                        }while(r!='c' && r!='C' && r!='q' && r!='Q');

                        partie.niveauActuel++;
                        if (r=='q' || r=='Q') {
                            sauvegarderPartie(&partie);
                            jeuEnCours=0;
                        }
                        else initialiserJeu(&partie, partie.niveauActuel);
                    }

                //defaite
                } else if (res == RES_DEFAITE) {
                    partie.vies--;
                    set_color(LIGHTRED, BLACK);
                    printf("\n ECHEC. Vies restantes: %d\n", partie.vies);
                    //enleve une vie
                    if (partie.vies > 0) {
                        printf(" [R] Reessayer | [Q] Quitter (Sauvegarder)\n Choix: ");
                        char r;
                        do{
                            r=getch();
                        }while(r!='r' && r!='R' && r!='q' && r!='Q');
                        if (r=='q' || r=='Q') {
                            sauvegarderPartie(&partie);
                            jeuEnCours=0;
                        }
                        else initialiserJeu(&partie, partie.niveauActuel);
                    //plus de vie -> game over
                    } else {
                        printf(" GAME OVER. Score: %d. Touche pour menu...", partie.score);
                        getch();
                        jeuEnCours = 0;
                    }
                //pause
                } else if (res == RES_PAUSE) {
                    printf("\n PAUSE. Sauver et Quitter ? (O/N) ");
                    char r;
                    do{
                        r=getch();
                    }while(r!='o' && r!='O' && r!='n' && r!='N');
                    if (r=='o' || r=='O') {
                        sauvegarderPartie(&partie);
                        jeuEnCours=0;
                    }
                }
            }
        }
    } while (choix != 4);

    set_color(WHITE, BLACK);
    clrscr();
    show_cursor();
    return 0;
}
