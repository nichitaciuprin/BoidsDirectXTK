#pragma once
#include <Windows.h>
#include <time.h>

namespace Helper
{
    bool EscapePressed()
    {
        // https://stackoverflow.com/questions/41600981/how-do-i-check-if-a-key-is-pressed-on-c
        auto keyState = GetKeyState(VK_ESCAPE);
        auto pressed = keyState & 0x8000;
        // auto toggled = keyState & 0x0001;
        return pressed;
    }
    long GetTime()
    {
        return clock();
    }
    void Wait(long oldTime, long newTime, long timeStep)
    {
        auto diff = newTime - oldTime;
        long waitTime = timeStep - diff;
        if (waitTime < 0) return;
        Sleep(waitTime);
    }
}