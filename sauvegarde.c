#include "sauvegarde.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int sauvegarderPartie(Jeu *p) {
    FILE *f = fopen("save.txt", "w");
    if (!f) return 0;

    fprintf(f, "MATCH3SAVE v2\n");
    fprintf(f, "%d %d %d %d %d %lld %d %d %d %d %d %d %d\n",
            p->niveauActuel,
            p->vies,
            p->score,
            p->coupsRestants,
            p->dureeMax,
            (long long)(p->tempsDebut),
            p->curseurX,
            p->curseurY,
            p->aSelectionneUneCase,
            p->selectionX,
            p->selectionY,
            p->mursCasses,
            p->objectifMurs);
    for (int i = 0; i < 6; i++) {
        if (i) fprintf(f, " ");
        fprintf(f, "%d", p->objectifs[i]);
    }
    fprintf(f, "\n");
    for (int i = 0; i < 6; i++) {
        if (i) fprintf(f, " ");
        fprintf(f, "%d", p->collecte[i]);
    }
    fprintf(f, "\n");
    for (int i = 0; i < LIGNES; i++) {
        for (int j = 0; j < COLONNES; j++) {
            if (j) fprintf(f, " ");
            fprintf(f, "%d", p->plateau[i][j].type);
        }
        fprintf(f, "\n");
    }
    fclose(f);
    return 1;
}

int chargerPartie(Jeu *p) {
    FILE *f = fopen("save.txt", "r");
    if (!f) return 0;
    char line[4096];
    if (!fgets(line, sizeof(line), f)) { fclose(f); return 0; }
    if (strncmp(line, "MATCH3SAVE", 9) == 0) {
        if (!fgets(line, sizeof(line), f)) { fclose(f); return 0; }
        char *tok = strtok(line, " \t\r\n"); if (!tok) { fclose(f); return 0; }
        p->niveauActuel = atoi(tok);
        tok = strtok(NULL, " \t\r\n"); if (!tok) { fclose(f); return 0; }
        p->vies = atoi(tok);
        tok = strtok(NULL, " \t\r\n"); if (!tok) { fclose(f); return 0; }
        p->score = atoi(tok);
        tok = strtok(NULL, " \t\r\n"); if (!tok) { fclose(f); return 0; }
        p->coupsRestants = atoi(tok);
        tok = strtok(NULL, " \t\r\n"); if (!tok) { fclose(f); return 0; }
        p->dureeMax = atoi(tok);
        tok = strtok(NULL, " \t\r\n"); if (!tok) { fclose(f); return 0; }
        long long tempsDebutLL = atoll(tok);
        p->tempsDebut = (time_t)tempsDebutLL;
        tok = strtok(NULL, " \t\r\n"); if (!tok) { fclose(f); return 0; }
        p->curseurX = atoi(tok);
        tok = strtok(NULL, " \t\r\n"); if (!tok) { fclose(f); return 0; }
        p->curseurY = atoi(tok);
        tok = strtok(NULL, " \t\r\n"); if (!tok) { fclose(f); return 0; }
        p->aSelectionneUneCase = atoi(tok);
        tok = strtok(NULL, " \t\r\n"); if (!tok) { fclose(f); return 0; }
        p->selectionX = atoi(tok);
        tok = strtok(NULL, " \t\r\n"); if (!tok) { fclose(f); return 0; }
        p->selectionY = atoi(tok);
        tok = strtok(NULL, " \t\r\n"); if (!tok) { p->mursCasses = 0; p->objectifMurs = 0; }
        else { p->mursCasses = atoi(tok); tok = strtok(NULL, " \t\r\n"); if (!tok) p->objectifMurs = 0; else p->objectifMurs = atoi(tok); }
        if (fgets(line, sizeof(line), f)) {
            sscanf(line, "%d %d %d %d %d %d",
                   &p->objectifs[0], &p->objectifs[1], &p->objectifs[2], &p->objectifs[3], &p->objectifs[4], &p->objectifs[5]);
        } else { for (int i=0;i<6;i++) p->objectifs[i]=0; }
        if (fgets(line, sizeof(line), f)) {
            sscanf(line, "%d %d %d %d %d %d",
                   &p->collecte[0], &p->collecte[1], &p->collecte[2], &p->collecte[3], &p->collecte[4], &p->collecte[5]);
        } else { for (int i=0;i<6;i++) p->collecte[i]=0; }
        for (int i = 0; i < LIGNES; i++) {
            if (!fgets(line, sizeof(line), f)) { for (int j = 0; j < COLONNES; j++) { p->plateau[i][j].type = VIDE; p->plateau[i][j].estSelectionne = 0; p->plateau[i][j].aSupprimer = 0; } continue; }
            char *tok = strtok(line, " \t\r\n");
            for (int j = 0; j < COLONNES; j++) {
                if (tok) { int t = atoi(tok); p->plateau[i][j].type = (TypeItem)t; tok = strtok(NULL, " \t\r\n"); }
                else p->plateau[i][j].type = VIDE;
                p->plateau[i][j].estSelectionne = 0; p->plateau[i][j].aSupprimer = 0;
            }
        }
        fclose(f);
        return 2;
    }
    rewind(f);
    if (fscanf(f, "%d %d %d", &p->niveauActuel, &p->vies, &p->score) < 3) { p->score = 0; fclose(f); return 0; }
    fclose(f);
    return 1;
}