#pragma once
#include <Windows.h>

class Helper
{
public:
    static bool EscapePressed()
    {
        // https://stackoverflow.com/questions/41600981/how-do-i-check-if-a-key-is-pressed-on-c
        auto keyState = GetKeyState(VK_ESCAPE);
        auto pressed = keyState & 0x8000;
        // auto toggled = keyState & 0x0001;
        return pressed;
    }
};