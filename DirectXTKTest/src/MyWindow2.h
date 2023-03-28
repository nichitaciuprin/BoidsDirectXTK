#pragma once
#include "World.h"
#include "DeviceResources.h"
#include "MyWindow.h"
using namespace DX;
using namespace DirectX;
using namespace DirectX::SimpleMath;

namespace MyWindow2
{
    void Create(HINSTANCE hInstance)
    {
        MyWindow::Create(hInstance);
    }
    void Render(const World* game)
    {
        MyWindow::Render(game);
    }
    bool Closed()
    {
        return MyWindow::windowClosed;
    }
    Vector2 DirectionWASD()
    {
        return MyWindow::DirectionWASD();
    }
    Vector2 MouseLook()
    {
        return MyWindow::MouseLook();
    }
}