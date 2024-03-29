#include <Windows.h>
#include "DirectXTKWrapper.h"
#include "World.h"

void BoidMain()
{
    if (XTKW::Initialise()) throw;

    auto timeStep = 10L;
    auto deltaTime = 0.010f;

    auto world = make_unique<World>();

    while(true)
    {
        if (XTKW::EscapePressed()) break;

        auto oldTime = XTKW::GetTime();

        world->Update(deltaTime);
        world->Render();

        auto newTime = XTKW::GetTime();

        XTKW::WaitLoop(oldTime,newTime,timeStep);
    }

    XTKW::Finalize();
}
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    // hides compiler warning for unused parameters
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    BoidMain();

    return 0;
}