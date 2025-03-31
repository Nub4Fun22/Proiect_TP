#include <stdio.h>
#define RAYGUI_IMPLEMENTATION 
#include "raylib.h"
#include "raygui\src/raygui.h"
#define Screen_Width 800   
#define Screen_Height 450

bool Start_Game = false;

typedef struct {
    char poza_carte[3000];
    Texture2D texture_carti;
    int cnt_carte;
}Carte;

void Initializare_Carti(Carte *set_carti)
{
    for(int i=2;i<12;i++)
    {
        sprintf(set_carti[i].poza_carte,"E:/Facultate/Tp_Proiect/My_Project/Cards/Card_%d.png",i);
        Image image=LoadImage(set_carti[i].poza_carte);
         if (image.data == NULL) {
            printf("Failed to load image: %s\n", set_carti[i].poza_carte);
            continue; // Skip this card if the image fails to load
        }
        set_carti[i].texture_carti=LoadTextureFromImage(image);
        UnloadImage(image);
         if (set_carti[i].texture_carti.id == 0) {
            printf("Failed to load texture from image: %s\n", set_carti[i].poza_carte);
            continue; // Skip this card if the texture fails to load
         }
    }
}

void Unload_Carti(Carte *set_carti)
{
    for (int i = 2; i < 12; i++)
    {
        UnloadTexture(set_carti[i].texture_carti); // Unload the texture
    }
}

int main(void)
{
    Carte set_carti[12];
    Initializare_Carti(set_carti);
    GuiLoadIcons("iconset.rgi",true);  
    SetTargetFPS(60);   ///Frames per second
    InitWindow(Screen_Width,Screen_Height,"Blackjack");  // The Meniu Window
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
                InitWindow(Screen_Width,Screen_Height,"Blackjack"); // The Window for the game itself
                while (!WindowShouldClose())
                {
                    
                    BeginDrawing();
                    
                    ClearBackground(DARKGREEN);

                            DrawTexture(set_carti[3].texture_carti,480,320,DARKGRAY);


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