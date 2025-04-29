#ifndef GAME_H
#define GAME_H

#include "raylib.h"

#define NUMAR_CARTI 12
#define NUMAR_MAXIM_TRAGERE_CARTI 7

typedef struct {
    int afisare_carte;
    char poza_carte[3000];
    Texture2D texture_carti;
    int val_carte;
} Carte;

extern Texture2D texture_back;

void InitCarti(Carte *set_carti);
void UnloadCarti(Carte *set_carti);
int TrageCarte(Carte *set_carti, int counter);
void DrawCarti(Carte *set_carti, int *ordine, int count, float centerX, float centerY);
void DrawCardBacks(int count, float centerX, float centerY);

#endif
