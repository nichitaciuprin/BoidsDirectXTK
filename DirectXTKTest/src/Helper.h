#pragma once
#include <Windows.h>
#include <time.h>
#include <cmath>
#include "pch.h"
#include "Console.h"
using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

namespace Helper
{
    bool EscapePressed()
    {
        auto keyState = GetKeyState(VK_ESCAPE);
        auto pressed = keyState & 0x8000;
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
        auto pi = 3.14159f;
        return radian * (180 / pi);
    }
    float ToRadian(float degree)
    {
        auto pi = 3.14159f;
        return degree * (pi / 180);
    }
    Vector3 ToDirection(Vector3 euler)
    {
        auto quaternion = Quaternion::CreateFromYawPitchRoll(-euler);
        Vector3 result = Vector3::Transform(Vector3::Forward,quaternion);
        return result;
    }
    float NormaliseDegree(float degree)
    {
        degree = remainderf(degree,360);
        if (degree < 0) degree = 360 + degree;
        return degree;
    }
    Quaternion ToRotation(Vector2 mousePosition)
    {
        mousePosition.y = std::clamp(mousePosition.y,-89.0f,89.0f);
        auto xRadian = Helper::ToRadian(-mousePosition.x);
        auto yRadian = Helper::ToRadian(mousePosition.y);
        return Quaternion::CreateFromYawPitchRoll(xRadian,yRadian,0);
    }
}