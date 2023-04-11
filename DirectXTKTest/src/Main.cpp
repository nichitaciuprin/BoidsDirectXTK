#include <Windows.h>
#include "MyLib.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    // hides compiler warning for unused parameters
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    if (StartMyLib()) return 1;

    const long timeStep = 10;
    const float timeStepF = 0.010f;

    auto world = make_unique<World>();
    Window::Create();

    while(true)
    {
        if (EscapePressed()) break;

        auto oldTime = GetTime();

        world->Update(timeStepF,Window::DirectionWASD(),Window::MouseLook());
        Window::Render(world.get());

        auto newTime = GetTime();
        WaitLoop(oldTime,newTime,timeStep);
    }

    EndMyLib();

    return 0;
}