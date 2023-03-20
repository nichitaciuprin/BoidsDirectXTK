#include "pch.h"
#include "WorldWindow.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

bool s_in_sizemove = false;
bool s_in_suspend = false;
bool s_minimized = false;
bool s_fullscreen = false;
MSG msg = {};

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    auto worldWindow = reinterpret_cast<WorldWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    switch (message)
    {
    case WM_PAINT:
        if (s_in_sizemove && worldWindow)
        {
            // worldWindow->Paint();
        }
        else
        {
            PAINTSTRUCT ps;
            std::ignore = BeginPaint(hwnd, &ps);
            EndPaint(hwnd, &ps);
        }
        break;

    case WM_DISPLAYCHANGE:
        if (worldWindow)
        {
            worldWindow->OnDisplayChange();
        }
        break;

    case WM_MOVE:
        if (worldWindow)
        {
            worldWindow->OnWindowMoved();
        }
        break;

    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
        {
            if (!s_minimized)
            {
                s_minimized = true;
                if (!s_in_suspend && worldWindow)
                    worldWindow->OnSuspending();
                s_in_suspend = true;
            }
        }
        else if (s_minimized)
        {
            s_minimized = false;
            if (s_in_suspend && worldWindow)
                worldWindow->OnResuming();
            s_in_suspend = false;
        }
        else if (!s_in_sizemove && worldWindow)
        {
            worldWindow->OnWindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
        }
        break;

    case WM_ENTERSIZEMOVE:
        s_in_sizemove = true;
        break;

    case WM_EXITSIZEMOVE:
        s_in_sizemove = false;
        if (worldWindow)
        {
            RECT rc;
            GetClientRect(hwnd, &rc);

            worldWindow->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
        }
        break;

    case WM_GETMINMAXINFO:
        if (lParam)
        {
            auto info = reinterpret_cast<MINMAXINFO*>(lParam);
            info->ptMinTrackSize.x = 320;
            info->ptMinTrackSize.y = 200;
        }
        break;

    case WM_ACTIVATEAPP:
        if (worldWindow)
        {
            if (wParam)
            {
                worldWindow->OnActivated();
            }
            else
            {
                worldWindow->OnDeactivated();
            }
        }
        break;

    case WM_POWERBROADCAST:
        switch (wParam)
        {
        case PBT_APMQUERYSUSPEND:
            if (!s_in_suspend && worldWindow)
                worldWindow->OnSuspending();
            s_in_suspend = true;
            return TRUE;

        case PBT_APMRESUMESUSPEND:
            if (!s_minimized)
            {
                if (s_in_suspend && worldWindow)
                    worldWindow->OnResuming();
                s_in_suspend = false;
            }
            return TRUE;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_SYSKEYDOWN:
        if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
        {
            // Implements the classic ALT+ENTER fullscreen toggle
            if (s_fullscreen)
            {
                SetWindowLongPtr(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
                SetWindowLongPtr(hwnd, GWL_EXSTYLE, 0);

                int width = 800;
                int height = 600;
                if (worldWindow)
                    worldWindow->GetDefaultSize(width, height);

                ShowWindow(hwnd, SW_SHOWNORMAL);

                SetWindowPos(hwnd, HWND_TOP, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
            }
            else
            {
                SetWindowLongPtr(hwnd, GWL_STYLE, WS_POPUP);
                SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_TOPMOST);

                SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

                ShowWindow(hwnd, SW_SHOWMAXIMIZED);
            }

            s_fullscreen = !s_fullscreen;
        }
        break;

    case WM_MENUCHAR:
        // A menu is active and the user presses a key that does not correspond
        // to any mnemonic or accelerator key. Ignore so we don't produce an error beep.
        return MAKELRESULT(0, MNC_CLOSE);

    case WM_KEYDOWN:
        switch (wParam)
        {
            case VK_ESCAPE:
                PostQuitMessage(0);
                break;

            default:
                break;
        }
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

WorldWindow::WorldWindow(HINSTANCE hInstance, int nCmdShow)
{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    m_deviceResources->RegisterDeviceNotify(this);

    WNDCLASSEXW wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEXW);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIconW(hInstance, L"IDI_ICON");
    wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wcex.lpszClassName = L"TEMPWindowClass";
    wcex.hIconSm = LoadIconW(wcex.hInstance, L"IDI_ICON");

    if (!RegisterClassExW(&wcex)) throw;

    // Create window
    int w, h;
    GetDefaultSize(w, h);

    RECT rc = { 0, 0, static_cast<LONG>(w), static_cast<LONG>(h) };

    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    LPCWSTR g_szAppName = L"TEMP";
    HWND hwnd = CreateWindowExW(0, L"TEMPWindowClass", g_szAppName, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
        nullptr);
    // TODO: Change to CreateWindowExW(WS_EX_TOPMOST, L"TEMPWindowClass", g_szAppName, WS_POPUP,
    // to default to fullscreen.

    if (!hwnd) throw;

    ShowWindow(hwnd, nCmdShow);
    // TODO: Change nCmdShow to SW_SHOWMAXIMIZED to default to fullscreen.

    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    GetClientRect(hwnd, &rc);

    auto width = rc.right - rc.left;
    auto height = rc.bottom - rc.top;

    m_deviceResources->SetWindow(hwnd, width, height);
    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();
    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();
}
void WorldWindow::Update(DX::StepTimer const& timer)
{
    // float elapsedTime = float(timer.GetElapsedSeconds());
}
void WorldWindow::Render()
{
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    Paint();
}
bool WorldWindow::ShouldQuit()
{
    return msg.message == WM_QUIT;
}
void WorldWindow::Paint()
{
    Clear();
    m_deviceResources->PIXBeginEvent(L"Render");
    auto context = m_deviceResources->GetD3DDeviceContext();

    // TODO: Add your rendering code here.

    m_deviceResources->PIXEndEvent();
    m_deviceResources->Present();
}
void WorldWindow::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto const viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_deviceResources->PIXEndEvent();
}
void WorldWindow::OnActivated()
{
    // TODO: Game is becoming active window.
}
void WorldWindow::OnDeactivated()
{
    // TODO: Game is becoming background window.
}
void WorldWindow::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}
void WorldWindow::OnResuming()
{
}
void WorldWindow::OnWindowMoved()
{
    auto const r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}
void WorldWindow::OnDisplayChange()
{
    m_deviceResources->UpdateColorSpace();
}
void WorldWindow::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}
void WorldWindow::GetDefaultSize(int& width, int& height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}
void WorldWindow::CreateDeviceDependentResources()
{
    auto device = m_deviceResources->GetD3DDevice();

    // TODO: Initialize device dependent objects here (independent of window size).
    device;
}
void WorldWindow::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.
}
void WorldWindow::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.
}
void WorldWindow::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}