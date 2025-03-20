#include <stdio.h>
#define RAYGUI_IMPLEMENTATION 
#include "raylib.h"
#include "raygui\src/raygui.h"
#define Screen_Width 800   
#define Screen_Height 450

bool Start_Game = false;


int main(void)
{
    GuiLoadIcons("iconset.rgi",true);  
    SetTargetFPS(60);   ///Frames per second
    InitWindow(Screen_Width,Screen_Height,"Blackjack");
    while (!WindowShouldClose())
    {
        float button_X,button_Y;
        button_X=(int)(Screen_Width-200)/2;  //Calculating the center X for my button
        button_Y=(int)(Screen_Height-24)/2;  //Calculating the center Y for my button
        BeginDrawing();
        ClearBackground(DARKGREEN);
        if (GuiButton((Rectangle){button_X,button_Y,200,24}, "#220#Start Game"))
            Start_Game = true;

        //EXIT BUTTON
        if(GuiButton((Rectangle){button_X,button_Y+34,200,24},"Exit Game"))
            {
                CloseWindow();
                return 0;
            }


        
        if(Start_Game==true)
            {
                EndDrawing();
                CloseWindow();
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
        EndDrawing();
    }
    CloseWindow();
    return 0;
}