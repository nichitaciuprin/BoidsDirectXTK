#pragma once
#include "Helper.h"
#include "World.h"
#include "Window.h"

void wWinMain2(HINSTANCE hInstance)
{
    auto world = make_unique<World>();
    Window::Create(hInstance);

    const long timeStepInMilliseconds = 10;
    const float timeStepInSeconds = 0.020f;

    while(true)
    {
        if (Helper::EscapePressed()) break;

        auto oldTime = Helper::GetTime();

        // Console_WriteLine(to_string(123));

        world->Update(timeStepInSeconds,Window::DirectionWASD(),Window::MouseLook());
        Window::Render(world.get());

        auto newTime = Helper::GetTime();
        Helper::Wait(oldTime,newTime,timeStepInMilliseconds);
    }
}