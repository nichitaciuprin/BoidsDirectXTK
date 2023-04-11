#pragma once

#include <Windows.h>
#include <DirectXMath.h>
#include "Window.h"
#include "Helper.h"
#include "World.h"
#include <time.h>

long GetTime()                                            // Returns curent time in milliseconds
{
    return clock();
}
void Wait(long milliseconds)                              // Suspends thread for milliseconds
{
    if (milliseconds < 0) return;
    Sleep(milliseconds);
}
void WaitLoop(long oldTime, long newTime, long timeStep)  // Suspends thread for time from loop
{
    auto diff = newTime - oldTime;
    long waitTime = timeStep - diff;
    Wait(waitTime);
}
{
    auto keyState = GetKeyState(VK_ESCAPE);
    auto pressed = keyState & 0x8000;
    return pressed;
}