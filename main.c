#include <stdio.h>
#include <stdlib.h>
#include "jeu.h"
#include "affichage.h"
#include "affichage_console.h" // Pour Sleep

int main() {
    int choix = 0;

    do {
        afficherMenu();
        if (scanf("%d", &choix) != 1) {
            while(getchar() != '\n');
            choix = 0;
        }

        if (choix == 1) {
            Jeu partie;
            initialiserJeu(&partie, 1);

            int enJeu = 1;
            clrscr();

            while (enJeu) {
                afficherJeu(&partie);

                // --- GESTION DES COMBOS AUTOMATIQUES ---
                // Si des alignements existent (après une chute), on les traite
                // sans attendre que le joueur joue.
                if (verifierAlignements(&partie)) {
                    Sleep(500); // Pause visuelle pour voir le match
                    supprimerItems(&partie);
                    afficherJeu(&partie); // Affiche les trous

                    Sleep(500); // Pause visuelle avant la chute
                    appliquerGravite(&partie);

                    // La boucle recommence, donc on revérifiera s'il y a de nouveaux matchs
                    continue; // On saute la gestion clavier pour relancer l'affichage
                }

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
                                // Tentative d'échange
                                if (partie.curseurX == partie.selectX && partie.curseurY == partie.selectY) {
                                    partie.plateau[partie.selectY][partie.selectX].estSelectionne = 0;
                                    partie.aSelectionneUneCase = 0;
                                }
                                else {
                                    int diffX = abs(partie.curseurX - partie.selectX);
                                    int diffY = abs(partie.curseurY - partie.selectY);

                                    if ((diffX == 1 && diffY == 0) || (diffX == 0 && diffY == 1)) {
                                        // permuterItems appelle maintenant verifierAlignements en interne
                                        // et annule le coup si ça ne fait pas de match !
                                        int succes = permuterItems(&partie, partie.selectX, partie.selectY,
                                                      partie.curseurX, partie.curseurY);

                                        partie.plateau[partie.selectY][partie.selectX].estSelectionne = 0;
                                        partie.aSelectionneUneCase = 0;

                                        if (!succes) {
                                            // Feedback visuel si le coup est invalide ?
                                        }
                                    }
                                }
                            }
                            break;

                        case 27: enJeu = 0; break;
                    }
                }
                Sleep(20);
            }
        }
    } while (choix != 3);

    show_cursor();
    set_color(WHITE, BLACK);
    clrscr();
    return 0;
}
