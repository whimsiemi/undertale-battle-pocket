#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

typedef struct {
    char name[64];
    Rectangle sourceRec;
} SpriteInfo;

typedef struct {
    Texture2D texture;
    SpriteInfo sprites[32];
    int spriteCount;
} TextureAtlas;

TextureAtlas LoadAtlas(const char *rtpaPath) {
    // Initalize atlas
    TextureAtlas atlas = {0};

    // Open the file specified in the function argument
    FILE *file = fopen(rtpaPath, "r");

    char line[256];
    char imagePath[128] = {0};

    // Gets the path of the image and the index of its sprites by reading the lines with "a" and "s" prefixes
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#') continue;

        if (line[0] == 'a') {
            sscanf(line, "a %s", imagePath);
        }

        else if (line[0] == 's') {
            SpriteInfo *sprite = &atlas.sprites[atlas.spriteCount];

            char nameId[64], tag[64];
            int originX, originY, posX, posY, srcW, srcH;
            // We don't need these
            sscanf(line, "s %s %s %d %d %d %d %d %d", nameId, tag, &originX, &originY, &posX, &posY, &srcW, &srcH);

            strcpy(sprite->name, nameId);
            sprite->sourceRec = (Rectangle){posX, posY, srcW, srcH};

            atlas.spriteCount++;
        }
    }

    fclose(file);

    atlas.texture = LoadTexture(imagePath);

    return atlas;
}

void DrawAtlasSprite(TextureAtlas atlas, const char *spriteName, Vector2 position) {
    // Find the sprite within the atlas's sprite array and draw it if found
    for (int i = 0; i < atlas.spriteCount; i++) {
        if (strcmp(atlas.sprites[i].name, spriteName) == 0) {
            DrawTextureRec(atlas.texture, atlas.sprites[i].sourceRec, position, WHITE);
            break;
        }
    }
}

int main()
{
    const int screenWidth = 160;
    const int screenHeight = 140;
    const int windowScale = 5;

    InitAudioDevice();

    Music music = LoadMusicStream("assets/music/enemy_approaching.wav");
    PlayMusicStream(music);
    
    Color gbDark = GetColor(0x0F380FFF);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth * windowScale, screenHeight * windowScale, "UNDERTALE Battle System - Game Boy'd!");
    SetTargetFPS(30);

    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);

    Texture2D textBox = LoadTexture("assets/imgs/text_box.png");
    Texture2D bg = LoadTexture("assets/imgs/bg.png");
    TextureAtlas optAtlas = LoadAtlas("assets/atlases/opt.rtpa");
    TextureAtlas froggitAtlas = LoadAtlas("assets/atlases/froggit.rtpa");

    int froggitFrame = 0;

    const int animStep = 2;

    int curAnimStep = 0;


    while (!WindowShouldClose()) {
        BeginTextureMode(target);
        UpdateMusicStream(music);
        ClearBackground(gbDark);
        BeginDrawing();

        DrawTexture(bg, 0, 0, WHITE);
        DrawTexture(textBox, 7, 78, WHITE);
        DrawAtlasSprite(optAtlas, "fight", (Vector2){7, 124});
        DrawAtlasSprite(optAtlas, "act", (Vector2){46, 124});
        DrawAtlasSprite(optAtlas, "item", (Vector2){86, 124});
        DrawAtlasSprite(optAtlas, "mercy", (Vector2){125, 124});
        DrawAtlasSprite(froggitAtlas, froggitAtlas.sprites[froggitFrame].name, (Vector2){50, 22});
        if (froggitFrame == froggitAtlas.spriteCount - 1) {froggitFrame = 0;}
        else if (animStep == curAnimStep) {
            froggitFrame++;
            curAnimStep = 0;
        }
        else {curAnimStep++;}
        // Draw render texture to the screen at a scale relative to the window size
        EndTextureMode();
        Rectangle screenRect = {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
        DrawTexturePro(target.texture, (Rectangle){0, 0, (float)target.texture.width, -(float)target.texture.height}, screenRect, (Vector2){0, 0}, 0, WHITE);
        EndDrawing();
    }
    UnloadMusicStream(music);
    UnloadRenderTexture(target);
    CloseAudioDevice();
    CloseWindow();
}