#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Game.h"

void InitCarti(Carte *set_carti) {
    for (int i = 2; i < NUMAR_CARTI; i++) {
        set_carti[i].val_carte = i;
        set_carti[i].afisare_carte = 0;
        sprintf(set_carti[i].poza_carte, "../My_Project/Cards/Card_%d.png", i);
        Image image = LoadImage(set_carti[i].poza_carte);
        if (image.data == NULL) continue;
        set_carti[i].texture_carti = LoadTextureFromImage(image);
        UnloadImage(image);
    }
}

void UnloadCarti(Carte *set_carti) {
    for (int i = 2; i < NUMAR_CARTI; i++) {
        UnloadTexture(set_carti[i].texture_carti);
    }
}

int TrageCarte(Carte *set_carti, int counter) {
    int carte_index;
    do {
        carte_index = rand() % NUMAR_CARTI;
    } while ((carte_index <= 1 || set_carti[carte_index].afisare_carte >= 2) && counter < NUMAR_MAXIM_TRAGERE_CARTI);

    return (carte_index > 1 && set_carti[carte_index].afisare_carte < 2) ? carte_index : -1;
}

void DrawCarti(Carte *set_carti, int *ordine, int count, float centerX, float centerY) {
    for (int i = 0; i < count; i++) {
        int index = ordine[i];
        if (index > 1 && set_carti[index].afisare_carte > 0) {
            DrawTexture(set_carti[index].texture_carti, centerX + ((i - 4) * 40), centerY, WHITE);
        }
    }
}

void DrawCardBacks(int count, float centerX, float centerY, Texture2D back_texture) {
    for (int i = 0; i < count; i++) {
        DrawTexture(back_texture, centerX + ((i - 1) * 40), centerY, WHITE);
    }
}

void DrawCardForPlayer(Carte *set_carti, int *ordine, int *counter) {
    if (*counter >= NUMAR_MAXIM_TRAGERE_CARTI) return;
    int carte_trasa = TrageCarte(set_carti, *counter);
    if (carte_trasa != -1) {
        set_carti[carte_trasa].afisare_carte++;
        ordine[(*counter)++] = carte_trasa;
    }
}

void DealerDraw(Carte *set_carti, int *ordine, int *counter) {
    int score = 0;
    for (int i = 0; i < *counter; i++)
        score += set_carti[ordine[i]].val_carte;

    while (score < 17 && *counter < NUMAR_MAXIM_TRAGERE_CARTI) {
        int carte = TrageCarte(set_carti, *counter);
        if (carte == -1) break;
        ordine[(*counter)++] = carte;
        set_carti[carte].afisare_carte++;
        score += set_carti[carte].val_carte;
    }
}

int CalculateScore(Carte *set_carti, int *ordine, int count) {
    int score = 0;
    for (int i = 0; i < count; i++)
        score += set_carti[ordine[i]].val_carte;
    return score;
}