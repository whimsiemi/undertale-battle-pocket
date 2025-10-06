#include <raylib.h>

namespace input {
    int getInput(const char *action)
    {
        if (!strcmp(action, "left")) {return IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT) || GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) <= -0.5f;}
        else if (!strcmp(action, "right")) {return IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT) || GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) >= 0.5f;}
        else if (!strcmp(action, "select")) {return IsKeyPressed(KEY_Z) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);}
        else {return 0;}  
    }
}