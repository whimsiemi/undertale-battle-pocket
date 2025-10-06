#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/atlas.h"
#include "headers/input.h"
#include "headers/gameplay.h"

using namespace atlas;
using namespace input;
using namespace gameplay;

int main()
{
    // Window parameters go here
    const int screenWidth = 160;
    const int screenHeight = 140;
    const int windowScale = 5;

    // Needed to make audio able to be played
    InitAudioDevice();

    // My tracker music! Hooray!
    Music musCh1 = LoadMusicStream("assets/music/mus.ch1.wav");
    Music musCh2 = LoadMusicStream("assets/music/mus.ch2.wav");
    Music musCh3 = LoadMusicStream("assets/music/mus.ch3.wav");
    Music musCh4 = LoadMusicStream("assets/music/mus.ch4.wav");
    Music sfxCh2 = LoadMusicStream("assets/music/sfx.ch2.wav");
    Music sfxCh4 = LoadMusicStream("assets/music/sfx.ch4.wav");
    PlayMusicStream(musCh1);
    PlayMusicStream(musCh2);
    PlayMusicStream(musCh3);
    PlayMusicStream(musCh4);
    
    // Defining a color for the background (note: kind of redundant, oops)
    Color gb = GetColor(0x9BBC0FFF);
    
    // When we initialize the window, we want the player to be able to resize it to be as big as they want
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth * windowScale, screenHeight * windowScale, "UNDERTALE Pocket Battle");

    // 30 FPS used because that's what the original game uses and I don't see a reason to change it
    SetTargetFPS(30);

    // Making a render texture that we can draw all visuals to then rescale for any resolution
    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);
    Vector2 renderOffset = Vector2({0, 0});

    float shakeDuration = 0.0f;
    float shakeMagnitude = 5.0f;

    // Image/atlas assets
    Texture2D textBox = LoadTexture("assets/imgs/text_box.png");
    Texture2D attackBox = LoadTexture("assets/imgs/attack_box.png");
    Texture2D bg = LoadTexture("assets/imgs/bg.png");
    TextureAtlas optAtlas = LoadAtlas("assets/atlases/opt.rtpa");
    AnimAtlas froggitAtlas = {LoadAtlas("assets/atlases/froggit.rtpa"), 0, 3, 0};

    // Font asset
    Font font = LoadFontEx("assets/fonts/detectives_n_dames_by_2bitcrook.ttf", 8, 0, 95);

    // Used for determining which option to highlight (and select if the select key is pressed once that's implemented)
    int menuSelect = 0;

    // Used for menu switch statement (0 = main, 1 = attack, 2 = act, 3 = item, 4 = mercy)
    int menuState = 0;

    const int maxHp = 20;
    int curHp = maxHp;

    const int maxEnHp = 50;
    int curEnHp = maxEnHp;

    // Main game loop
    while (!WindowShouldClose()) {
        // Initialization for drawing and music (music is separated into channels, sound effects replacing the output of music in certain channels while they are playing)
        BeginTextureMode(target);
        UpdateMusicStream(musCh1);
        UpdateMusicStream(musCh2);
        UpdateMusicStream(musCh3);
        UpdateMusicStream(musCh4);
        UpdateMusicStream(sfxCh2);
        UpdateMusicStream(sfxCh4);
        ClearBackground(gb);
        BeginDrawing();

        // Draws all visuals
        DrawTexture(bg, 0, 0, WHITE);
        switch (menuState) {
            case 1:
                DrawTexture(attackBox, 7, 78, WHITE);
                break;
            default:
                 DrawTexture(textBox, 7, 78, WHITE);
        }
        DrawAtlasSprite(optAtlas, (menuSelect == 0 ? "fight_selected" : "fight"), (Vector2){7, 127});
        DrawAtlasSprite(optAtlas, (menuSelect == 1 ? "act_selected" : "act"), (Vector2){46, 127});
        DrawAtlasSprite(optAtlas, (menuSelect == 2 ? "item_selected" : "item"), (Vector2){86, 127});
        DrawAtlasSprite(optAtlas, (menuSelect == 3 ? "mercy_selected" : "mercy"), (Vector2){125, 127});

        // A separate function is used to animate animated atlas sprites, makes everything much cleaner
        AnimateAtlasSprite(froggitAtlas, "froggit", (Vector2){50, 22});

        DrawTextEx(font, TextFormat("%d", curEnHp), (Vector2){screenWidth / 2, 10}, (float)font.baseSize, 2, gb);

        std::string infoStr(TextFormat("FRISK   LV 1   HP %d", curHp));
        infoStr += TextFormat("/%d", maxHp);

        std::string boxTxt("* Froggit hopped close!");
        if (curEnHp <= 0) {
            boxTxt = TextFormat("* Froggit is defeated!");
        }
        else if (curEnHp <= (maxEnHp / 3)) {
            boxTxt = TextFormat("* Froggit is crticially \nwounded!");
        }
        else if (curEnHp <= (maxEnHp / 2)) {
            boxTxt = TextFormat("* Froggit is wounded!");
        }
        else if (curEnHp < maxEnHp)
        {
            boxTxt = TextFormat("* Froggit feels a small \n pain!");
        }

        DrawTextEx(font, infoStr.c_str(), (Vector2){7, 116}, (float)font.baseSize, 2, gb);

        // Navigate menu using polled inputs checked within the input header's "getInput" function (also handles sound effects overwriting music in their respective channels)
        switch (menuState)
        {
            case 0:
                if (getInput("right") && !IsMusicStreamPlaying(sfxCh4)) {
                    if (menuSelect < 3) {menuSelect++;}
                    else {menuSelect = 0;}
                    SetMusicVolume(musCh4, 0);
                    PlayMusicStream(sfxCh4);
                } else if (getInput("left") && !IsMusicStreamPlaying(sfxCh4)) {
                    if (menuSelect > 0) {menuSelect--;}
                    else {menuSelect = 3;}
                    SetMusicVolume(musCh4, 0);
                    PlayMusicStream(sfxCh4);
                }
                if (getInput("select") && !IsMusicStreamPlaying(sfxCh2)) {
                    switch (menuSelect) {
                        case 0:
                            menuState = 1;
                            initAttack();
                            break;
                    }
                    SetMusicVolume(musCh2, 0);
                    PlayMusicStream(sfxCh2); 
                }
                // Draws text within the text box
                DrawTextEx(font, boxTxt.c_str(), (Vector2){11, 82}, (float)font.baseSize, 2, gb);
                break;
            case 1:
                switch (attackMechanic(getInput("select"))) {
                    case 0:
                        drawAttackBar();
                        break;
                    case 1:
                        menuState = 0;
                        break;
                    case 2:
                        curEnHp = Clamp(curEnHp - damageEnemy(), 0, maxEnHp);
                        shakeMagnitude = damageEnemy();
                        shakeDuration = 0.5f;
                        menuState = 0;
                        SetMusicVolume(musCh2, 0);
                        PlayMusicStream(sfxCh2);
                        break;
                }
                break;
        }
        
        // Unmutes music played in certain channels when sound effects played in them finish (nice lil hardware accuracy thing!)
        if (IsMusicStreamPlaying(sfxCh4) && (GetMusicTimePlayed(sfxCh4) / GetMusicTimeLength(sfxCh4)) >= 0.5f) {
            StopMusicStream(sfxCh4);
            SetMusicVolume(musCh4, 1);
        }

        if (IsMusicStreamPlaying(sfxCh2) && (GetMusicTimePlayed(sfxCh2) / GetMusicTimeLength(sfxCh2)) >= 0.5f) {
            StopMusicStream(sfxCh2);
            SetMusicVolume(musCh2, 1);
        }

        if (shakeDuration > 0.0f)
        {
            shakeDuration -= GetFrameTime(); // Decrease shake time
            float shakeX = (float)(rand() % 100 - 50) / 50.0f * shakeMagnitude;
            float shakeY = (float)(rand() % 100 - 50) / 50.0f * shakeMagnitude;
            renderOffset = (Vector2){shakeX, shakeY};
        }
        else
        {
            renderOffset = (Vector2){0, 0}; // Reset camera
        }

        // Draws render texture to the screen at a scale relative to the window size
        EndTextureMode();
        Rectangle screenRect = {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
        DrawTexturePro(target.texture, (Rectangle){0, 0, (float)target.texture.width, -(float)target.texture.height}, screenRect, renderOffset, 0, WHITE);
        EndDrawing();
    }

    // Clearing stuff from RAM and closing the game when game loop ends
    UnloadMusicStream(musCh1);
    UnloadMusicStream(musCh2);
    UnloadMusicStream(musCh3);
    UnloadMusicStream(musCh4);
    UnloadMusicStream(sfxCh2);
    UnloadMusicStream(sfxCh4);
    UnloadRenderTexture(target);
    CloseAudioDevice();
    CloseWindow();
}