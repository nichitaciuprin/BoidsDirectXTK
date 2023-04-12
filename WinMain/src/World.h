#pragma once
#include "Console.h"
#include "BoidWorld.h"
#include "MyLib.h"
using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

class World
{
public:
    Vector3 cameraPosition;
    Vector3 cameraTarget;
    Vector3 cameraUp;
    BoidWorld boidWorld;
    World()
    {
        cameraUp = Vector3::Up;
        cameraPosition += Vector3::Up*110;
        cameraPosition += Vector3::Right*175;
        cameraPosition += Vector3::Backward*175;
        cameraTarget = Vector3::Up*50;
    }
    void Update(float deltaTime, Vector2 wasdDirection, Vector2 mousePositionDelta)
    {
        UNREFERENCED_PARAMETER(wasdDirection);
        UNREFERENCED_PARAMETER(mousePositionDelta);
        auto rotation = Quaternion::CreateFromAxisAngle(Vector3::Up,deltaTime/2);
        cameraPosition = Vector3::Transform(cameraPosition,rotation);
        boidWorld.Update(deltaTime);
    }
    void Render()
    {
        WindowUpdate();
        WindowClear();
        WindowRenderStart();
        WindowSetCamera(cameraPosition, cameraTarget, cameraUp);
        WindowDrawGround();
        auto length = boidWorld.boids.size();
        for (size_t i = 0; i < length; i++)
        {
            auto boid = &boidWorld.boids[i];
            WindowDrawSphere(boid->pos);
        }
        WindowRenderEnd();
    }
};