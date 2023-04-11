#pragma once
#include "World.h"
#include "DeviceResources.h"
#include "WindowPrivate.h"
using namespace DX;
using namespace DirectX;
using namespace DirectX::SimpleMath;

namespace Window
{
    void Render(const World* world)
    {
        WindowPrivate::Update();
        WindowPrivate::Clear();

        WindowPrivate::PaintStart();

        WindowPrivate::PaintSetCamera(world->cameraPosition, world->cameraTarget, world->cameraUp);
        WindowPrivate::PaintSetPerpective();

        WindowPrivate::PaintGround();

        auto length = world->boidWorld.boids.size();
        for (size_t i = 0; i < length; i++)
        {
            auto boid = &world->boidWorld.boids[i];
            WindowPrivate::PaintSphere(boid->pos);
        }

        WindowPrivate::PaintEnd();
    }
    bool Closed()
    {
        return WindowPrivate::windowClosed;
    }
    Vector2 DirectionWASD()
    {
        auto axisY = WindowPrivate::key_w + WindowPrivate::key_s;
        auto axisX = WindowPrivate::key_a + WindowPrivate::key_d;
        auto result = Vector2(axisX, axisY);
        result.Normalize();
        return result;
    }
    Vector2 MouseLook()
    {
        return Vector2::Zero;
        // auto state = WindowPrivate::mouse->GetState();
        // return Vector2((float)state.x,(float)-state.y);
    }
}