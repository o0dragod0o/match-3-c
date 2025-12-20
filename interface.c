#include "interface.h"
#include "affichage_console.h"
#include <stdio.h>
#include <time.h>

void afficherMenu() {
    // Affiche le menu principal
    clrscr();
    set_color(LIGHTCYAN, BLACK);
    printf("\n MATCH-3\n");
    set_color(WHITE, BLACK);
    printf("1. Nouvelle partie\n2. Charger une partie\n3. Règles\n4. Quitter\nChoix: ");
}

void afficherRegles() {
    clrscr();
    set_color(LIGHTCYAN, BLACK);
    printf("╔═══════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                          🎮 RÈGLES DU JEU 🎮                              ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════════════╝\n\n");
    
    set_color(YELLOW, BLACK);
    printf("🎯 BUT : ");
    set_color(WHITE, BLACK);
    printf("Remplir le contrat en alignant des symboles identiques.\n\n");
    
    set_color(YELLOW, BLACK);
    printf("🕹️  CONTRÔLES :\n");
    set_color(WHITE, BLACK);
    printf("   ZQSD   - Déplacer le curseur\n");
    printf("   ESPACE - Sélectionner/échanger deux symboles adjacents\n");
    printf("   ESC    - Pause/Menu\n\n");
    
    set_color(YELLOW, BLACK);
    printf("🎲 COMMENT JOUER :\n");
    set_color(WHITE, BLACK);
    printf("   1. Déplace le curseur sur un symbole\n");
    printf("   2. Appuie sur ESPACE pour le sélectionner\n");
    printf("   3. Déplace le curseur sur un symbole adjacent (haut/bas/gauche/droite)\n");
    printf("   4. Appuie sur ESPACE pour échanger les deux symboles\n\n");
    
    set_color(YELLOW, BLACK);
    printf("✨ ALIGNEMENTS :\n");
    set_color(WHITE, BLACK);
    printf("   3 alignés       - Suppression normale\n");
    printf("   6 alignés       - Bonus massif (ligne ou colonne)\n");
    printf("   9+ en croix     - Super bonus (ligne + colonne)\n");
    printf("   Carré 4×4       - Efface 16 cases d'un coup\n\n");
    
    set_color(YELLOW, BLACK);
    printf("🌟 ITEMS SPÉCIAUX :\n");
    set_color(LIGHTMAGENTA, BLACK);
    printf("   🪄 Joker  ");
    set_color(WHITE, BLACK);
    printf("- Remplace n'importe quel symbole dans un alignement\n");
    set_color(LIGHTGRAY, BLACK);
    printf("   🚧 Mur    ");
    set_color(WHITE, BLACK);
    printf("- Se casse si tu supprimes un symbole adjacent\n\n");
    
    set_color(YELLOW, BLACK);
    printf("📋 OBJECTIFS : ");
    set_color(WHITE, BLACK);
    printf("Collecte les symboles demandés ");
    set_color(LIGHTRED, BLACK);
    printf("🍪");
    set_color(YELLOW, BLACK);
    printf("🍫");
    set_color(BROWN, BLACK);
    printf("🍬");
    set_color(LIGHTBLUE, BLACK);
    printf("🍭");
    set_color(LIGHTGREEN, BLACK);
    printf("🍨");
    set_color(WHITE, BLACK);
    printf(" et casse les ");
    set_color(LIGHTGRAY, BLACK);
    printf("🚧");
    set_color(WHITE, BLACK);
    printf(" si nécessaire.\n\n");
    
    set_color(LIGHTRED, BLACK);
    printf("❌ DÉFAITE : ");
    set_color(WHITE, BLACK);
    printf("Si tu perds toutes tes vies, si le temps expire, ou si\n");
    printf("            tu n'as plus de coups.\n\n");
    
    set_color(LIGHTGREEN, BLACK);
    printf("✅ VICTOIRE : ");
    set_color(WHITE, BLACK);
    printf("Remplis le contrat avant la fin du temps/coups pour passer\n");
    printf("             au niveau suivant !\n\n");
    
    set_color(DARKGRAY, BLACK);
    printf("═══════════════════════════════════════════════════════════════════════════\n");
    set_color(LIGHTCYAN, BLACK);
    printf("Appuyez sur une touche pour continuer...");
    set_color(WHITE, BLACK);
    getch();
}

