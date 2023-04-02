#pragma once
#include "pch.h"
#include "Console.h"
using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

class World
{
public:
    Vector3 cameraPosition = Vector3::Zero;
    Vector3 cameraRotation = Vector3::Zero;
    Vector2 mousePosition = Vector3::Zero;
    World()
    {
    }
    void Update(float deltaTime, Vector2 wasdDirection, Vector2 mousePositionDelta)
    {
        mousePosition += mousePositionDelta;
        cameraRotation = Vector3(-mousePosition.y,mousePosition.x,0)/100;
        if (wasdDirection == Vector2::Zero) return;
        auto quat = Quaternion::CreateFromYawPitchRoll(-cameraRotation);
        auto wasdDirection2 = Vector3(wasdDirection.x,0,-wasdDirection.y);
        auto direction = Vector3::Transform(wasdDirection2,quat);
        auto speed = 10.0f;
        cameraPosition += direction * deltaTime * speed;
    }
private:
};