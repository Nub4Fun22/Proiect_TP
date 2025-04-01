#include <stdio.h>
#include <time.h>
#define RAYGUI_IMPLEMENTATION 
#include "raylib.h"
#include "raygui/src/raygui.h"
#define Screen_Width 800   
#define Screen_Height 450
#define Numar_Carti 12    // TO NOT BE MODIFIED !!
#define Numar_Maxim_Tragere_Carti 5 // TO NOT BE MODIFIED !!
bool Start_Game = false;

typedef struct {
    int afisare_carte; // contor afisare carte per frame
    char poza_carte[3000];
    Texture2D texture_carti;
    int val_carte;
}Carte;

void Initializare_Carti(Carte *set_carti)
{
    for(int i=2;i<Numar_Carti;i++)
    {
        set_carti[i].val_carte=i;
        set_carti[i].afisare_carte=0;
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
    for (int i = 2; i < Numar_Carti; i++)
    {
        UnloadTexture(set_carti[i].texture_carti); // Unload the texture
    }
}

int Trage_Carte(Carte *set_carti,float Center_X,float Center_Y,int counter)
{
    int carte_index=rand()%Numar_Carti;
    if(counter<Numar_Maxim_Tragere_Carti)
    {
        while(true)
        {
            while(carte_index<=1)
                carte_index=rand()%Numar_Carti;
                        
            if(set_carti[carte_index].afisare_carte<2)
                return carte_index;
            carte_index=rand()%Numar_Carti;
        }
    }
    printf("Erroare for no reason:%d\n",carte_index); //A se repara cumva pt 1 si 0
    return -1;
}
// for(int i=2;i<Numar_Carti;i++)
// DrawTexture(set_carti[i].texture_carti,Center_X+((i-4)*40),Center_Y,WHITE);

int main(void)
{
    srand(time(NULL));
    Carte set_carti[Numar_Carti];
    GuiLoadIcons("iconset.rgi",true);  
    SetTargetFPS(60);   ///Frames per second
    InitWindow(Screen_Width,Screen_Height,"Blackjack");  // The Meniu Window
    while (!WindowShouldClose())
    {
        float Center_X,Center_Y;
        Center_X=(int)(Screen_Width-200)/2;  //Calculating the center X for my button a
        Center_Y=(int)(Screen_Height-24)/2;  //Calculating the center Y for my button
        BeginDrawing();
        ClearBackground(DARKGREEN);
        if (GuiButton((Rectangle){Center_X,Center_Y,200,24}, "#220#Start Game"))
            Start_Game = true;

        //EXIT BUTTON
        if(GuiButton((Rectangle){Center_X,Center_Y+34,200,24},"Exit Game"))
            {
                CloseWindow();
                return 0;
            }
        EndDrawing();
        
        if(Start_Game==true)
            {
                CloseWindow();
                InitWindow(Screen_Width,Screen_Height,"Blackjack"); // The Window for the game itself
                    Initializare_Carti(set_carti);

                    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0x000000FF);
                    GuiSetFont(GetFontDefault());

                    int Card_Counter=0;
                    int afisare_carduri_ordine[Numar_Maxim_Tragere_Carti]={0};
                while (!WindowShouldClose())
                {
                    BeginDrawing();
                    ClearBackground(DARKGREEN);
                    
                        if(GuiButton((Rectangle){Screen_Width-200,0,200,24},"Exit Game"))
                            {
                                CloseWindow();
                                return 0;
                            }

                        if(GuiButton((Rectangle){Center_X,Center_Y+100,200,24},"Draw Card"))
                        {
                            int carte_trasa=Trage_Carte(set_carti,Center_X,Center_Y,Card_Counter);
                            printf("Dupa functie: %d\n",carte_trasa);
                            if(carte_trasa!=-1)
                            {
                                set_carti[carte_trasa].afisare_carte++;
                                printf("Nr de carti pentru %d este %d\n",carte_trasa,set_carti[carte_trasa].afisare_carte);
                                afisare_carduri_ordine[Card_Counter]=carte_trasa;
                            }
                            Card_Counter++;
                        }

                        for(int i=0;i<Card_Counter;i++)
                        {
                            int index=afisare_carduri_ordine[i];
                            if(index!=0)
                                if(set_carti[index].afisare_carte!=0)
                                    DrawTexture(set_carti[index].texture_carti,Center_X+((i-4)*40),Center_Y,WHITE);
                        }
                    EndDrawing();

                }
                    CloseWindow();
                    return 0;
            }

    }
    CloseWindow();
    return 0;
}                         