void afficherJeu(Jeu *p) {
    hide_cursor();
    gotoxy(0,0);

    // Calcule du temps restant
    int tempsEcoule = (int)(time(NULL) - p->tempsDebut);
    int tempsRestant = p->dureeMax - tempsEcoule;
    if (tempsRestant < 0) tempsRestant = 0;

    // Statut (niveau, vies, coups, temps, score)
    set_color(WHITE, BLACK);
    printf("NIV:%d  VIES:%d  COUPS:%2d  TEMPS:%3ds  SCORE:%d                \n",
           p->niveauActuel, p->vies, p->coupsRestants, tempsRestant, p->score);

    // Affiche le contrat
    set_color(LIGHTCYAN, BLACK);
    printf("CONTRAT: ");
    if (p->objectifs[COOKIE]) {
        set_color(LIGHTRED, BLACK);
        printf("🍪%d/%d ", p->collecte[COOKIE], p->objectifs[COOKIE]);
        set_color(LIGHTCYAN, BLACK);
    }
    if (p->objectifs[CHOCOLAT]) {
        set_color(YELLOW, BLACK);
        printf("🍫%d/%d ", p->collecte[CHOCOLAT], p->objectifs[CHOCOLAT]);
        set_color(LIGHTCYAN, BLACK);
    }
    if (p->objectifs[BONBON]) {
        set_color(BROWN, BLACK);
        printf("🍬%d/%d ", p->collecte[BONBON], p->objectifs[BONBON]);
        set_color(LIGHTCYAN, BLACK);
    }
    if (p->objectifs[SUCETTE]) {
        set_color(LIGHTBLUE, BLACK);
        printf("🍭%d/%d ", p->collecte[SUCETTE], p->objectifs[SUCETTE]);
        set_color(LIGHTCYAN, BLACK);
    }
    if (p->objectifs[GLACE]) {
        set_color(LIGHTGREEN, BLACK);
        printf("🍨%d/%d ", p->collecte[GLACE], p->objectifs[GLACE]);
        set_color(LIGHTCYAN, BLACK);
    }
    if (p->objectifMurs > 0) {
        set_color(LIGHTGRAY, BLACK);
        printf("🚧%d/%d ", p->mursCasses, p->objectifMurs);
        set_color(LIGHTCYAN, BLACK);
    }
    printf("                              \n");

    set_color(DARKGRAY, BLACK);
    printf("--------------------------------------------------\n");
    
    // Affiche le plateau
    for (int i=0;i<LIGNES;i++) {
        for (int j=0;j<COLONNES;j++) {
            TypeItem t = p->plateau[i][j].type;
            const char *sym = "  ";
            int col = WHITE, bg = BLACK;
            switch(t) {
                case COOKIE: sym = "🍪";
                col = LIGHTRED;
                break;
                case CHOCOLAT: sym = "🍫";
                col = YELLOW;
                break;
                case BONBON: sym = "🍬";
                col = BROWN;
                break;
                case SUCETTE: sym = "🍭";
                col = LIGHTBLUE;
                break;
                case GLACE: sym = "🍨";
                col = LIGHTGREEN;
                break;
                case JOKER: sym = "🪄";
                col = LIGHTMAGENTA;
                break;
                case MUR: sym = "🚧";
                col = LIGHTGRAY;
                break;
                case VIDE: default: sym = ".";
                col = DARKGRAY;
                break;
            }
            if (p->plateau[i][j].estSelectionne) bg = MAGENTA;
            if (i == p->curseurY && j == p->curseurX) {
                bg = LIGHTGRAY;
                if (col == LIGHTGRAY) col = BLACK;
            }
            set_color(col, bg);
            printf("%s ", sym);
        }
        set_color(WHITE, BLACK);
        printf("\n");
    }

    int baseRow = 4;

    int footerRow = baseRow + LIGNES + 1;
    gotoxy(0, footerRow);
    // efface contenu précédent
    set_color(WHITE, BLACK);
    printf("                                                            ");
    gotoxy(0, footerRow);
    printf("[ZQSD]: Bouger | [ESPACE]: Action | [ESC]: Pause               ");
}

