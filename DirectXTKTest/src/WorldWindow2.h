#pragma once
#include "World.h"
#include "WorldWindow.h"

using namespace std;
using namespace DirectX::SimpleMath;

class WorldWindow2
{
public:
    WorldWindow2(HINSTANCE hInstance)
    {
        worldWindow = make_unique<WorldWindow>(hInstance);
    };
    void Render(const World* game)
    {
        worldWindow->Render(game);
    }
    bool Closed()
    {
        return worldWindow->windowClosed;
    }
    Vector2 DirectionWASD()
    {
        return worldWindow->DirectionWASD();
    }
private:
    unique_ptr<WorldWindow> worldWindow;
};
