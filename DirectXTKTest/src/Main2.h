#pragma once
#include "Helper.h"
#include "World.h"
#include "MyWindow2.h"

void wWinMain2(HINSTANCE hInstance)
{
    auto world = make_unique<World>();
    // MyWindow2::Create(hInstance);

    const long timeStepInMilliseconds = 10;
    const float timeStepInSeconds = 0.020f;

    while(true)
    {
        if (Helper::EscapePressed()) break;

        auto oldTime = Helper::GetTime();

        Test::MethodTest();

        // Console_WriteLine(to_string(123));

        // world->Update(timeStepInSeconds,MyWindow2::DirectionWASD(),MyWindow2::MouseLook());
        // MyWindow2::Render(world.get());

        auto newTime = Helper::GetTime();
        Helper::Wait(oldTime,newTime,timeStepInMilliseconds);
    }
}