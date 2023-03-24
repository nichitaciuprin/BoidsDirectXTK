#pragma once
#include "pch.h"
#include "World.h"
#include "WorldWindow.h"
#include "WorldWindow2.h"
#include "Helper.h"
#include "console.h"
#include <time.h>
#include <iostream>

class Test
{
public:
    Test()
    {
    }
private:
};

void wWinMain2(HINSTANCE hInstance)
{
    // Console_Test();
    auto world = make_unique<World>();
    auto worldWindow2 = make_unique<WorldWindow2>(hInstance);
    while(true)
    {
        if (Helper::EscapePressed()) break;

        long oldTime = clock();

        auto directionWASD = worldWindow2->DirectionWASD();
        world->Update(0.02f,directionWASD,Vector2::Zero);
        worldWindow2->Render(world.get());

        long newTime = clock();

        long diff = newTime - oldTime;
        long timeStep = 20;
        long waitTime = timeStep - diff;
        if (waitTime < 0) continue;
        Sleep(waitTime);
    }
}