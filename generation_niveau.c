#include "generation_niveau.h"
#include "effets.h"
#include "physique.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

int calculerTotalItems(int niveau) {
    // difficulté logarithmique pour le total d'items requis
    double val = 30.0 - 10.0 * log((double)niveau + 1.0);
    int totalItems = (int)(val + 0.5);
    if (totalItems < 9) totalItems = 9;  // Minimum de 9 items
    return totalItems;
}

void genererNiveau(Jeu *p) {
    // Remplit grilles items aléatoires
    for (int i=0; i<LIGNES; i++) {
        for (int j=0; j<COLONNES; j++) {
            // 5% chance d'avoir un JOKER, sinon bonbon aléatoire
            if ((rand() % 100) < 5) p->plateau[i][j].type = JOKER;
            else p->plateau[i][j].type = (rand() % 5) + 1;
            p->plateau[i][j].estSelectionne = 0;
            p->plateau[i][j].aSupprimer = 0;
        }
    }
}

void placerMurs(Jeu *p) {
    int niveau = p->niveauActuel;
    if (niveau < 2) return;

    int count = 5 + (rand() % niveau);
    if (count > 10) count = 10;

    int placed = 0, attempts = 0;
    while (placed < count && attempts < 1000) {
        int i = rand() % LIGNES;
        int j = rand() % COLONNES;
        if (p->plateau[i][j].type != MUR) {
            p->plateau[i][j].type = MUR;
            p->plateau[i][j].estSelectionne = 0;
            p->plateau[i][j].aSupprimer = 0;
            placed++;
        }
        attempts++;
    }
}

void initialiserJeu(Jeu *p, int niveau) {
    p->niveauActuel = niveau;
    if (niveau == 1) p->vies = 3;

    p->curseurX = COLONNES / 2;
    p->curseurY = LIGNES / 2;
    p->aSelectionneUneCase = 0;

    // Reset objectifs
    for(int i=0; i<6; i++) {
        p->objectifs[i] = 0;
        p->collecte[i] = 0;
    }
    p->objectifMurs = 0;
    p->mursCasses = 0;

    int totalItems = calculerTotalItems(niveau);
    int nbTypes = (niveau >= 6) ? 4 : (niveau >= 3 ? 3 : 2);
    int fruits[5] = {COOKIE, CHOCOLAT, BONBON, SUCETTE, GLACE};

    // Mélange aléatoire des types de fruits
    for (int i=0; i<5; i++) {
        int r = rand()%5, t = fruits[i];
        fruits[i] = fruits[r]; fruits[r] = t;
    }

    int minPer = 6 + (niveau / 2); // augmente minimum contart
    if (minPer < 5) minPer = 5;
    int reste = totalItems;
    for (int i = 0; i < nbTypes; i++) {
        int part;
        if (i == nbTypes - 1) {
            part = reste;
        } else {
            int minPossible = minPer;
            int maxPossible = reste - minPer * (nbTypes - 1 - i);
            if (maxPossible < minPossible) maxPossible = minPossible;
            part = minPossible + (rand() % (maxPossible - minPossible + 1));
        }
        p->objectifs[fruits[i]] = part;
        reste -= part;
    }

    // Contrat murs plus fréquent et plus exigeant
    if (niveau >= 2 && (rand()%3) != 0) {
        p->objectifMurs = 3 + (niveau / 2);
        if (p->objectifMurs > 10) p->objectifMurs = 10;
        p->mursCasses = 0;
    }

    // Moins de coups et de temps pour plus de difficulté
    int baseMoves = 12 - niveau;
    if (baseMoves < 4) baseMoves = 4;
    p->coupsRestants = totalItems + baseMoves;
    p->dureeMax = 20 + (totalItems * 2);
    p->tempsDebut = time(NULL);

    genererNiveau(p);

    // Suppression des matchs initiaux sans points
    while (verifierAlignements(p)) {
        for (int i=0; i<LIGNES; i++){
            for (int j=0; j<COLONNES; j++){
                if (p->plateau[i][j].aSupprimer) p->plateau[i][j].type = VIDE;
            }
        }
        appliquerGravite(p);
    }

    placerMurs(p);
}