#include "pch.h"
#include "World.h"
#include "WorldWindow.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

bool sizemove = false;
bool suspend = false;
bool minimized = false;
bool fullscreen = false;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    auto worldWindow = reinterpret_cast<WorldWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    if (worldWindow == NULL)
        return DefWindowProc(hwnd, message, wParam, lParam);

    switch (message)
    {
        case WM_DISPLAYCHANGE:
            worldWindow->OnDisplayChange();
            break;
        case WM_MOVE:
            worldWindow->OnWindowMoved();
            break;
        case WM_SIZE:
            if (wParam == SIZE_MINIMIZED && !minimized)
            {
                minimized = true;
                if (!suspend)
                    worldWindow->OnSuspending();
                suspend = true;
            }
            else if (minimized)
            {
                minimized = false;
                if (suspend)
                    worldWindow->OnResuming();
                suspend = false;
            }
            else if (!sizemove)
            {
                worldWindow->OnWindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
            }
            break;
        case WM_ENTERSIZEMOVE:
            sizemove = true;
            break;
        case WM_EXITSIZEMOVE:
            sizemove = false;
            RECT rc;
            GetClientRect(hwnd, &rc);
            worldWindow->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
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
            if (wParam)
                worldWindow->OnActivated();
            else
                worldWindow->OnDeactivated();
            break;
        case WM_POWERBROADCAST:
            switch (wParam)
            {
                case PBT_APMQUERYSUSPEND:
                    if (!suspend && worldWindow)
                        worldWindow->OnSuspending();
                    suspend = true;
                    return TRUE;
                case PBT_APMRESUMESUSPEND:
                    if (!minimized)
                    {
                        if (suspend && worldWindow)
                            worldWindow->OnResuming();
                        suspend = false;
                    }
                    return TRUE;
            }
            break;
        case WM_DESTROY:
            worldWindow->quit = true;
            PostQuitMessage(0);
            break;
        case WM_SYSKEYDOWN:
            if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
            {
                // Implements the classic ALT+ENTER fullscreen toggle
                if (fullscreen)
                {
                    SetWindowLongPtr(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
                    SetWindowLongPtr(hwnd, GWL_EXSTYLE, 0);
                    int width = 800;
                    int height = 600;
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
                fullscreen = !fullscreen;
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
                    worldWindow->quit = true;
                    PostQuitMessage(0);
                    break;
                default:
                    break;
            }
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

LPCWSTR lpszClassName = L"TEMPWindowClass";
LPCWSTR lpIconName = L"IDI_ICON";

bool classRegistered = false;
void MaybeRegisterClass(HINSTANCE hInstance)
{
    if (classRegistered) return;
    WNDCLASSEXW windowClass = {};
    windowClass.cbSize = sizeof(WNDCLASSEXW);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WndProc;
    windowClass.hInstance = hInstance;
    windowClass.hIcon = LoadIconW(hInstance, lpIconName);
    windowClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    windowClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    windowClass.lpszClassName = lpszClassName;
    windowClass.hIconSm = LoadIconW(windowClass.hInstance, lpIconName);
    if (!RegisterClassExW(&windowClass)) throw;
    classRegistered = true;
}

WorldWindow::WorldWindow(HINSTANCE hInstance)
{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    m_deviceResources->RegisterDeviceNotify(this);

    MaybeRegisterClass(hInstance);

    int w, h;
    GetDefaultSize(w, h);

    RECT rc = { 0, 0, static_cast<LONG>(w), static_cast<LONG>(h) };

    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    auto width = rc.right - rc.left;
    auto height = rc.bottom - rc.top;

    LPCWSTR g_szAppName = L"TEMP";
    m_hwnd = CreateWindowExW(0, lpszClassName, g_szAppName, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, hInstance,
        nullptr);
    // TODO: Change to CreateWindowExW(WS_EX_TOPMOST, L"TEMPWindowClass", g_szAppName, WS_POPUP,
    // to default to fullscreen.

    if (!m_hwnd) throw;

    ShowWindow(m_hwnd, SW_SHOWNORMAL);

    SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    GetClientRect(m_hwnd, &rc);

    m_deviceResources->SetWindow(m_hwnd, width, height);
    m_deviceResources->CreateDeviceResources();
    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    auto context = m_deviceResources->GetD3DDeviceContext();
    m_shape = GeometricPrimitive::CreateSphere(context);
}
void WorldWindow::Render(World* world)
{
    if (PeekMessage(&msg, m_hwnd, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    Clear();
    Paint(world);
}
void WorldWindow::Paint(World* world)
{
    m_deviceResources->PIXBeginEvent(L"Render");

    auto context = m_deviceResources->GetD3DDeviceContext();
    auto size = m_deviceResources->GetOutputSize();
    auto cameraPosition = world->cameraPosition;
    auto cameraTarget = cameraPosition+Vector3::Forward;
    auto m_view = Matrix::CreateLookAt(cameraPosition, cameraTarget, Vector3::UnitY);
    auto m_proj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f, float(size.right) / float(size.bottom), 0.1f, 10.f);
    auto m_world_1 = Matrix::CreateWorld(Vector3::Zero, Vector3::Forward, Vector3::Up);
    auto m_world_2 = Matrix::CreateWorld(Vector3::Zero+Vector3::Right*3, Vector3::Forward, Vector3::Up);
    m_shape->Draw(m_world_1,m_view,m_proj);
    m_shape->Draw(m_world_2,m_view,m_proj);

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
    CreateWindowSizeDependentResources();
}