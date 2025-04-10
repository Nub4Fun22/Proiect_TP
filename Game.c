#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Game.h"

void InitCarti(Carte *set_carti) {
    for (int i = 2; i < NUMAR_CARTI; i++) {
        set_carti[i].val_carte = i;
        set_carti[i].afisare_carte = 0;
        sprintf(set_carti[i].poza_carte, "E:/Facultate/Tp_Proiect/My_Project/Cards/Card_%d.png", i);

        Image image = LoadImage(set_carti[i].poza_carte);
        if (image.data == NULL) {
            printf("Failed to load image: %s\n", set_carti[i].poza_carte);
            continue;
        }

        set_carti[i].texture_carti = LoadTextureFromImage(image);
        UnloadImage(image);

        if (set_carti[i].texture_carti.id == 0) {
            printf("Failed to load texture from image: %s\n", set_carti[i].poza_carte);
        }
    }
}

void UnloadCarti(Carte *set_carti) {
    for (int i = 2; i < NUMAR_CARTI; i++) {
        UnloadTexture(set_carti[i].texture_carti);
    }
}

int TrageCarte(Carte *set_carti, int counter) {
    int carte_index = rand() % NUMAR_CARTI;

    if (counter < NUMAR_MAXIM_TRAGERE_CARTI) {
        while (1) {
            while (carte_index <= 1)
                carte_index = rand() % NUMAR_CARTI;

            if (set_carti[carte_index].afisare_carte < 2)
                return carte_index;

            carte_index = rand() % NUMAR_CARTI;
        }
    }

    printf("Unexpected error: %d\n", carte_index);
    return -1;
}

void DrawCarti(Carte *set_carti, int *ordine, int count, float centerX, float centerY) {
    for (int i = 0; i < count; i++) {
        int index = ordine[i];
        if (index > 1 && set_carti[index].afisare_carte > 0) {
            DrawTexture(set_carti[index].texture_carti, centerX + ((i - 4) * 40), centerY, WHITE);
        }
    }
}