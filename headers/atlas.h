#include <iostream>
#include <string>

// Thank you to the internet for making this so much easier <3
namespace atlas {
    typedef struct {
        char name[64];
        Rectangle sourceRec;
    } SpriteInfo;
    
    typedef struct {
        Texture2D texture;
        SpriteInfo sprites[32];
        int spriteCount;
    } TextureAtlas;

    class AnimAtlas {
        public:
            TextureAtlas atlas;
            int curFrame;
            int stepsToAnim;
            int curStep;
    };

    // Function for loading texture atlases by parsing the parameters of the defined rtpa file
    TextureAtlas LoadAtlas(const char *rtpaPath) {
        // Initalize atlas
        TextureAtlas atlas = {0};

        // Open the file specified in the function argument
        FILE *file = fopen(rtpaPath, "r");

        char line[256];
        char imagePath[128] = {0};

        // This while loops gets the path of the image and the index of its sprites by reading the lines with "a" and "s" prefixes
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

    // Function for drawing sprites defined within an atlas
    void DrawAtlasSprite(TextureAtlas atlas, const char *spriteName, Vector2 position) {
        // Find the sprite within the atlas's sprite array and draw it if found
        for (int i = 0; i < atlas.spriteCount; i++) {
            if (strcmp(atlas.sprites[i].name, spriteName) == 0) {
                DrawTextureRec(atlas.texture, atlas.sprites[i].sourceRec, position, WHITE);
                break;
            }
        }
    }

    // Function for handling atlases that contain animated sprites
    void AnimateAtlasSprite(AnimAtlas& anim, const char *animName, Vector2 position) {
        std::string nextAnimName = anim.atlas.sprites[anim.curFrame + 1].name;
        while (nextAnimName != "") {
            if (std::isdigit(nextAnimName.back())) {nextAnimName.pop_back();}
            else {break;}
        }
        if (anim.curStep == anim.stepsToAnim) {
            if (anim.curFrame != anim.atlas.spriteCount - 1 && strcmp(nextAnimName.c_str(), animName) == 0) {anim.curFrame++;}
            else {anim.curFrame = 0;}
            anim.curStep = 0;
        } else {anim.curStep++;}
        
        DrawTextureRec(anim.atlas.texture, anim.atlas.sprites[anim.curFrame].sourceRec, position, WHITE);
    }
}