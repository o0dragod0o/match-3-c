#ifndef JEU_H
#define JEU_H

#include <time.h> // Nécessaire pour la gestion du temps

// Taille de la grille
#define LIGNES 10
#define COLONNES 20

// Définition des types de fruits
typedef enum {
    VIDE = 0,
    FRAISE = 1,
    OIGNON = 2,
    MANDARINE = 3,
    MYRTILLE = 4,
    CITRON = 5,
    MUR = 99
} TypeItem;

// Structure d'une case
typedef struct {
    TypeItem type;
    int estSelectionne;
    int aSupprimer; // Marqueur pour suppression
} Case;

// Structure principale du Jeu
typedef struct {
    Case plateau[LIGNES][COLONNES];
    int score;
    int vies;
    int coupsRestants;
    int niveauActuel;

    // --- NOUVEAU : Gestion du contrat et du temps ---
    int objectifs[6];    // Quantité à collecter pour chaque fruit (index 1 à 5)
    int collecte[6];     // Quantité déjà collectée
    time_t tempsDebut;   // Moment où le niveau a commencé
    int dureeMax;        // Temps limite en secondes

    // --- Curseurs et sélection ---
    int curseurX, curseurY;
    int aSelectionneUneCase;
    int selectX, selectY;
} Jeu;

// Prototypes des fonctions
void initialiserJeu(Jeu *p, int niveau);
void genererNiveau(Jeu *p);
int permuterItems(Jeu *p, int x1, int y1, int x2, int y2);
int verifierAlignements(Jeu *p);
void supprimerItems(Jeu *p);
void appliquerGravite(Jeu *p);

// Gestion de la sauvegarde
void sauvegarderPartie(Jeu *p);
int chargerPartie(Jeu *p);

#endif
