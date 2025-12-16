#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "jeu.h"
#include "affichage.h"
#include "affichage_console.h"

int verifierVictoire(Jeu *p) {
    for (int i = 1; i <= 5; i++) {
        if (p->collecte[i] < p->objectifs[i]) {
            return 0; 
        }
    }
    return 1;
}

int main() {
    srand((unsigned)time(NULL));

    int choix = 0;
    Jeu partie;
    partie.vies = 3; 

    do {
        afficherMenu();
        if (scanf("%d", &choix) != 1) {
            while(getchar() != '\n');
            choix = 0;
        }

        if (choix == 3) {
            afficherRegles();
        }
        else if (choix == 1 || choix == 2) {
            
            if (choix == 1) {
                // Nouvelle partie : Score à 0
                partie.score = 0; 
                initialiserJeu(&partie, 1);
                partie.vies = 3;
            } else {
                if (!chargerPartie(&partie)) {
                    printf("\nAucune sauvegarde trouvee ! Appuyez sur une touche.");
                    getch();
                    continue;
                }
                // Sauvegarde du score chargé
                int scoreSauvegarde = partie.score;
                int viesSauvegardees = partie.vies;
                initialiserJeu(&partie, partie.niveauActuel);
                
                // Restauration (car initialiserJeu ne remet plus à 0, mais on est prudent)
                partie.vies = viesSauvegardees;
                partie.score = scoreSauvegarde;
            }

            int jeuEnCours = 1;

            while (jeuEnCours && partie.vies > 0) {
                int niveauTermine = 0; // 0: en cours, 1: gagné, -1: perdu, 2: pause
                clrscr();

                while (niveauTermine == 0) {
                    afficherJeu(&partie);

                    // Temps
                    if ((time(NULL) - partie.tempsDebut) > partie.dureeMax) {
                        niveauTermine = -1;
                        break;
                    }
                    // Victoire
                    if (verifierVictoire(&partie)) {
                        niveauTermine = 1;
                        break;
                    }
                    // Coups
                    if (partie.coupsRestants <= 0) {
                        niveauTermine = -1;
                        break;
                    }

                    // Combos
                    if (verifierAlignements(&partie)) {
                        Sleep(500);
                        supprimerItems(&partie);
                        afficherJeu(&partie);
                        Sleep(500);
                        appliquerGravite(&partie);
                        continue; 
                    }

                    // Input
                    if (kbhit()) {
                        int touche = getch();
                        switch(touche) {
                            case 'z': case 'Z': if (partie.curseurY > 0) partie.curseurY--; break;
                            case 's': case 'S': if (partie.curseurY < LIGNES - 1) partie.curseurY++; break;
                            case 'q': case 'Q': if (partie.curseurX > 0) partie.curseurX--; break;
                            case 'd': case 'D': if (partie.curseurX < COLONNES - 1) partie.curseurX++; break;
                            
                            case ' ':
                                if (!partie.aSelectionneUneCase) {
                                    partie.plateau[partie.curseurY][partie.curseurX].estSelectionne = 1;
                                    partie.selectX = partie.curseurX;
                                    partie.selectY = partie.curseurY;
                                    partie.aSelectionneUneCase = 1;
                                } else {
                                    if (partie.curseurX == partie.selectX && partie.curseurY == partie.selectY) {
                                        partie.plateau[partie.selectY][partie.selectX].estSelectionne = 0;
                                        partie.aSelectionneUneCase = 0;
                                    } else {
                                        int dx = abs(partie.curseurX - partie.selectX);
                                        int dy = abs(partie.curseurY - partie.selectY);
                                        
                                        if ((dx == 1 && dy == 0) || (dx == 0 && dy == 1)) {
                                            int succes = permuterItems(&partie, partie.selectX, partie.selectY, partie.curseurX, partie.curseurY);
                                            partie.plateau[partie.selectY][partie.selectX].estSelectionne = 0;
                                            partie.aSelectionneUneCase = 0;
                                        }
                                    }
                                }
                                break;

                            case 27: // ESC
                                niveauTermine = 2;
                                break;
                        }
                    }
                    Sleep(50);
                }

                // Fin de niveau
                clrscr();
                if (niveauTermine == 1) {
                    set_color(LIGHTGREEN, BLACK);
                    printf("\n\n   VICTOIRE ! CONTRAT REMPLI !\n");
                    printf("   Score actuel : %d\n", partie.score);
                    printf("   Niveau %d termine.\n\n", partie.niveauActuel);
                    printf("   [C] Continuer au niveau suivant\n");
                    printf("   [Q] Sauvegarder et Quitter\n");
                    printf("   Votre choix : ");
                    
                    char r = ' '; 
                    do { r = getch(); } while(r!='c' && r!='C' && r!='q' && r!='Q');
                    
                    if (r == 'q' || r == 'Q') {
                        partie.niveauActuel++; 
                        sauvegarderPartie(&partie);
                        jeuEnCours = 0; 
                    } else {
                        // On garde le score et les vies
                        partie.niveauActuel++;
                        initialiserJeu(&partie, partie.niveauActuel);
                    }

                } else if (niveauTermine == -1) {
                    partie.vies--;
                    set_color(LIGHTRED, BLACK);
                    printf("\n\n   ECHEC ! (Temps ou Coups epuises)\n");
                    printf("   Vies restantes : %d\n", partie.vies);

                    if (partie.vies > 0) {
                        printf("   Reessayer le niveau ? (O/N) : ");
                        char r = ' '; do { r = getch(); } while(r!='o' && r!='O' && r!='n' && r!='N');
                        
                        if (r == 'o' || r == 'O') {
                            // On recommence sans reset score (ou on pourrait remettre le score de début de niveau si on voulait être strict)
                            initialiserJeu(&partie, partie.niveauActuel);
                        } else {
                            printf("   Sauvegarder et Quitter ? (O/N) : ");
                            do { r = getch(); } while(r!='o' && r!='O' && r!='n' && r!='N');
                            if (r == 'o' || r == 'O') sauvegarderPartie(&partie);
                            jeuEnCours = 0; 
                        }
                    } else {
                        printf("   GAME OVER !\n");
                        printf("   Score Final : %d\n", partie.score);
                        printf("   Appuyez sur une touche...");
                        getch();
                        jeuEnCours = 0;
                    }
                } else if (niveauTermine == 2) {
                    printf("\n\n   PAUSE.\n");
                    printf("   Sauvegarder et Quitter ? (O/N) : ");
                    char r = ' '; do { r = getch(); } while(r!='o' && r!='O' && r!='n' && r!='N');
                    if (r == 'o' || r == 'O') {
                        sauvegarderPartie(&partie);
                        jeuEnCours = 0;
                    } else {
                        jeuEnCours = 0;
                    }
                }
            }
        }

    } while (choix != 4);

    show_cursor();
    set_color(WHITE, BLACK);
    clrscr();
    return 0;
}