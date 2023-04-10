#pragma once
#include "Helper.h"
#include "World.h"
#include "Window.h"

void wWinMain2(HINSTANCE hInstance)
{
    const long timeStepInMilliseconds = 10;
    const float timeStepInSeconds = 0.020f;

    auto world = make_unique<World>();
    Window::Create(hInstance);

    while(true)
    {
        if (Helper::EscapePressed()) break;

        auto oldTime = Helper::GetTime();

        world->Update(timeStepInSeconds,Window::DirectionWASD(),Window::MouseLook());
        Window::Render(world.get());

        auto newTime = Helper::GetTime();
        Helper::Wait(oldTime,newTime,timeStepInMilliseconds);
    }
}