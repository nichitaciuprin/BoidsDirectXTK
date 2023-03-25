#pragma once
#include "pch.h"
using namespace DirectX::SimpleMath;

class World
{
public:
    Vector3 cameraPosition = Vector3::Backward*5;
    Vector3 cameraRotation = Vector3::Zero;
    World()
    {
    }
    void Update(float deltaTime, Vector2 wasdDirection, Vector2 mouseRotation)
    {
        auto speed = 10.0f;
        cameraPosition += Vector3(wasdDirection.x,wasdDirection.y,0) * speed * deltaTime;
        cameraRotation += Vector3(mouseRotation.x,mouseRotation.y,0);
    }
private:
};