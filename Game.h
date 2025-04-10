#ifndef GAME_H
#define GAME_H

#include "raylib.h"

#define NUMAR_CARTI 12
#define NUMAR_MAXIM_TRAGERE_CARTI 5

typedef struct {
    int afisare_carte;
    char poza_carte[3000];
    Texture2D texture_carti;
    int val_carte;
} Carte;

void InitCarti(Carte *set_carti);
void UnloadCarti(Carte *set_carti);
void DrawCarti(Carte *set_carti, int *ordine, int count, float centerX, float centerY);
int TrageCarte(Carte *set_carti, int counter);

#endif