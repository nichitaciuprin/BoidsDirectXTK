#pragma once

#include <Windows.h>
#include <DirectXMath.h>
#include <time.h>
#include "Window.h"
#include "SimpleMath.h"
#include "Vector3Ext.h"
#include "AABB.h"
#include "Subgen.h"

class XTKW
{
public:
    static int Initialise()
    {
        // asserts that DirectXMath can be used
        if (!DirectX::XMVerifyCPUSupport()) return 1;

        // Initializes the COM library for use by the calling thread, sets the thread's concurrency model,
        // and creates a new apartment for the thread if one is required.
        if (FAILED(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED))) return 1;

        return 0;
    }
    static void Finalize()
    {
        // Closes the COM library on the current thread, unloads all DLLs loaded by the thread,
        // frees any other resources that the thread maintains, and forces all RPC connections on the thread to close.
        CoUninitialize();
    }
    static long GetTime()
    {
        return clock();
    }
    static void Wait(long milliseconds)
    {
        if (milliseconds < 0) return;
        Sleep(milliseconds);
    }
    static void WaitLoop(long oldTime, long newTime, long timeStep)
    {
        auto diff = newTime - oldTime;
        long waitTime = timeStep - diff;
        Wait(waitTime);
    }
    static float RandomFractionUnsigned()
    {
        return tempSubgen.FractionUnsigned();
    }
    static float RandomFractionSigned()
    {
        return tempSubgen.FractionSigned();
    }
    static float RandomRange(float min, float max)
    {
        return tempSubgen.Range(min,max);
    }
    static bool EscapePressed()
    {
        auto keyState = GetKeyState(VK_ESCAPE);
        auto pressed = keyState & 0x8000;
        return pressed;
    }
    static void ConsoleInit(int x, int y, int width, int height)
    {
        Console::Init();
        Console::SetPosition(x,y,width,height);
        Console::Show();
    }
    static void ConsoleClose()
    {
        Console::Close();
    }
    static void ConsoleWriteLine(string msg)
    {
        cout << msg << '\n';
    }
    static void WindowInit(int x, int y, int width, int height)
    {
        if (tempWindow != nullptr) return;
        auto hInstance = GetModuleHandle(NULL);
        tempWindow = std::make_unique<Window>(hInstance,"BoidsDirectXTK",x,y,width,height);
    }
    static void WindowInitDefault()
    {
        if (tempWindow != nullptr) return;
        auto hInstance = GetModuleHandle(NULL);
        tempWindow = std::make_unique<Window>(hInstance,"BoidsDirectXTK",0,0,800,600);
    }
    static void WindowInitFullscreen()
    {
        if (tempWindow != nullptr) return;
        auto hInstance = GetModuleHandle(NULL);
        tempWindow = std::make_unique<Window>(hInstance,"BoidsDirectXTK",0,0,800,600);
        tempWindow->ToFullscreen();
    }
    static void WindowClose()
    {
        if (tempWindow == nullptr) return;
        auto hInstance = GetModuleHandle(NULL);
        tempWindow = std::make_unique<Window>(hInstance,"BoidsDirectXTK",0,0,800,600);
    }
    static void WindowUpdate()
    {
        if (tempWindow == nullptr) return;
        tempWindow->Update();
    }
    static void WindowClear()
    {
        if (tempWindow == nullptr) return;
        tempWindow->Clear();
    }
    static void WindowRenderStart()
    {
        if (tempWindow == nullptr) return;
        tempWindow->PaintStart();
    }
    static void WindowRenderEnd()
    {
        if (tempWindow == nullptr) return;
        tempWindow->PaintEnd();
    }
    static void WindowSetCamera(Vector3 position, Vector3 target, Vector3 up)
    {
        if (tempWindow == nullptr) return;
        tempWindow->PaintSetCamera(position, target, up);
        tempWindow->PaintSetPerpective();
    }
    static void WindowDrawGround()
    {
        if (tempWindow == nullptr) return;
        tempWindow->PaintGround();
    }
    static void WindowDrawSphere(const Vector3& position)
    {
        if (tempWindow == nullptr) return;
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