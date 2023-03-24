#pragma once
#include "pch.h"
#include "World.h"
#include "WorldWindow.h"
#include "Helper.h"
#include "console.h"
#include <time.h>

void wWinMain2(HINSTANCE hInstance)
{
    auto world = make_unique<World>();
    auto worldWindow = make_unique<WorldWindow>(hInstance);
    Console_Test();
    while(true)
    {
        if (Helper::EscapePressed()) break;

        long oldTime = clock();

        auto directionWASD = worldWindow->DirectionWASD();
        world->Update(0.02f,directionWASD,Vector2::Zero);
        worldWindow->Render(world.get());

        long newTime = clock();

        long diff = newTime - oldTime;
        long timeStep = 20;
        long waitTime = timeStep - diff;
        if (waitTime < 0) continue;
        Sleep(waitTime);
    }
}