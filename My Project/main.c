#include <stdio.h>
#include "raylib.h"
#define Screen_Width 800
#define Screen_Height 420

int main(void)
{
    SetTargetFPS(60);
    InitWindow(Screen_Width,Screen_Height,"Blackjack");
    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(DARKGREEN);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}