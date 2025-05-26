#include <stdio.h>
#include <time.h>
#define RAYGUI_IMPLEMENTATION
#include "raylib.h"
#include "raygui/src/raygui.h"
#include "Game.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

int main(void) {
    InitAudioDevice();
    Music menuMusic = LoadMusicStream("Music/menu.mp3");
    Music gameMusic = LoadMusicStream("Music/game.mp3");
    float volume = 5.0f;
    srand(time(NULL));
    Carte set_carti[NUMAR_CARTI];
    bool start_game = false;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Blackjack");
        GuiLoadIcons("iconset.rgi", true);
        SetTargetFPS(60);
        GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0x000000FF);
        GuiSetFont(GetFontDefault());
        PlayMusicStream(menuMusic);

    Texture2D back_card = LoadTexture("../My_Project/Cards/Back.png");
    if (back_card.id == 0) {
        printf("Eroare: Back.png nu a fost încărcat corect!\n");
    }

    while (!WindowShouldClose()) {
        UpdateMusicStream(menuMusic);
        float centerX = (SCREEN_WIDTH - 200) / 2;
        float centerY = (SCREEN_HEIGHT - 24) / 2;
        GuiSliderBar((Rectangle){centerX, centerY + 68, 200, 20}, NULL, TextFormat("Volume: %d", (int)volume), &volume, 0, 10);
        SetMusicVolume(menuMusic, volume / 10.0f);

        BeginDrawing();
        ClearBackground(DARKGREEN);

        DrawText("Blackjack Roulette", SCREEN_WIDTH/2 - MeasureText("Blackjack Roulette", 20)/2, 20, 20, RAYWHITE);

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
                GuiLoadIcons("iconset.rgi", true);
                SetTargetFPS(60);
                GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0x000000FF);
                PlayMusicStream(gameMusic);
                GuiSetFont(GetFontDefault());
                GuiSliderBar((Rectangle){centerX, centerY, 200, 20}, NULL, TextFormat("Volume: %d", (int)volume), &volume, 0, 10);
                SetMusicVolume(gameMusic, volume / 10.0f);

            Texture2D back_card = LoadTexture("../My_Project/Cards/Back.png");
            if (back_card.id == 0) {
                printf("Eroare: Back.png nu a fost încărcat corect!\n");
            }

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
            bool reveal_dealer = false;
            bool game_over = false;
            int result_action = 0;
            const char *result_msg = NULL;

            while (!WindowShouldClose()) {
                UpdateMusicStream(gameMusic);
                BeginDrawing();
                ClearBackground(DARKGREEN);

                DrawText("Blackjack Roulette", SCREEN_WIDTH/2 - MeasureText("Blackjack Roulette", 20)/2, 20, 20, RAYWHITE);

                if (GuiButton((Rectangle){SCREEN_WIDTH - 200, 0, 200, 24}, "Exit Game")) {
                    UnloadTexture(back_card);
                    UnloadCarti(set_carti);
                    CloseWindow();
                    return 0;
                }

                if (player_turn) {
                    if (GuiButton((Rectangle){50, 300, 100, 24}, "Draw Card")) {
                        DrawCardForPlayer(set_carti, afisare_carduri_ordine, &card_counter);
                    }
                    if (GuiButton((Rectangle){160, 300, 100, 24}, "Double Draw")) {
                        DrawCardForPlayer(set_carti, afisare_carduri_ordine, &card_counter);
                        DrawCardForPlayer(set_carti, afisare_carduri_ordine, &card_counter);
                    }
                    if (GuiButton((Rectangle){270, 300, 100, 24}, "Pass")) {
                        player_turn = false;
                        dealer_done = true;
                        DealerDraw(set_carti, dealer_cards, &dealer_counter);
                        int player_score = CalculateScore(set_carti, afisare_carduri_ordine, card_counter);
                        int dealer_score = CalculateScore(set_carti, dealer_cards, dealer_counter);
                        result_msg = (player_score > 21 || (dealer_score <= 21 && dealer_score >= player_score)) ? "You Lose!" : "You Win!";
                        game_over = true;
                        const char *result_msg = (player_score > 21 || (dealer_score <= 21 && dealer_score >= player_score)) ? "You Lose!" : "You Win!";
                        int result = GuiMessageBox((Rectangle){ SCREEN_WIDTH/2 - 125, SCREEN_HEIGHT/2 - 60, 250, 140 },
                            "Game Over", result_msg, "Exit;Replay");
                        if (result == 0) { } // waiting
                        else if (result == 1) {
                            UnloadTexture(back_card);
                            UnloadCarti(set_carti);
                            CloseWindow();
                            return 0;
                        } else if (result == 2) {
                            start_game = true;
                            break;
                        }
                    }
                    if (GuiButton((Rectangle){380, 300, 100, 24}, "Peek")) {
                        reveal_dealer = true;
                    }
                    if (GuiButton((Rectangle){490, 300, 120, 24}, "Reset One Card") && card_counter > 0) {
                        card_counter--;
                        set_carti[afisare_carduri_ordine[card_counter]].afisare_carte--;
                    }
                }

                if (!player_turn && dealer_done) {
                    DrawCarti(set_carti, dealer_cards, dealer_counter, centerX, centerY - 100);
                } else if (reveal_dealer) {
                    DrawCarti(set_carti, dealer_cards, dealer_counter, centerX, centerY - 100);
                } else {
                    DrawCardBacks(2, centerX, centerY - 100, back_card);
                }

                DrawCarti(set_carti, afisare_carduri_ordine, card_counter, centerX, centerY);
                int total_score = CalculateScore(set_carti, afisare_carduri_ordine, card_counter);
                char score_text[50];
                sprintf(score_text, "Score: %d", total_score);
                DrawText(score_text, SCREEN_WIDTH - 150, 40, 20, WHITE);

                if (game_over) {
                    result_action = GuiMessageBox((Rectangle){ SCREEN_WIDTH/2 - 125, SCREEN_HEIGHT/2 - 60, 250, 140 },
                                                  "Game Over", result_msg, "Exit;Replay");
                    if (result_action == 1) {
                        UnloadTexture(back_card);
                        UnloadCarti(set_carti);
                        CloseWindow();
                        return 0;
                    } else if (result_action == 2) {
                            UnloadTexture(back_card);
                            UnloadCarti(set_carti);
                            CloseWindow();
                            main();  // restart the entire game
                            return 0;
                    }
                    EndDrawing();
                    continue;
                }

                EndDrawing();
            }

            UnloadTexture(back_card);
            UnloadCarti(set_carti);
            CloseWindow();
            return 0;
        }
    }

    CloseWindow();
    return 0;
}