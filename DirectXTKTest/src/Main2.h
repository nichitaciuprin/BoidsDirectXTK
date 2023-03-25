#pragma once
#include "Helper.h"
#include "World.h"
#include "MyWindow2.h"

void wWinMain2(HINSTANCE hInstance)
{
    auto world = make_unique<World>();
    MyWindow2::Create(hInstance);

    while(true)
    {
        if (Helper::EscapePressed()) break;

        auto oldTime = Helper::GetTime();

        world->Update(0.02f,MyWindow2::DirectionWASD(),Vector2::Zero);
        MyWindow2::Render(world.get());

        auto newTime = Helper::GetTime();
        Helper::Wait(oldTime,newTime,20);
    }
}