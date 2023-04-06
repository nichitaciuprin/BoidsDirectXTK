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
        return WindowPrivate::DirectionWASD();
    }
    Vector2 MouseLook()
    {
        return WindowPrivate::MouseLook();
    }
}