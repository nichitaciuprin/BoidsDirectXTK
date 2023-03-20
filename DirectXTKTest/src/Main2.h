#pragma once
#include "pch.h"
#include "World.h"
#include "WorldWindow.h"

void wWinMain2(HINSTANCE hInstance, int nCmdShow)
{
    auto world = make_unique<World>();
    auto worldWindow = make_unique<WorldWindow>(hInstance,nCmdShow);

    while(true)
    {
        if (worldWindow->quit) break;

        world->Update(0.02f);

        worldWindow->Render();
    }

    worldWindow.reset();
}