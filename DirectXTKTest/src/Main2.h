#pragma once
#include "Helper.h"
#include "World.h"
#include "MyWindow2.h"

void wWinMain2(HINSTANCE hInstance)
{
    auto world = make_unique<World>();
    MyWindow2::Create(hInstance);

    const long timeStepInMilliseconds = 20;
    const float timeStepInSeconds = 0.02f;

    while(true)
    {
        if (Helper::EscapePressed()) break;

        auto oldTime = Helper::GetTime();

        world->Update(timeStepInSeconds,MyWindow2::DirectionWASD(),Vector2::Zero);
        MyWindow2::Render(world.get());

        auto newTime = Helper::GetTime();
        Helper::Wait(oldTime,newTime,timeStepInMilliseconds);
    }
}