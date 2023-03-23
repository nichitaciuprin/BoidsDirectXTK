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
        cameraPosition += Vector3(wasdDirection.x,wasdDirection.y,0) * deltaTime;
    }
private:
};