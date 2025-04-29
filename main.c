/* main.c */
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
            int dealer_cards[NUMAR_MAXIM_TRAGERE_CARTI] = {0};
            int dealer_counter = 0;

            dealer_cards[dealer_counter++] = TrageCarte(set_carti, 0);
            dealer_cards[dealer_counter++] = TrageCarte(set_carti, 0);
            set_carti[dealer_cards[0]].afisare_carte = 1;
            set_carti[dealer_cards[1]].afisare_carte = 1;

            bool player_turn = true;
            bool dealer_done = false;

            while (!WindowShouldClose()) {
                BeginDrawing();
                ClearBackground(DARKGREEN);

                if (GuiButton((Rectangle){SCREEN_WIDTH - 200, 0, 200, 24}, "Exit Game")) {
                    UnloadCarti(set_carti);
                    CloseWindow();
                    return 0;
                }

                if (player_turn) {
                    if (GuiButton((Rectangle){centerX - 110, centerY + 100, 100, 24}, "Draw Card")) {
                        int carte_trasa = TrageCarte(set_carti, card_counter);
                        if (carte_trasa != -1) {
                            set_carti[carte_trasa].afisare_carte++;
                            afisare_carduri_ordine[card_counter++] = carte_trasa;
                        }
                    }

                    if (GuiButton((Rectangle){centerX + 10, centerY + 100, 100, 24}, "Pass")) {
                        player_turn = false;
                        int dealer_score = 0;
                        for (int i = 0; i < dealer_counter; i++)
                            dealer_score += set_carti[dealer_cards[i]].val_carte;

                        while (dealer_score < 17 && dealer_counter < NUMAR_MAXIM_TRAGERE_CARTI) {
                            int drawn = TrageCarte(set_carti, 0);
                            if (drawn == -1) break;
                            dealer_cards[dealer_counter++] = drawn;
                            set_carti[drawn].afisare_carte = 1;
                            dealer_score += set_carti[drawn].val_carte;
                        }

                        dealer_done = true;
                    }
                }

                if (!player_turn && dealer_done) {
                    DrawCarti(set_carti, dealer_cards, dealer_counter, centerX, centerY - 100);
                } else {
                    DrawCardBacks(2, centerX, centerY - 100);
                }

                DrawCarti(set_carti, afisare_carduri_ordine, card_counter, centerX, centerY);

                int total_score = 0;
                for (int i = 0; i < card_counter; i++) {
                    total_score += set_carti[afisare_carduri_ordine[i]].val_carte;
                }
                char score_text[50];
                sprintf(score_text, "Score: %d", total_score);
                DrawText(score_text, SCREEN_WIDTH - 150, 40, 20, WHITE);

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
