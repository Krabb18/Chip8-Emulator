#include "Include/raylib.h"
#include "Chip8CPU.h"

using namespace std;

const int screenWidth = 640;
const int screenHeight = 320;


void RenderScreenBuffer(Chip8CPU &chip8)
{
    //Image imgBuffer = GenImageColor(screenWidth, screenHeight, WHITE);

    for(int h = 0; h<screenHeight; h++)
    {
        for(int w = 0; w<screenWidth; w++)
        {
            int pixel = chip8.m_ScreenData[h][w];
            if(pixel == 0)
            {
                //cout << "hsdkfhjdsfjkhs" << endl;
                DrawPixel(w, h, BLACK);
                //ImageDrawPixel(&imgBuffer, w, h, RED);    
            }  
            else
            {
                //cout << "hsdkfhjdsfjkhs" << endl;
                DrawPixel(w, h, WHITE);
            }
        }
    }

    /*
        Texture2D texture = LoadTextureFromImage(imgBuffer);
    DrawTexture(texture, 0, 0, WHITE);

    UnloadTexture(texture);
    UnloadImage(imgBuffer);
    
    */

}

void HandleInputPressed(Chip8CPU &chip8)
{
    int key = -1;
    if(IsKeyDown(KEY_X))
    {
        key = 0;
    }

    if(IsKeyDown(KEY_Q))
    {
        key = 1;
    }

    if(IsKeyDown(KEY_W))
    {
        key = 2;
    }

    if(IsKeyDown(KEY_E))
    {
        key = 3;
    }

    if(IsKeyDown(KEY_E))
    {
        key = 4;
    }

    if(IsKeyDown(KEY_R))
    {
        key = 5;
    }

    if(IsKeyDown(KEY_T))
    {
        key = 6;
    }

    if(IsKeyDown(KEY_Z))
    {
        key = 7;
    }

    if(IsKeyDown(KEY_U))
    {
        key = 8;
    }

    if(IsKeyDown(KEY_I))
    {
        key = 9;
    }

    if(IsKeyDown(KEY_O))
    {
        key = 10;
    }

    if(IsKeyDown(KEY_P))
    {
        key = 11;
    }

    if(IsKeyDown(KEY_A))
    {
        key = 12;
    }

    if(IsKeyDown(KEY_S))
    {
        key = 12;
    }

    if(IsKeyDown(KEY_D))
    {
        key = 13;
    }

    if(IsKeyDown(KEY_F))
    {
        key = 14;
    }

    if(IsKeyDown(KEY_G))
    {
        key = 15;
    }

    if(key != -1)
    {
        chip8.m_KeyState[key] = 1;
    }
    
}


void HandleInputReleased(Chip8CPU &chip8)
{
    int key = -1;
    if(IsKeyReleased(KEY_X))
    {
        key = 0;
    }

    if(IsKeyReleased(KEY_Q))
    {
        key = 1;
    }

    if(IsKeyReleased(KEY_W))
    {
        key = 2;
    }

    if(IsKeyReleased(KEY_E))
    {
        key = 3;
    }

    if(IsKeyReleased(KEY_E))
    {
        key = 4;
    }

    if(IsKeyReleased(KEY_R))
    {
        key = 5;
    }

    if(IsKeyReleased(KEY_T))
    {
        key = 6;
    }

    if(IsKeyReleased(KEY_Z))
    {
        key = 7;
    }

    if(IsKeyReleased(KEY_U))
    {
        key = 8;
    }

    if(IsKeyReleased(KEY_I))
    {
        key = 9;
    }

    if(IsKeyReleased(KEY_O))
    {
        key = 10;
    }

    if(IsKeyReleased(KEY_P))
    {
        key = 11;
    }

    if(IsKeyReleased(KEY_A))
    {
        key = 12;
    }

    if(IsKeyReleased(KEY_S))
    {
        key = 12;
    }

    if(IsKeyReleased(KEY_D))
    {
        key = 13;
    }

    if(IsKeyReleased(KEY_F))
    {
        key = 14;
    }

    if(IsKeyReleased(KEY_G))
    {
        key = 15;
    }

    if(key != -1)
    {
        chip8.m_KeyState[key] = 0;
    }
    
}


void HandleEmulator(Chip8CPU &chip8, int numFrame)
{
    HandleInputPressed(chip8);
    HandleInputReleased(chip8);

    chip8.DecreaseTimers( );
	for (int i = 0 ; i < numFrame; i++)
    {
        chip8.ExecuteOpcode( ) ;
    }
    RenderScreenBuffer(chip8);
}

int main(void)
{

    Chip8CPU chip8CPU;
    chip8CPU.CPUReset();
    chip8CPU.LoadRom("rom/space.ch8");


    InitWindow(screenWidth, screenHeight, "Chip8");

    InitAudioDevice();
    SetTargetFPS(100);

    Sound bieb = LoadSound("bieb.wav");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        
        if (!IsSoundReady(bieb)) {
        std::cout << "Sound file konnte nicht geladen werden!" << std::endl;
    }
        
        //DrawText("Hello Chip8 new", 00, 50, 20, BLACK);
        ClearBackground(RAYWHITE);
        HandleEmulator(chip8CPU, 60);
        if(chip8CPU.makeSound)
        {
            PlaySound(bieb);
            chip8CPU.makeSound = false;
        }

        //if (IsKeyPressed(KEY_SPACE)) PlaySound(bieb); 

        EndDrawing();
    }
    UnloadSound(bieb);
    CloseWindow();

    return 0;
}

