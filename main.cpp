#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <vector>
#include "headers/atlas.hpp"
#include "headers/input.hpp"
#include "headers/gameplay.hpp"
#include "headers/funcs.hpp"

using namespace atlas;
using namespace input;
using namespace gameplay;
using namespace funcs;

// EXCUSE THE MESSY CODE! This is my first raylib project and I'm not necessarily known for optimized codebases lol, so look at this with a grain of salt
int main()
{
    // Window parameters go here
    const int screenWidth = 160;
    const int screenHeight = 140;
    const int windowScale = 5;

    std::vector<Bullet> bullets;

    // Needed to make audio able to be played
    InitAudioDevice();

    // My tracker music! Hooray!
    Music musCh1 = LoadMusicStream("assets/music/mus.ch1.wav");
    Music musCh2 = LoadMusicStream("assets/music/mus.ch2.wav");
    Music musCh3 = LoadMusicStream("assets/music/mus.ch3.wav");
    Music musCh4 = LoadMusicStream("assets/music/mus.ch4.wav");
    Music sfxCh2 = LoadMusicStream("assets/music/sfx.ch2.wav");
    Music sfxCh4 = LoadMusicStream("assets/music/sfx.ch4.wav");
    Music sfx2Ch4 = LoadMusicStream("assets/music/sfx2.ch4.wav");
    PlayMusicStream(musCh1);
    PlayMusicStream(musCh2);
    PlayMusicStream(musCh3);
    PlayMusicStream(musCh4);
    
    // Defining a color for the background (note: kind of redundant, oops)
    Color gb = GetColor(0x9BBC0FFF);

    std::string boxTxt = TextFormat("* Froggit hopped close!\n* Press Z (or A)to\nselect and attack!");
    
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

    bool enemyShake = false;
    float enemyShakeMagnitude = 2.0f;

    bool hasAttacked = true;
    int enDamageTaken = 0;

    int bulletInterval = 18;
    int bulletCooldown = bulletInterval;

    int fightLength = 150;
    int fightDuration = 0;

    Vector2 playerPos = {77, 93};

    // Types: 0 = health, 1 = attack, 2 = defense
    struct Item {
        std::string name = "";
        int type = 0;
        int increment = 0;
    };

    std::vector<Item> inventory;

    struct Item itemList[] = {
        {"Candy", 0, 10},
        {"Boxing Gloves", 1, 5},
        {"Wooden Shield", 2, 10},
    };

    std::string actList[] = {
        "Compliment",
        "Threaten",
    };

    for (int i = 0; i < (sizeof(itemList) / sizeof(itemList[0])); i++) {
        inventory.push_back(itemList[i]);
    }

    // Image/atlas assets
    Texture2D textBox = LoadTexture("assets/imgs/text_box.png");
    Texture2D attackBox = LoadTexture("assets/imgs/attack_box.png");
    Texture2D fightBox = LoadTexture("assets/imgs/fight_box.png");
    Texture2D bg = LoadTexture("assets/imgs/bg.png");
    Texture2D froggitHurt = LoadTexture("assets/imgs/froggit_hurt.png");
    TextureAtlas optAtlas = LoadAtlas("assets/atlases/opt.rtpa");
    AnimAtlas froggitAtlas = {LoadAtlas("assets/atlases/froggit.rtpa"), 0, 3, 0};
    Texture2D player = LoadTexture("assets/imgs/heart.png");

    // Used for the enemy shake effect
    Vector2 enemyOffset = Vector2({0, 0});

    // Font asset
    Font font = LoadFontEx("assets/fonts/detectives_n_dames_by_2bitcrook.ttf", 8, 0, 95);

    // Used for determining which option to highlight (and select if the select key is pressed once that's implemented)
    int menuSelect = 0;

    // Used for menu switch statement (0 = main, 1 = attack, 2 = dodge, 3 = act, 4 = item, 5 = mercy)
    int menuState = 0;

    bool canMercy = false;

    const int maxHp = 20;
    int curHp = maxHp;

    float damageMult = 1.0f;
    float defenseMult = 1.0f;

    const int maxEnHp = 50;
    int curEnHp = maxEnHp;
    int prevEnHp = curEnHp;

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
        UpdateMusicStream(sfx2Ch4);
        ClearBackground(gb);
        BeginDrawing();
        
        if (getInput("restart") && !menuState) {
            curEnHp = maxEnHp;
            curHp = maxHp;
            canMercy = false;
            boxTxt = TextFormat("* Froggit hopped close!\n* Press Z (or A)to\nselect and attack!");
            inventory.clear();
            for (int i = 0; i < (sizeof(itemList) / sizeof(itemList[0])); i++) {
                inventory.push_back(itemList[i]);
            }
        }

        // Draws all visuals
        DrawTexture(bg, 0, 0, WHITE);
        
        // A separate function is used to animate animated atlas sprites, makes everything much cleaner
        if (!enemyShake && curEnHp > 0) {AnimateAtlasSprite(froggitAtlas, (Vector2){50, 22} + enemyOffset);}
        else {DrawTexture(froggitHurt, 50 + enemyOffset.x, 22 + enemyOffset.y, WHITE);}
        if (curEnHp > 0) {
            std::string infoStr(TextFormat("FRISK   LV 1   HP %d", curHp));
            infoStr += TextFormat("/%d", maxHp);
            DrawTextEx(font, infoStr.c_str(), (Vector2){7, 116}, (float)font.baseSize, 2, gb);
            switch (menuState) {
            case 0:
                DrawTexture(textBox, 7, 78, WHITE);
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
                            if (curEnHp > 0) {
                                menuState = 1;
                                hasAttacked = false;
                                InitAttack();
                            }
                            break;
                        case 1:
                            menuSelect = 0;
                            menuState = 3;
                            break;
                        case 2:
                            if (inventory.size() > 0) {
                                menuSelect = 0;
                                menuState = 4;
                            }
                            break;
                        case 3:
                            if (canMercy){
                                curEnHp = 0;
                                menuSelect = 0;
                                menuState = 0;
                            }
                            break;
                    }
                    SetMusicVolume(musCh2, 0);
                    PlayMusicStream(sfxCh2); 
                }
                // HP display text
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
                // Draws text within the text box
                DrawTextEx(font, boxTxt.c_str(), (Vector2){11, 82}, (float)font.baseSize, 2, gb);
                break;
            case 1:
                DrawTexture(attackBox, 7, 78, WHITE);
                switch (AttackMechanic(getInput("select"))) {
                    case 0:
                        DrawAttackBar(hasAttacked);
                        break;
                    case 1:
                        hasAttacked = true;
                        menuState = 2;
                        enDamageTaken = 0;
                        bulletCooldown = bulletInterval;
                        fightDuration = 0;
                        break;
                    case 2:
                        hasAttacked = true;
                        prevEnHp = curEnHp;
                        enDamageTaken = DamageEnemy(damageMult);
                        shakeMagnitude = enDamageTaken;
                        shakeDuration = 0.5f;
                        SetMusicVolume(musCh4, 0);
                        PlayMusicStream(sfx2Ch4);
                        break;
                }
                break;
            case 2:
                DrawTexture(fightBox, 62, 78, WHITE);
                if (getInput("up") && !CheckCollisionRecs(Rectangle{playerPos.x, playerPos.y, 6, 6}, Rectangle{62, 79, 37, 2})) {playerPos.y--;}
                if (getInput("down") && !CheckCollisionRecs(Rectangle{playerPos.x, playerPos.y, 6, 6}, Rectangle{62, 111, 37, 2})) {playerPos.y++;}
                if (getInput("left") && !CheckCollisionRecs(Rectangle{playerPos.x, playerPos.y, 6, 6}, Rectangle{62, 79, 3, 36})) {playerPos.x--;}
                if (getInput("right") && !CheckCollisionRecs(Rectangle{playerPos.x, playerPos.y, 6, 6}, Rectangle{95, 79, 2, 36})) {playerPos.x++;}
                DrawTexture(player, playerPos.x, playerPos.y, WHITE);
                bulletCooldown--;
                fightDuration++;
                if (fightDuration >= fightLength) {
                    bullets.clear();
                    menuState = 0;
                    break;
                }
                if (!bulletCooldown) {
                    bullets.push_back(*new Bullet);
                    bullets.back().BulletLogic();
                    bulletCooldown = bulletInterval;
                }
                for (auto it = bullets.begin(); it != bullets.end(); ) {
                    it->BulletLogic();
                    if (it->ShouldDeleteBullet())
                    {
                        it = bullets.erase(it);
                    }
                    else if (it->BulletCollision(playerPos))
                    {
                        it = bullets.erase(it);
                        if (curHp >= (2 / defenseMult)) {curHp -= (2 / defenseMult);}
                    }
                    else {
                        ++it;
                    }
                }
                break;
            case 3:
                DrawTexture(textBox, 7, 78, WHITE);
                for (int i = 0; i < sizeof(actList) / sizeof(actList[0]); i++) {
                    std::string name = actList[i];
                    if (i == menuSelect) {name += " <-";}
                    DrawTextEx(font, name.c_str(), (Vector2){11, 82 + (10*i)}, (float)font.baseSize, 2, gb);
                }
                if (getInput("back")) {
                    menuSelect = 0;
                    menuState = 0;
                }
                if (getInput("down") && !IsMusicStreamPlaying(sfxCh4)) {
                    if (menuSelect < sizeof(actList) / sizeof(actList[0]) - 1) {menuSelect++;}
                    else {menuSelect = 0;}
                    SetMusicVolume(musCh4, 0);
                    PlayMusicStream(sfxCh4);
                } else if (getInput("up") && !IsMusicStreamPlaying(sfxCh4)) {
                    if (menuSelect > 0) {menuSelect--;}
                    else {menuSelect = sizeof(actList) / sizeof(actList[0]) - 1;}
                    SetMusicVolume(musCh4, 0);
                    PlayMusicStream(sfxCh4);
                }
                if (getInput("select") && !IsMusicStreamPlaying(sfxCh2)) {
                    if (actList[menuSelect] == "Compliment") {canMercy = true;}
                    menuState = 2;
                    menuSelect = 0;
                    enDamageTaken = 0;
                    enemyShake = false;
                    bulletCooldown = bulletInterval;
                    fightDuration = 0;
                    SetMusicVolume(musCh2, 0);
                    PlayMusicStream(sfxCh2); 
                }
                break;
            case 4:
                DrawTexture(textBox, 7, 78, WHITE);
                for (int i = 0; i < inventory.size(); i++) {
                    std::string name = inventory[i].name;
                    if (i == menuSelect) {name += " <-";}
                    DrawTextEx(font, name.c_str(), (Vector2){11, 82 + (10*i)}, (float)font.baseSize, 2, gb);
                }
                if (getInput("back")) {
                    menuSelect = 0;
                    menuState = 0;
                }
                if (getInput("down") && !IsMusicStreamPlaying(sfxCh4)) {
                    if (menuSelect < inventory.size() - 1) {menuSelect++;}
                    else {menuSelect = 0;}
                    SetMusicVolume(musCh4, 0);
                    PlayMusicStream(sfxCh4);
                } else if (getInput("up") && !IsMusicStreamPlaying(sfxCh4)) {
                    if (menuSelect > 0) {menuSelect--;}
                    else {menuSelect = inventory.size() - 1;}
                    SetMusicVolume(musCh4, 0);
                    PlayMusicStream(sfxCh4);
                }
                if (getInput("select") && !IsMusicStreamPlaying(sfxCh2)) {
                    switch (inventory[menuSelect].type) {
                        case 0:
                            curHp = Clamp(curHp + inventory[menuSelect].increment, 0, maxHp);
                            break;
                        case 1:
                            damageMult += float(inventory[menuSelect].increment) / 10;
                            break;
                        case 2:
                            defenseMult += float(inventory[menuSelect].increment) / 10;
                            break;
                    }
                    inventory.erase(inventory.begin() + menuSelect);
                    menuState = 0;
                    menuSelect = 0;
                    SetMusicVolume(musCh2, 0);
                    PlayMusicStream(sfxCh2); 
                }
                break;
            default:
                 DrawTexture(textBox, 7, 78, WHITE);
        }
        DrawAtlasSprite(optAtlas, (menuSelect == 0 && menuState == 0 ? "fight_selected" : "fight"), (Vector2){7, 127});
        DrawAtlasSprite(optAtlas, (menuSelect == 1 && menuState == 0 ? "act_selected" : "act"), (Vector2){46, 127});
        DrawAtlasSprite(optAtlas, (menuSelect == 2 && menuState == 0 ? "item_selected" : "item"), (Vector2){86, 127});
        switch (canMercy) {
            case 0: DrawAtlasSprite(optAtlas, (menuSelect == 3 && menuState == 0 ? "nomercy_selected" : "nomercy"), (Vector2){125, 127}); break;
            case 1: DrawAtlasSprite(optAtlas, (menuSelect == 3 && menuState == 0 ? "mercy_selected" : "mercy"), (Vector2){125, 127}); break;
        }}
        else {
            DrawTexture(textBox, 7, 78, WHITE);
            boxTxt = TextFormat("* Froggit is defeated!\nPress R (or SELECT) to\nrestart!");
            DrawTextEx(font, boxTxt.c_str(), (Vector2){11, 82}, (float)font.baseSize, 2, gb);
        }

        DrawTextEx(font, TextFormat("%d", curEnHp), (Vector2){screenWidth / 2 - ((MeasureText(TextFormat("%d", curEnHp), (float)font.baseSize)) / 2), 14}, (float)font.baseSize, 2, gb);
        
        // Unmutes music played in certain channels when sound effects played in them finish (nice lil hardware accuracy thing!)
        if (IsMusicStreamPlaying(sfxCh4) && (GetMusicTimePlayed(sfxCh4) / GetMusicTimeLength(sfxCh4)) >= 0.5f) {
            StopMusicStream(sfxCh4);
            SetMusicVolume(musCh4, 1);
        }
        if (IsMusicStreamPlaying(sfx2Ch4)) {
            if ((GetMusicTimePlayed(sfx2Ch4) / GetMusicTimeLength(sfx2Ch4)) >= 0.95f) {
                StopMusicStream(sfx2Ch4);
                SetMusicVolume(musCh4, 1);
                if (curEnHp > 0) {
                    menuState = 2;
                    enDamageTaken = 0;
                    enemyShake = false;
                    froggitAtlas.curFrame = 9;
                    bulletCooldown = bulletInterval;
                    fightDuration = 0;
                }
                else {
                    enemyShake = false;
                    menuState = 0;
                }
            }
            // If this sound effect has reached the enemy damage sound bite, begin decreasing HP and shaking enemy
            else if ((GetMusicTimePlayed(sfx2Ch4) / GetMusicTimeLength(sfx2Ch4)) >= 0.5f) {
                enemyShake = true;
                curEnHp = Lerp(curEnHp, Clamp(prevEnHp - enDamageTaken, 0, maxEnHp), 0.1f);
            }
        }
        if (IsMusicStreamPlaying(sfxCh2) && (GetMusicTimePlayed(sfxCh2) / GetMusicTimeLength(sfxCh2)) >= 0.5f) {
            StopMusicStream(sfxCh2);
            SetMusicVolume(musCh2, 1);
        }

        // Render target shake uses duration to track how long it should be shaking for (the shake itself is done by offsetting the position by a shake offset)
        if (shakeDuration > 0.0f)
        {
            shakeDuration -= GetFrameTime();
            float shakeX = (float)(rand() % 100 - 50) / 50.0f * shakeMagnitude;
            float shakeY = (float)(rand() % 100 - 50) / 50.0f * shakeMagnitude;
            renderOffset = (Vector2){shakeX, shakeY};
        }
        else
        {
            renderOffset = (Vector2){0, 0};
        }

        // Enemy shake uses a boolean, so that it will shake throughout the entire health deduction lerp
        if (enemyShake)
        {
            float shakeX = (float)(rand() % 100 - 50) / 50.0f * enemyShakeMagnitude;
            float shakeY = (float)(rand() % 100 - 50) / 50.0f * enemyShakeMagnitude;
            enemyOffset = (Vector2){shakeX, shakeY};
        }
        else
        {
            enemyOffset = (Vector2){0, 0};
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
    UnloadMusicStream(sfx2Ch4);
    UnloadRenderTexture(target);
    CloseAudioDevice();
    CloseWindow();
}