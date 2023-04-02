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
        // auto xDegree = Helper::ToRadian(euler.x);
        // auto yDegree = Helper::ToRadian(euler.y);

        auto quaternion = Quaternion::CreateFromYawPitchRoll(-euler);
        Vector3 result = Vector3::Transform(Vector3::Forward,quaternion);
        // Console_WriteLine(to_string(result.z));
        return result;

        // auto vec1 = Vector3(0,-sin(xDegree),-cos(xDegree));
        // auto vec2 = Vector3(sin(yDegree),0,-cos(yDegree));
        // auto vec3 = vec1+vec2;
        // vec3.Normalize();
        // return vec3;
    }
    // float NormaliseDegree(float degree)
    // {
    //     degree = remainderf(degree,360);
    //     if (degree < 0) degree = 360 + degree;
    //     return degree;
    // }
}