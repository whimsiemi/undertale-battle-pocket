#include <raylib.h>
#include <stdio.h>
#include <raymath.h>

namespace gameplay {
    int attackBarX;
    
    // Literally just resets the moving bar's x value
    void initAttack(){
        attackBarX = 150;
    }

    // Draws the moving bar that indicates hit timing
    void drawAttackBar(bool hasAttacked){
        DrawRectangle(attackBarX - 1, 79, 2, 34, GetColor(0x9BBC0FFF));
        if (!hasAttacked) {attackBarX -= 2;}
    }

    // Check for if player has inputted or the timing window has been exceeded
    int attackMechanic(bool attackInput){
        if (attackBarX < 8) {
            return 1;
        }
        if (attackInput) {
            return 2;
        }
        return 0;
    }
    // Returns the amount of damage that the enemy should take based on the hit accuracy
    int damageEnemy(){
        int accuracy = (GetScreenWidth() / 2 - floor(abs(attackBarX - GetScreenWidth() / 2))) * 100 / (GetScreenWidth() / 2);
        if (accuracy >= 90)
        {
            return 25;
        }
        return accuracy / 4;
    }
}