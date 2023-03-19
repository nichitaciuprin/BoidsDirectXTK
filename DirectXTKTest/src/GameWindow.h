#pragma once
#include "pch.h"
#include "Game.h"

int GameWindow_Create(HINSTANCE hInstance, int nCmdShow, Game* game);
void GameWindow_Render(Game* game);
bool GameWindow_ShouldQuit();