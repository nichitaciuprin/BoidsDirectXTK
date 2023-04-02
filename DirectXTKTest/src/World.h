#pragma once
#include "pch.h"
#include "Console.h"
using namespace DirectX::SimpleMath;
using namespace std;

class World
{
public:
    // Transform duno;
    Vector3 cameraPosition = Vector3::Zero;
    Vector3 cameraRotation = Vector3::Zero;
    World()
    {
    }
    void Update(float deltaTime, Vector2 wasdDirection, Vector2 mousePositionDelta)
    {
        auto speed = 10.0f;

        cameraRotation += Vector3(-mousePositionDelta.y,mousePositionDelta.x,0)/10;

        // cameraPosition

        // cameraPosition += Vector3(wasdDirection.x,0,-wasdDirection.y) * speed * deltaTime;
        // Console_WriteLine(to_string(direction.x));
        auto direction = ToDirection(cameraRotation);
        if (wasdDirection != Vector2::UnitY) return;
        cameraPosition += direction * deltaTime;

        // auto direction1 = wasdDirection;
        // auto direction2 = ToDirection(cameraRotation);
        // cameraPosition += cameraPosition + ToDirection(cameraRotation);
    }
private:
    Vector3 ToDirection(Vector3 euler)
    {
        auto xDegree = Helper::ToRadian(euler.x);
        auto yDegree = Helper::ToRadian(euler.y);
        auto vec1 = Vector3(0,-sin(xDegree),-cos(xDegree));
        auto vec2 = Vector3(sin(yDegree),0,-cos(yDegree));
        // auto vec2 = Vector3(0,0,0);
        auto vec3 = vec1+vec2;
        // auto vec3 = vec2;
        vec3.Normalize();
        return vec3;
    }
};