#include <raylib.h>
#include <stdio.h>
#include <raymath.h>

namespace gameplay {
    int attackBarX;
    void initAttack(){
        attackBarX = 150;
    }
    void drawAttackBar(){
        DrawRectangle(attackBarX, 79, 2, 34, GetColor(0x9BBC0FFF));
        attackBarX -= 2;
    }
    int attackMechanic(bool attackInput){
        std::cout << TextFormat("%d", attackBarX) << std::endl;
        if (attackBarX < 8) {
            return 1;
        }
        if (attackInput) {
            return 2;
        }
        return 0;
    }
    int damageEnemy(){
        int accuracy = Clamp(floor(abs(attackBarX - 80)), 10, 80);
        if (accuracy == 10)
        {
            return 25;
        }
        return 10 + (25 / (1 + accuracy));
    }
}