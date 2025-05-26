
#include <stdio.h>
#include <time.h>
#define RAYGUI_IMPLEMENTATION
#include "raylib.h"
#include "raygui/src/raygui.h"
#include "Game.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

typedef enum {
    SCREEN_MENU,
    SCREEN_GAME
} GameScreen;


void SaveLastScores(int player, int dealer) {
    FILE *f = fopen("score.txt", "w");
    if (f) {
        fprintf(f, "%d %d", player, dealer);
        fclose(f);
    }
}

void LoadLastScores(int *player, int *dealer) {
    FILE *f = fopen("score.txt", "r");
    if (f) {
        fscanf(f, "%d %d", player, dealer);
        fclose(f);
    } else {
        *player = -1;
        *dealer = -1;
    }
}


int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Blackjack");
    InitAudioDevice();
    SetTargetFPS(60);
    GuiLoadIcons("iconset.rgi", true);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0x000000FF);
    GuiSetFont(GetFontDefault());

    Music menuMusic = LoadMusicStream("Music/menu.mp3");
    Music gameMusic = LoadMusicStream("Music/game.mp3");
    float volume = 5.0f;

    Texture2D back_card = LoadTexture("../My_Project/Cards/Back.png");
    if (back_card.id == 0) {
        printf("Eroare: Back.png nu a fost încărcat corect!\n");
    }

    Carte set_carti[NUMAR_CARTI];
    InitCarti(set_carti);

    GameScreen currentScreen = SCREEN_MENU;
    int lastPlayerScore, lastDealerScore;
    bool musicStarted = false;
    bool showPreviousScore = false;
    LoadLastScores(&lastPlayerScore, &lastDealerScore);

    int card_counter = 0;
    int afisare_carduri_ordine[NUMAR_MAXIM_TRAGERE_CARTI] = {0};
    int dealer_cards[NUMAR_MAXIM_TRAGERE_CARTI] = {0};
    int dealer_counter = 0;

    bool player_turn = true;
    bool dealer_done = false;
    bool reveal_dealer = false;
    bool game_over = false;
    int result_action = 0;
    const char *result_msg = NULL;

    while (!WindowShouldClose()) {
        float centerX = (SCREEN_WIDTH - 200) / 2;
        float centerY = (SCREEN_HEIGHT - 24) / 2;

        if (currentScreen == SCREEN_MENU) {
            UpdateMusicStream(menuMusic);
            SetMusicVolume(menuMusic, volume / 10.0f);
            if (!musicStarted) {
                PlayMusicStream(menuMusic);
                musicStarted = true;
            }

            BeginDrawing();
            ClearBackground(DARKGREEN);
                GuiSliderBar((Rectangle){SCREEN_WIDTH - 280, SCREEN_HEIGHT - 40, 200, 20}, NULL, TextFormat("Volume: %d", (int)volume), &volume, 0, 10);

            DrawText("Blackjack Roulette", SCREEN_WIDTH/2 - MeasureText("Blackjack Roulette", 20)/2, 20, 20, RAYWHITE);
            

            if (GuiButton((Rectangle){centerX, centerY, 200, 24}, "#220#Start Game")) {
                StopMusicStream(menuMusic);
                PlayMusicStream(gameMusic);
                SetMusicVolume(gameMusic, volume / 10.0f);

                dealer_cards[dealer_counter++] = TrageCarte(set_carti, 0);
                dealer_cards[dealer_counter++] = TrageCarte(set_carti, 0);
                set_carti[dealer_cards[0]].afisare_carte = 1;
                set_carti[dealer_cards[1]].afisare_carte = 1;

                currentScreen = SCREEN_GAME;
            }

            
            if (GuiButton((Rectangle){centerX, centerY + 68, 200, 24}, "Previous Score")) {
                showPreviousScore = !showPreviousScore;
            }

            if (showPreviousScore && lastPlayerScore != -1 && lastDealerScore != -1) {
                DrawText(TextFormat("Last Score - You: %d | Dealer: %d", lastPlayerScore, lastDealerScore), centerX, centerY + 130, 20, WHITE);
            }


            if (GuiButton((Rectangle){centerX, centerY + 34, 200, 24}, "Exit Game")) {
                break;
            }

            EndDrawing();
        }
        else if (currentScreen == SCREEN_GAME) {
            UpdateMusicStream(gameMusic);
            SetMusicVolume(gameMusic, volume / 10.0f);

            BeginDrawing();
            ClearBackground(DARKGREEN);
                GuiSliderBar((Rectangle){SCREEN_WIDTH - 280, SCREEN_HEIGHT - 40, 200, 20}, NULL, TextFormat("Volume: %d", (int)volume), &volume, 0, 10);

            DrawText("Blackjack Roulette", SCREEN_WIDTH/2 - MeasureText("Blackjack Roulette", 20)/2, 20, 20, RAYWHITE);

            if (GuiButton((Rectangle){SCREEN_WIDTH - 200, 0, 200, 24}, "Exit Game")) {
                break;
            }

            if (player_turn) {
                if (GuiButton((Rectangle){50, 300, 120, 24}, "Draw Card")) {
                    DrawCardForPlayer(set_carti, afisare_carduri_ordine, &card_counter);
                }
                
                static bool showSadFace = false;
                if (GuiButton((Rectangle){160, 300, 150, 24}, "Force Dealer Draw")) {
                    showSadFace = UseForceDealerDraw(set_carti, dealer_cards, &dealer_counter, afisare_carduri_ordine, &card_counter);
                }
                if (showSadFace) {
                    DrawText(":C", SCREEN_WIDTH/2 - MeasureText(":C", 80)/2, SCREEN_HEIGHT/2 - 40, 80, RED);
                }

                if (GuiButton((Rectangle){360, 300, 100, 24}, "Pass")) {
                    player_turn = false;
                    dealer_done = true;
                    DealerDraw(set_carti, dealer_cards, &dealer_counter);
                    int player_score = CalculateScore(set_carti, afisare_carduri_ordine, card_counter);
                    int dealer_score = CalculateScore(set_carti, dealer_cards, dealer_counter);
                    result_msg = ((player_score > 21) || (dealer_score <= 21 && dealer_score > player_score)) ? "You Lose!" : "You Win!";
                    game_over = true;
                    SaveLastScores(player_score, dealer_score);
                }
                if (GuiButton((Rectangle){470, 300, 100, 24}, "Peek")) {
                    reveal_dealer = true;
                }
                if (GuiButton((Rectangle){590, 300, 150, 24}, "Reset One Card") && card_counter > 0) {
                    card_counter--;
                    set_carti[afisare_carduri_ordine[card_counter]].afisare_carte--;
                }
            }

            if (!player_turn && dealer_done) {
                DrawCarti(set_carti, dealer_cards, dealer_counter, centerX, centerY - 100);
            } else if (reveal_dealer) {
                DrawCarti(set_carti, dealer_cards, dealer_counter, centerX, centerY - 100);
            } else {
                DrawCardBacks(dealer_counter, centerX, centerY - 100, back_card);
            }

            DrawCarti(set_carti, afisare_carduri_ordine, card_counter, centerX, centerY);
            int total_score = CalculateScore(set_carti, afisare_carduri_ordine, card_counter);
            char score_text[50];
            sprintf(score_text, "Score: %d", total_score);
            DrawText(score_text, SCREEN_WIDTH - 150, 40, 20, WHITE);
                if (dealer_done || reveal_dealer) {
                    char dealer_text[50];
                    int dealer_score = CalculateScore(set_carti, dealer_cards, dealer_counter);
                    sprintf(dealer_text, "Dealer: %d", dealer_score);
                    DrawText(dealer_text, SCREEN_WIDTH - 150, 70, 20, WHITE);
                }


            if (game_over) {
                result_action = GuiMessageBox((Rectangle){ SCREEN_WIDTH/2 - 125, SCREEN_HEIGHT/2 - 60, 250, 140 },
                                              "Game Over", result_msg, "Exit;Main Menu");
                if (result_action == 1) break;
                else if (result_action == 2) {
                    currentScreen = SCREEN_MENU;
                    StopMusicStream(gameMusic);
                    musicStarted = false;
                    LoadLastScores(&lastPlayerScore, &lastDealerScore);
                    card_counter = 0;
                    dealer_counter = 0;
                    player_turn = true;
                    dealer_done = false;
                    reveal_dealer = false;
                    game_over = false;
                    for (int i = 0; i < NUMAR_CARTI; i++) set_carti[i].afisare_carte = 0;
                    dealer_cards[dealer_counter++] = TrageCarte(set_carti, 0);
                    dealer_cards[dealer_counter++] = TrageCarte(set_carti, 0);
                    set_carti[dealer_cards[0]].afisare_carte = 1;
                    set_carti[dealer_cards[1]].afisare_carte = 1;
                }
            }

            EndDrawing();
        }
    }

    UnloadCarti(set_carti);
    UnloadTexture(back_card);
    UnloadMusicStream(menuMusic);
    UnloadMusicStream(gameMusic);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
