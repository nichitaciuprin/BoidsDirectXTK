#include <Windows.h>
#include <DirectXMath.h>
#include "Helper.h"
#include "World.h"
#include "Window.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    // hides compiler warning for unused parameters
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    // asserts that DirectXMath can be used
    if (!DirectX::XMVerifyCPUSupport()) return 1;

    // Initializes the COM library for use by the calling thread, sets the thread's concurrency model,
    // and creates a new apartment for the thread if one is required.
    if (FAILED(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED))) return 1;

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

    // Closes the COM library on the current thread, unloads all DLLs loaded by the thread,
    // frees any other resources that the thread maintains, and forces all RPC connections on the thread to close.
    CoUninitialize();

    return 0;
}