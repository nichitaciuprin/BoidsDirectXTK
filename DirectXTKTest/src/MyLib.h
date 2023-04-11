#pragma once

#include <Windows.h>
#include <DirectXMath.h>
#include "WindowPrivate.h"
#include "Helper.h"
#include "World.h"
#include <time.h>

int StartMyLib()                            // Must be called before this library use
{
    // asserts that DirectXMath can be used
    if (!DirectX::XMVerifyCPUSupport()) return 1;

    // Initializes the COM library for use by the calling thread, sets the thread's concurrency model,
    // and creates a new apartment for the thread if one is required.
    if (FAILED(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED))) return 1;

    return 0;
}
void EndMyLib()                             // Must be called after  this library use
{
    // Closes the COM library on the current thread, unloads all DLLs loaded by the thread,
    // frees any other resources that the thread maintains, and forces all RPC connections on the thread to close.
    CoUninitialize();
}

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

bool EscapePressed()    // Returns true if escape button pressed, otherwise false
{
    auto keyState = GetKeyState(VK_ESCAPE);
    auto pressed = keyState & 0x8000;
    return pressed;
}

void ConsoleClose();
void ConsoleInit(int x, int y, int width, int height);
void ConsoleWriteLine(string msg);

void WindowClose();
void WindowInit(int x, int y, int width, int height)
{
    auto hInstance = GetModuleHandle(NULL);
    WindowPrivate::Create(hInstance,x,y,width,height);
}
void WindowInitDefault()
{
    WindowInit(0,0,800,600);
}
void WindowInitFullscreen();
void WindowInitFullscreenMonitor(int monitorIndex);
void WindowClear();
void WindowRenderStart();
void WindowRenderEnd();
void WindowSetCamera(Vector3 position, Vector3 target, Vector3 up)
{
    // WindowPrivate::PaintSetCamera(world->cameraPosition, world->cameraTarget, world->cameraUp);
}
void WindowDrawAABB(const AABB& aabb);
void WindowDrawSphere(const Vector3& position, float radius);