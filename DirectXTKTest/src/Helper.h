#pragma once
#include <Windows.h>
#include <time.h>
#include <cmath>
// using namespace std;

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
    float ToDegree(float radian)
    {
        auto pi = 3.14159;
        return radian * (180 / pi);
    }
    float ToRadian(float degree)
    {
        auto pi = 3.14159;
        return degree * (pi / 180);
    }
    // float NormaliseDegree(float degree)
    // {
    //     degree = remainderf(degree,360);
    //     if (degree < 0) degree = 360 + degree;
    //     return degree;
    // }
}