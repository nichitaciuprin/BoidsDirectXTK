#include "pch.h"
#include "Main2.h"

// Adds support for clang. Maybe remove
#ifdef __clang__
#pragma clang diagnostic ignored "-Wcovered-switch-default"
#pragma clang diagnostic ignored "-Wswitch-enum"
#endif

// Tell Nvidia/AMD to use high-performance graphics card
extern "C"
{
    // Nvidia specifies this in their Optimus rendering specs.
    // http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;

    // AMD Specifies this in a tutorial doc for selecting the best graphics device
    // https://gpuopen.com/learn/amdpowerxpressrequesthighperformance/
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    // hides compiler warning for unused parameters
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // asserts that DirectXMath can be used
    if (!DirectX::XMVerifyCPUSupport()) return 1;

    // Initializes the COM library for use by the calling thread, sets the thread's concurrency model,
    // and creates a new apartment for the thread if one is required.
    if (FAILED(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED))) return 1;

    wWinMain2(hInstance,nCmdShow);

    // Closes the COM library on the current thread, unloads all DLLs loaded by the thread,
    // frees any other resources that the thread maintains, and forces all RPC connections on the thread to close.
    CoUninitialize();

    return 0;
}