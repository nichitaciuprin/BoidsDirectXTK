#pragma once
#include "pch.h"
#include "WorldWindow.h"

void wWinMain2(HINSTANCE hInstance, int nCmdShow)
{
    auto worldWindow = make_unique<WorldWindow>(hInstance,nCmdShow);

    while(true)
    {
        if (worldWindow->quit) break;

        // Game logic must be here

        worldWindow->Render();
    }

    worldWindow.reset();
}