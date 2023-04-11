#include <Windows.h>
#include "MyLib.h"

void Render(const World* world)
{
    WindowPrivate::HandleWindowMessages();
    WindowPrivate::Clear();

    WindowPrivate::PaintStart();

    WindowPrivate::PaintSetCamera(world->cameraPosition, world->cameraTarget, world->cameraUp);
    WindowPrivate::PaintSetPerpective();

    WindowPrivate::PaintGround();

    auto length = world->boidWorld.boids.size();
    for (size_t i = 0; i < length; i++)
    {
        auto boid = &world->boidWorld.boids[i];
        WindowPrivate::PaintSphere(boid->pos);
    }

    WindowPrivate::PaintEnd();
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

        world->Update(timeStepF,Window::DirectionWASD(),Window::MouseLook());
        Render(world.get());

        auto newTime = GetTime();
        WaitLoop(oldTime,newTime,timeStep);
    }

    EndMyLib();

    return 0;
}