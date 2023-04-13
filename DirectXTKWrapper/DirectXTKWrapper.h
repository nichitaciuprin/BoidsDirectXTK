#pragma once

#include <Windows.h>
#include <DirectXMath.h>
#include <time.h>
#include "Window.h"
#include "SimpleMath.h"
#include "Vector3Ext.h"
#include "AABB.h"
#include "Subgen.h"

// using namespace std;

class XTKW
{
public:
    static int Initialise()  // Must be called before XTKW is used
    {
        // asserts that DirectXMath can be used
        if (!DirectX::XMVerifyCPUSupport()) return 1;

        // Initializes the COM library for use by the calling thread, sets the thread's concurrency model,
        // and creates a new apartment for the thread if one is required.
        if (FAILED(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED))) return 1;

        return 0;
    }
    static void Finalize()   // Must be called after  XTKW is used
    {
        // Closes the COM library on the current thread, unloads all DLLs loaded by the thread,
        // frees any other resources that the thread maintains, and forces all RPC connections on the thread to close.
        CoUninitialize();
    }

    static long GetTime()                                            // Returns curent time in milliseconds
    {
        return clock();
    }
    static void Wait(long milliseconds)                              // Suspends thread for milliseconds
    {
        if (milliseconds < 0) return;
        Sleep(milliseconds);
    }
    static void WaitLoop(long oldTime, long newTime, long timeStep)  // Suspends thread for time from loop
    {
        auto diff = newTime - oldTime;
        long waitTime = timeStep - diff;
        Wait(waitTime);
    }

    static float RandomFractionUnsigned()           // returns value (inclusive) from  0f to  1f
    {
        return tempSubgen.FractionUnsigned();
    }
    static float RandomFractionSigned()             // returns value (inclusive) from -1f to  1f
    {
        return tempSubgen.FractionSigned();
    }
    static float RandomRange(float min, float max)  // returns value (inclusive) from min to max
    {
        return tempSubgen.Range(min,max);
    }

    static bool EscapePressed()  // Returns true if escape button pressed, otherwise false
    {
        auto keyState = GetKeyState(VK_ESCAPE);
        auto pressed = keyState & 0x8000;
        return pressed;
    }

    static void ConsoleClose();
    static void ConsoleInit(int x, int y, int width, int height);
    static void ConsoleWriteLine(string msg);

    static void WindowClose();
    static void WindowInit(int x, int y, int width, int height)
    {
        if (tempWindow != nullptr) return;
        auto hInstance = GetModuleHandle(NULL);
        tempWindow = std::make_unique<Window>(hInstance,"BoidsDirectXTK",x,y,width,height);
    }
    static void WindowInitDefault()
    {
        WindowInit(0,0,800,600);
    }
    static void WindowInitFullscreen();
    static void WindowInitFullscreenMonitor(int monitorIndex);
    static void WindowUpdate()
    {
        tempWindow->Update();
    }
    static void WindowClear()
    {
        tempWindow->Clear();
    }
    static void WindowRenderStart()
    {
        tempWindow->PaintStart();
    }
    static void WindowRenderEnd()
    {
        tempWindow->PaintEnd();
    }
    static void WindowSetCamera(Vector3 position, Vector3 target, Vector3 up)
    {
        tempWindow->PaintSetCamera(position, target, up);
        tempWindow->PaintSetPerpective();
    }
    static void WindowDrawGround()
    {
        tempWindow->PaintGround();
    }
    static void WindowDrawSphere(const Vector3& position)
    {
        tempWindow->PaintSphere(position);
    }

    static float ToDegree(float radian)
    {
        return XMConvertToDegrees(radian);
    }
    static float ToRadian(float degree)
    {
        return XMConvertToRadians(degree);
    }
    static Vector3 ToDirection(Vector3 euler)
    {
        auto quaternion = Quaternion::CreateFromYawPitchRoll(-euler);
        Vector3 result = Vector3::Transform(Vector3::Forward,quaternion);
        return result;
    }
    static float NormaliseDegree(float degree)
    {
        degree = remainderf(degree,360);
        if (degree < 0) degree = 360 + degree;
        return degree;
    }
    static Quaternion ToRotation(Vector2 mousePosition)
    {
        mousePosition.y = clamp(mousePosition.y,-89.0f,89.0f);
        auto xRadian = ToRadian(-mousePosition.x);
        auto yRadian = ToRadian(mousePosition.y);
        return Quaternion::CreateFromYawPitchRoll(xRadian,yRadian,0);
    }

private:
    static unique_ptr<Window> tempWindow;
    static Subgen tempSubgen;
};
unique_ptr<Window> XTKW::tempWindow = nullptr;
Subgen XTKW::tempSubgen = Subgen(0);








