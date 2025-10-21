#include <raylib.h>
#include <stdio.h>
#include <raymath.h>
#include "headers/funcs.hpp"

namespace gameplay {
    int attackBarX;

    class Bullet {
        public:
            float speed = funcs::GetRandomFloat(0.4f, 0.65f);
            Vector2 position = {GetRandomValue(0, 1) ? 64 : 93, GetRandomValue(0, 1) ? 80 : 109};
            Vector2 direction = {
                position.x == 64 ? funcs::GetRandomFloat(0.2f, 1.0f) : funcs::GetRandomFloat(-1.0f, -0.2f),
                position.y == 80 ? funcs::GetRandomFloat(0.2f, 1.0f) : funcs::GetRandomFloat(-1.0f, -0.2f)
            };
            Texture2D bulletTex = LoadTexture("assets/imgs/bullet.png");
            void BulletLogic() {
                position += Vector2Normalize(direction) * speed;
                DrawTexture(bulletTex, position.x, position.y, WHITE);
            }
            int ShouldDeleteBullet() {
                return !(position.x >= 64 && position.x <= 93 && position.y >= 80 && position.y <= 109);
            }
            int BulletCollision(Vector2 player) {
                return CheckCollisionRecs(Rectangle{player.x + 1, player.y + 1, 4, 4}, Rectangle{this->position.x, this->position.y, 3, 3});
            }
    };

    // Literally just resets the moving bar's x value
    void InitAttack(){
        attackBarX = 150;
    }

    // Draws the moving bar that indicates hit timing
    void DrawAttackBar(bool hasAttacked){
        DrawRectangle(attackBarX - 1, 79, 2, 34, GetColor(0x9BBC0FFF));
        if (!hasAttacked) {attackBarX -= 2;}
    }

    // Check for if player has inputted or the timing window has been exceeded
    int AttackMechanic(bool attackInput){
        if (attackBarX < 8) {
            return 1;
        }
        if (attackInput) {
            return 2;
        }
        return 0;
    }
    // Returns the amount of damage that the enemy should take based on the hit accuracy
    int DamageEnemy(float mult){
        int accuracy = floor((80 - abs(attackBarX - 80)) * 1.25f);
        if (accuracy >= 90)
        {
            return floor(25 * mult);
        }
        return floor((accuracy / 4) * mult);
    }
}