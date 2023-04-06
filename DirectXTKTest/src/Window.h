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
    void Render(const World* game)
    {
        WindowPrivate::Render(game);
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
        return WindowPrivate::MouseLook();
    }
}