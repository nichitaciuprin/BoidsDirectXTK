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
    Quaternion cameraRotation = Quaternion::Identity;
    Vector2 mousePosition = Vector3::Zero;
    World()
    {
    }
    void Update(float deltaTime, Vector2 wasdDirection, Vector2 mousePositionDelta)
    {
        mousePosition += mousePositionDelta;
        cameraRotation = Quaternion::CreateFromYawPitchRoll(Helper::ToRadian(-mousePosition.x),Helper::ToRadian(mousePosition.y),0);
        if (wasdDirection == Vector2::Zero) return;
        auto wasdDirection2 = Vector3(wasdDirection.x,0,-wasdDirection.y);
        auto direction = Vector3::Transform(wasdDirection2,cameraRotation);
        auto speed = 10.0f;
        cameraPosition += direction * deltaTime * speed;
    }
private:
};