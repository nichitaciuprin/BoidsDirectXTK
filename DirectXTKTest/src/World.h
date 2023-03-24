#pragma once
#include "pch.h"
using namespace DirectX::SimpleMath;

class World
{
public:
    Vector3 cameraPosition = Vector3::Backward*5;
    World()
    {
    }
    void Update(float deltaTime, Vector2 wasdDirection, Vector2 mouseRotation)
    {
        auto speed = 10.0f;
        cameraPosition += Vector3(wasdDirection.x,wasdDirection.y,0) * speed * deltaTime;
    }
private:
};