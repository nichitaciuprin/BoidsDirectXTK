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
    while(true)
    {
        if (Helper::EscapePressed()) break;

        long time1 = clock();

        auto directionWASD = worldWindow->DirectionWASD();
        world->Update(0.02f,directionWASD,Vector2::Zero);
        worldWindow->Render(world.get());

        long time2 = clock();
        long diff = time2 - time1;
        long timeStep = 20;
        long waitTime = timeStep - diff;
        Sleep(waitTime);
    }
}