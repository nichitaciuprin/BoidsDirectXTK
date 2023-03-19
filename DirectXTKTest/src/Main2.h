#pragma once
#include "pch.h"
#include "Game.h"

void wWinMain2(HINSTANCE hInstance, int nCmdShow)
{
    auto game = make_unique<Game>();
    if (GameWindow_Create(hInstance,nCmdShow,game.get())) return;

    while(true)
    {
        if (GameWindow_ShouldQuit()) break;

        // Game logic must be here

        GameWindow_Render(game.get());
    }

    game.reset();
}