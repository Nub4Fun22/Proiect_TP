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
int TrageCarte(Carte *set_carti, int counter);
void DrawCarti(Carte *set_carti, int *ordine, int count, float centerX, float centerY);
void DrawCardBacks(int count, float centerX, float centerY, Texture2D back_texture);
void DrawCardForPlayer(Carte *set_carti, int *ordine, int *counter);
void DealerDraw(Carte *set_carti, int *ordine, int *counter);
int CalculateScore(Carte *set_carti, int *ordine, int count);

#endif

// Desenează maxim `maxCount` cărți dintr-un set
void DrawCartiLimitate(Carte *set_carti, int *ordine, int count, float centerX, float centerY, int maxCount);

// Power-up: Forțează dealerul să tragă o carte și cu 20% șansă și playerul
bool UseForceDealerDraw(Carte *set_carti, int *ordineDealer, int *counterDealer, int *ordinePlayer, int *counterPlayer);
