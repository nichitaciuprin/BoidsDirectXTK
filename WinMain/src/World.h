#pragma once

#include "DirectXTKWrapper.h"
#include "BoidWorld.h"

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
        XTKW::WindowInitDefault();
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
        XTKW::WindowUpdate();
        XTKW::WindowClear();
        XTKW::WindowRenderStart();
        XTKW::WindowSetCamera(cameraPosition, cameraTarget, cameraUp);
        XTKW::WindowDrawGround();
        auto length = boidWorld.boids.size();
        for (size_t i = 0; i < length; i++)
        {
            auto boid = &boidWorld.boids[i];
            XTKW::WindowDrawSphere(boid->pos);
        }
        XTKW::WindowRenderEnd();
    }
};