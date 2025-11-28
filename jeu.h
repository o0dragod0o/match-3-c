#ifndef JEU_H
#define JEU_H

// --- MODIFICATION : Taille réduite pour tester facilement ---
#define LIGNES 10
#define COLONNES 10

typedef enum {
    VIDE = 0,
    FRAISE = 1,
    OIGNON = 2,
    MANDARINE = 3,
    MYRTILLE = 4,
    CITRON = 5,
    MUR = 99
} TypeItem;

typedef struct {
    TypeItem type;
    int estSelectionne;
    int aSupprimer; // --- NOUVEAU : Marqueur pour savoir ce qu'il faut effacer
} Case;

typedef struct {
    Case plateau[LIGNES][COLONNES];
    int score;
    int vies;
    int coupsRestants;
    int niveauActuel;
    int curseurX, curseurY;
    int aSelectionneUneCase;
    int selectX, selectY;
} Jeu;

void initialiserJeu(Jeu *p, int niveau);
void genererNiveau(Jeu *p);
int permuterItems(Jeu *p, int x1, int y1, int x2, int y2);

// --- NOUVEAUX PROTOTYPES ---
int verifierAlignements(Jeu *p); // Détecte et marque
void supprimerItems(Jeu *p);     // Transforme les marqués en VIDE
void appliquerGravite(Jeu *p);   // Fait tomber les items

#endif
