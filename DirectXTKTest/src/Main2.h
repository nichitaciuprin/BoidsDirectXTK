#pragma once
#include "pch.h"
#include "Game.h"

void wWinMain2(HINSTANCE hInstance, int nCmdShow)
{
    auto game = make_unique<Game>(hInstance,nCmdShow);

    while(true)
    {
        if (game->ShouldQuit()) break;

        // Game logic must be here

        game->Render();
    }

    game.reset();
}