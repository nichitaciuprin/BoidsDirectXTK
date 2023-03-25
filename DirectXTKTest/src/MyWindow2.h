#pragma once
#include "pch.h"
#include "World.h"
#include "DeviceResources.h"
#include "MyWindow.h"
using namespace DX;
using namespace DirectX;
using namespace DirectX::SimpleMath;

namespace MyWindow2
{
    void Render(const World* game)
    {
        // MyWindow::Create(hInstance);
    }
    bool Closed()
    {
        return false;
        // return MyWindow::windowClosed;
    }
    Vector2 DirectionWASD()
    {
        return Vector2::Zero;
        // return MyWindow::DirectionWASD();
    }
}
