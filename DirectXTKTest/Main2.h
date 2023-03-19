#include "pch.h"
#include "Game.h"
#include "WinMainWindow.h"

void wWinMain2(HINSTANCE hInstance, int nCmdShow)
{
    auto game = std::make_unique<Game>();
    if (WinMainWindow_Create(hInstance,nCmdShow,game.get())) return;
    while(true)
    {
        if (WinMainWindow_ShouldQuit()) break;

        // Game logic must be here

        WinMainWindow_Render(game.get());
    }
    game.reset();
}