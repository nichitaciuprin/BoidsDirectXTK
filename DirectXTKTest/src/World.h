#pragma once
#include "pch.h"
using namespace DirectX::SimpleMath;

class World
{
public:
    Vector3 cameraPosition = Vector3::Backward*5;
    Vector2 direction;
    World()
    {
    }
    void Update(float deltaTime, Vector2 wasdDirection, Vector2 mouseRotation)
    {
        // SHORT state = GetKeyState(VK_SHIFT);
        // SHORT state2 = GetKeyState(VK_SHIFT) & 0x8000;
        // SHORT test = 0x0001;
        // auto isHightOrderBit = (GetKeyState(VK_SHIFT) >> 15);
        // if (GetKeyState(VK_SHIFT) & 0x8000)
        // if(isHightOrderBit)
        // {
        //     // https://stackoverflow.com/questions/41600981/how-do-i-check-if-a-key-is-pressed-on-c
        //     cameraPosition += Vector3::Right*deltaTime;
        // }
        cameraPosition += Vector3(wasdDirection.x,wasdDirection.y,0) * deltaTime;
    }
private:
};