#pragma once
#include "World.h"
#include "DeviceResources.h"
#include "MyWindow.h"
using namespace DX;
using namespace DirectX;
using namespace DirectX::SimpleMath;

class MyWindow2
{
public:
    static void Create(HINSTANCE hInstance)
    {
        MyWindow::Create(hInstance);
    }
    static void Render(const World* game)
    {
        MyWindow::Render(game);
    }
    static bool Closed()
    {
        return MyWindow::windowClosed;
    }
    static Vector2 DirectionWASD()
    {
        return MyWindow::DirectionWASD();
    }
    static Vector2 MouseLook()
    {
        return MyWindow::MouseLook();
    }
};