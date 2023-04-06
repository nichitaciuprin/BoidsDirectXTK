#pragma once
#include "World.h"
#include "DeviceResources.h"
#include "WindowPrivate.h"
using namespace DX;
using namespace DirectX;
using namespace DirectX::SimpleMath;

namespace Window
{
    void Create(HINSTANCE hInstance)
    {
        WindowPrivate::Create(hInstance);
    }
    void Render(const World* world)
    {
        WindowPrivate::HandleWindowMessages();
        WindowPrivate::Clear();
        WindowPrivate::Paint(world);
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
        auto state = WindowPrivate::mouse->GetState();
        return Vector2((float)state.x,(float)-state.y);
    }
}