#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/atlas.h"
using namespace std;
using namespace atlas;

int main()
{
    // Window parameters go here
    const int screenWidth = 160;
    const int screenHeight = 140;
    const int windowScale = 5;

    // Needed to make audio able to be played
    InitAudioDevice();

    // My tracker music! Hooray!
    Music music = LoadMusicStream("assets/music/enemy_approaching.wav");
    PlayMusicStream(music);
    
    // Defining a color for the background (note: kind of redundant, oops)
    Color gbDark = GetColor(0x0F380FFF);
    
    // When we initialize the window, we want the player to be able to resize it to be as big as they want
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth * windowScale, screenHeight * windowScale, "UNDERTALE Pocket Battle");

    // 30 FPS used because that's what the original game uses and I don't see a reason to change it
    SetTargetFPS(30);

    // Making a render texture that we can draw all visuals to then rescale for any resolution
    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);

    // Image/atlas assets
    Texture2D textBox = LoadTexture("assets/imgs/text_box.png");
    Texture2D bg = LoadTexture("assets/imgs/bg.png");
    TextureAtlas optAtlas = LoadAtlas("assets/atlases/opt.rtpa");
    AnimAtlas froggitAtlas = {LoadAtlas("assets/atlases/froggit.rtpa"), 0, 3, 0};

    // Main game loop
    while (!WindowShouldClose()) {

        // Initialization for drawing and music
        BeginTextureMode(target);
        UpdateMusicStream(music);
        ClearBackground(gbDark);
        BeginDrawing();

        // Draws all visuals
        DrawTexture(bg, 0, 0, WHITE);
        DrawTexture(textBox, 7, 78, WHITE);
        DrawAtlasSprite(optAtlas, "fight", (Vector2){7, 124});
        DrawAtlasSprite(optAtlas, "act", (Vector2){46, 124});
        DrawAtlasSprite(optAtlas, "item", (Vector2){86, 124});
        DrawAtlasSprite(optAtlas, "mercy", (Vector2){125, 124});

        // A separate function is used to animate animated atlas sprites, makes everything much cleaner
        AnimateAtlasSprite(froggitAtlas, (Vector2){50, 22});

        // Draws render texture to the screen at a scale relative to the window size
        EndTextureMode();
        Rectangle screenRect = {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
        DrawTexturePro(target.texture, (Rectangle){0, 0, (float)target.texture.width, -(float)target.texture.height}, screenRect, (Vector2){0, 0}, 0, WHITE);
        EndDrawing();
    }

    // Clearing stuff from RAM and closing the game when game loop ends
    UnloadMusicStream(music);
    UnloadRenderTexture(target);
    CloseAudioDevice();
    CloseWindow();
}