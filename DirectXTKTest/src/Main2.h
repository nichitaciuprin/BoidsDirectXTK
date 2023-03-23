#pragma once
#include "pch.h"
#include "World.h"
#include "WorldWindow.h"
#include "Helper.h"
#include "console.h"

void wWinMain2(HINSTANCE hInstance)
{
    auto world = make_unique<World>();
    auto worldWindow = make_unique<WorldWindow>(hInstance);
    while(true)
    {
        if (Helper::EscapePressed()) break;
        auto directionWASD = worldWindow->DirectionWASD();
        world->Update(0.02f,directionWASD,Vector2::Zero);
        worldWindow->Render(world.get());
    }
}