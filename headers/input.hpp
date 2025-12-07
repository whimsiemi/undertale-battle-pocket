#include <raylib.h>

namespace input {

    // Polls inputs for certain gameplay actions
    int getInput(const char *action)
    {
        if (!strcmp(action, "left")) {return IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT) || GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) <= -0.5f;}
        else if (!strcmp(action, "right")) {return IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT) || GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) >= 0.5f;}
        else if (!strcmp(action, "up")) {return IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_UP) || GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y) <= -0.5f;}
        else if (!strcmp(action, "down")) {return IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN) || GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y) >= 0.5f;}
        else if (!strcmp(action, "select")) {return IsKeyPressed(KEY_Z) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);}
        else if (!strcmp(action, "back")) {return IsKeyPressed(KEY_X) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT);}
        else if (!strcmp(action, "restart")) {return IsKeyPressed(KEY_R) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_LEFT);}
        else {return 0;}  
    }
}