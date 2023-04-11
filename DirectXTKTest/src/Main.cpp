#include <Windows.h>
#include "MyLib.h"

void Render(const World* world)
{
    WindowUpdate();
    WindowClear();
    WindowRenderStart();
    WindowSetCamera(world->cameraPosition, world->cameraTarget, world->cameraUp);
    WindowDrawGround();
    auto length = world->boidWorld.boids.size();
    for (size_t i = 0; i < length; i++)
    {
        auto boid = &world->boidWorld.boids[i];
        WindowDrawSphere(boid->pos);
    }
    WindowRenderEnd();
}
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
    WindowInitDefault();

    while(true)
    {
        if (EscapePressed()) break;

        auto oldTime = GetTime();

        world->Update(timeStepF,Vector2::Zero,Vector2::Zero);
        Render(world.get());

        auto newTime = GetTime();
        WaitLoop(oldTime,newTime,timeStep);
    }

    EndMyLib();

    return 0;
}