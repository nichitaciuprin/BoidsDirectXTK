#pragma once
#include "pch.h"
#include "World.h"
#include "WorldWindow.h"
#include "WorldWindow2.h"
#include "Helper.h"
#include "console.h"
#include <time.h>
#include <iostream>
#include "MyWindow.h"

void wWinMain2(HINSTANCE hInstance)
{
    // Console_Test();
    auto world = make_unique<World>();
    MyWindow::Create(hInstance);
    while(true)
    {
        if (Helper::EscapePressed()) break;

        long oldTime = clock();

        world->Update(0.02f,MyWindow::DirectionWASD(),Vector2::Zero);
        MyWindow::Render(world.get());

        long newTime = clock();

        long diff = newTime - oldTime;
        long timeStep = 20;
        long waitTime = timeStep - diff;
        if (waitTime < 0) continue;
        Sleep(waitTime);
    }
}