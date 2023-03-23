#pragma once
#include "pch.h"
#include "World.h"
#include "WorldWindow.h"
#include "console.h"

void wWinMain2(HINSTANCE hInstance)
{
    auto world = make_unique<World>();
    auto worldWindow = make_unique<WorldWindow>(hInstance);
    while(!worldWindow->quit)
    {
        world->Update(0.02f);
        worldWindow->Render(world.get());
    }
}