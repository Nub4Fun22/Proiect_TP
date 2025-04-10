#include <stdio.h>
#include <time.h>

#define RAYGUI_IMPLEMENTATION
#include "raylib.h"
#include "raygui/src/raygui.h"
#include "Game.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

int main(void) {
    srand(time(NULL));
    Carte set_carti[NUMAR_CARTI];
    bool start_game = false;

    GuiLoadIcons("iconset.rgi", true);
    SetTargetFPS(60);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Blackjack");

    while (!WindowShouldClose()) {
        float centerX = (SCREEN_WIDTH - 200) / 2;
        float centerY = (SCREEN_HEIGHT - 24) / 2;

        BeginDrawing();
        ClearBackground(DARKGREEN);

        if (GuiButton((Rectangle){centerX, centerY, 200, 24}, "#220#Start Game"))
            start_game = true;

        if (GuiButton((Rectangle){centerX, centerY + 34, 200, 24}, "Exit Game")) {
            CloseWindow();
            return 0;
        }

        EndDrawing();

        if (start_game) {
            CloseWindow();
            InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Blackjack");

            InitCarti(set_carti);
            GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0x000000FF);
            GuiSetFont(GetFontDefault());

            int card_counter = 0;
            int afisare_carduri_ordine[NUMAR_MAXIM_TRAGERE_CARTI] = {0};

            while (!WindowShouldClose()) {
                BeginDrawing();
                ClearBackground(DARKGREEN);

                if (GuiButton((Rectangle){SCREEN_WIDTH - 200, 0, 200, 24}, "Exit Game")) {
                    UnloadCarti(set_carti);
                    CloseWindow();
                    return 0;
                }

                if (GuiButton((Rectangle){centerX, centerY + 100, 200, 24}, "Draw Card")) {
                    int carte_trasa = TrageCarte(set_carti, card_counter);
                    if (carte_trasa != -1) {
                        set_carti[carte_trasa].afisare_carte++;
                        afisare_carduri_ordine[card_counter++] = carte_trasa;
                    }
                }

                DrawCarti(set_carti, afisare_carduri_ordine, card_counter, centerX, centerY);
                EndDrawing();
            }

            UnloadCarti(set_carti);
            CloseWindow();
            return 0;
        }
    }

    CloseWindow();
    return 0;
}