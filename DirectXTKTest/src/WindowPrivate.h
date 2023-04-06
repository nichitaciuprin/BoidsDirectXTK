#pragma once
#include "winuser.h"
#include "World.h"
#include "DeviceResources.h"
#include "Console.h"
#include "Mouse.h"
#include "Helper.h"
using namespace std;
using namespace DX;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

namespace WindowPrivate
{
    const LPCWSTR className = L"WorldWindow";
    const LPCWSTR windowName = L"WorldWindow";
    const LPCWSTR iconName = L"IDI_ICON";
    const int defaultWidth = 800;
    const int defaultHeight = 600;
    bool classRegistered;
    bool sizemove = false;
    bool minimized = false;
    bool fullscreen = false;
    bool windowClosed = false;
    float key_w = 0.0f;
    float key_a = 0.0f;
    float key_s = 0.0f;
    float key_d = 0.0f;
    unique_ptr<DeviceResources> m_deviceResources;
    HWND m_hwnd;
    MSG msg = {};
    unique_ptr<GeometricPrimitive> m_shape;
    unique_ptr<Mouse> mouse;

    void OnDisplayChange()
    {
        m_deviceResources->UpdateColorSpace();
    }
    void OnWindowMoved()
    {
        auto outputSize = m_deviceResources->GetOutputSize();
        m_deviceResources->WindowSizeChanged(outputSize.right, outputSize.bottom);
    }
    void OnWindowSizeChanged(int width, int height)
    {
        if (!m_deviceResources->WindowSizeChanged(width, height))
            return;
    }
    LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        if (m_deviceResources == NULL)
            return DefWindowProc(hwnd, message, wParam, lParam);

        switch (message)
        {
            case WM_MOUSEACTIVATE: return MA_ACTIVATEANDEAT;
            case WM_ACTIVATE:
            case WM_ACTIVATEAPP:
            case WM_INPUT:
            case WM_MOUSEMOVE:
            case WM_LBUTTONDOWN:
            case WM_LBUTTONUP:
            case WM_RBUTTONDOWN:
            case WM_RBUTTONUP:
            case WM_MBUTTONDOWN:
            case WM_MBUTTONUP:
            case WM_MOUSEWHEEL:
            case WM_XBUTTONDOWN:
            case WM_XBUTTONUP:
            case WM_MOUSEHOVER:
                Mouse::ProcessMessage(message, wParam, lParam);
                break;
            // case WM_SETCURSOR:
            //     if (LOWORD(lParam) == HTCLIENT)
            //     {
            //         RECT clientRect;
            //         RECT windowRect;
            //         GetClientRect(hwnd, &clientRect);
            //         GetWindowRect(hwnd, &windowRect);
            //         AdjustWindowRect(&clientRect, WS_OVERLAPPEDWINDOW, FALSE);
            //         RECT finalRect = { windowRect.left, windowRect.top, windowRect.left+clientRect.right, windowRect.top+clientRect.bottom };
            //         ClipCursor(&windowRect);
            //         ShowCursor(false);
            //         // SetCursorPos(300,300);
            //     }
            //     break;
            case WM_DISPLAYCHANGE:
                OnDisplayChange();
                break;
            case WM_MOVE:
                OnWindowMoved();
                break;
            case WM_SIZE:
                if (wParam == SIZE_MINIMIZED && !minimized)
                {
                    minimized = true;
                }
                else if (minimized)
                {
                    minimized = false;
                }
                else if (!sizemove)
                {
                    OnWindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
                }
                break;
            case WM_EXITSIZEMOVE:
                sizemove = false;
                RECT rc;
                GetClientRect(hwnd, &rc);
                OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
                break;
            case WM_DESTROY:
                windowClosed = true;
                break;
            case WM_SYSKEYDOWN:
                switch (wParam)
                {
                    case VK_RETURN: // Alt+Enter
                        if (fullscreen)
                        {
                            SetWindowLongPtr(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
                            SetWindowLongPtr(hwnd, GWL_EXSTYLE, 0);
                            auto uFlags = SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED;
                            SetWindowPos(hwnd, HWND_TOP, 0, 0, defaultWidth, defaultHeight, uFlags);
                            ShowWindow(hwnd, SW_SHOWNORMAL);
                        }
                        else
                        {
                            SetWindowLongPtr(hwnd, GWL_STYLE, WS_POPUP);
                            SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_TOPMOST);
                            auto uFlags = SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED;
                            SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, uFlags);
                            ShowWindow(hwnd, SW_SHOWMAXIMIZED);
                        }
                        fullscreen = !fullscreen;
                    default: break;
                }
                break;
            case WM_MENUCHAR:
                // Removes error sound during alt+enter
                // A menu is active and the user presses a key that does not correspond
                // to any mnemonic or accelerator key
                return MAKELRESULT(0, MNC_CLOSE);
            case WM_KEYDOWN:
                switch (wParam)
                {
                    case VK_ESCAPE: PostQuitMessage(0); break;
                    case 'W': key_w =  1.0f; break;
                    case 'A': key_a = -1.0f; break;
                    case 'S': key_s = -1.0f; break;
                    case 'D': key_d =  1.0f; break;
                    default: break;
                }
                break;
            case WM_KEYUP:
                switch (wParam)
                {
                    case 'W': key_w = 0.0f; break;
                    case 'A': key_a = 0.0f; break;
                    case 'S': key_s = 0.0f; break;
                    case 'D': key_d = 0.0f; break;
                    default: break;
                }
                break;
        }

        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    void MaybeRegisterClass(HINSTANCE hInstance)
    {
        if (classRegistered) return;
        WNDCLASSEXW windowClass = {};
        windowClass.cbSize = sizeof(WNDCLASSEXW);
        windowClass.style = CS_HREDRAW | CS_VREDRAW;
        windowClass.lpfnWndProc = WndProc;
        windowClass.hInstance = hInstance;
        windowClass.hIcon = LoadIconW(hInstance, iconName);
        windowClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
        windowClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
        windowClass.lpszClassName = className;
        windowClass.hIconSm = LoadIconW(windowClass.hInstance, iconName);
        if (!RegisterClassExW(&windowClass)) throw;
        classRegistered = true;
    }
    void Create(HINSTANCE hInstance)
    {
        MaybeRegisterClass(hInstance);

        RECT rc = { 0, 0, static_cast<LONG>(defaultWidth), static_cast<LONG>(defaultHeight) };
        AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

        auto width = rc.right - rc.left;
        auto height = rc.bottom - rc.top;
        m_hwnd = CreateWindowExW(0 /*WS_EX_TOPMOST*/, className, windowName, WS_OVERLAPPEDWINDOW,
            0, 0, width, height, nullptr, nullptr, hInstance, nullptr);
        if (!m_hwnd) throw;
        ShowWindow(m_hwnd, SW_SHOWNORMAL);
        GetClientRect(m_hwnd, &rc);

        mouse = make_unique<Mouse>();
        mouse->SetWindow(m_hwnd);
        mouse->SetMode(Mouse::MODE_RELATIVE);

        m_deviceResources = make_unique<DeviceResources>();
        m_deviceResources->SetWindow(m_hwnd, width, height);
        m_deviceResources->CreateDeviceResources();
        m_deviceResources->CreateWindowSizeDependentResources();
        auto context = m_deviceResources->GetD3DDeviceContext();
        m_shape = GeometricPrimitive::CreateSphere(context);
    }
    Vector2 MouseLook()
    {
        auto state = mouse->GetState();
        return Vector2((float)state.x,(float)-state.y);
    }
    void HandleWindowMessages()
    {
        while (PeekMessage(&msg, m_hwnd, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    void Clear()
    {
        m_deviceResources->PIXBeginEvent(L"Clear");
        auto context = m_deviceResources->GetD3DDeviceContext();
        auto renderTarget = m_deviceResources->GetRenderTargetView();
        auto depthStencil = m_deviceResources->GetDepthStencilView();
        context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
        context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        context->OMSetRenderTargets(1, &renderTarget, depthStencil);
        auto const viewport = m_deviceResources->GetScreenViewport();
        context->RSSetViewports(1, &viewport);
        m_deviceResources->PIXEndEvent();
    }
    Vector3 ToDirection(Vector2 mousePosition)
    {
        mousePosition.x = remainderf(mousePosition.x,360);
        mousePosition.y = remainderf(mousePosition.y,360);
        if (mousePosition.x < 0) mousePosition.x = 360 - mousePosition.x;
        if (mousePosition.y < 0) mousePosition.y = 360 - mousePosition.y;
        auto x = cos(mousePosition.x);
        auto y = cos(mousePosition.y);
        auto z = sin(mousePosition.y);
        return Vector3(x,y,z);
    }
    void Paint(const World* world)
    {
        m_deviceResources->PIXBeginEvent(L"Render");

        auto size = m_deviceResources->GetOutputSize();
        auto cameraPosition = world->cameraPosition;
        auto cameraDirection = Helper::ToDirection(world->cameraRotation);
        // cameraPosition.z = -cameraPosition.z;
        auto m_view =
            Matrix::CreateLookAt(cameraPosition, cameraPosition+cameraDirection, Vector3::UnitY);
            // Matrix::CreateRotationX(Helper::ToRadian(world->cameraRotation.x)) *
            // Matrix::CreateRotationY(Helper::ToRadian(world->cameraRotation.y));
            // Matrix::CreateRotationX(world->cameraRotation.y/100);
        auto m_proj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f, float(size.right) / float(size.bottom), 0.1f, 100.f);

        auto m_world_1 = Matrix::CreateWorld(Vector3::Zero+Vector3::Forward*5,                  Vector3::Forward, Vector3::Up);
        auto m_world_2 = Matrix::CreateWorld(Vector3::Zero+Vector3::Forward*5+Vector3::Right*2, Vector3::Forward, Vector3::Up);
        auto m_world_3 = Matrix::CreateWorld(Vector3::Zero+Vector3::Forward*5+Vector3::Up*2,    Vector3::Forward, Vector3::Up);
        m_shape->Draw(m_world_1,m_view,m_proj);
        m_shape->Draw(m_world_2,m_view,m_proj);
        m_shape->Draw(m_world_3,m_view,m_proj);

        m_deviceResources->PIXEndEvent();

        m_deviceResources->Present();
    }
    void Render(const World* world)
    {
        HandleWindowMessages();
        Clear();
        Paint(world);
    }
    Vector2 DirectionWASD()
    {
        auto axisY = key_w + key_s;
        auto axisX = key_a + key_d;
        auto result = Vector2(axisX, axisY);
        result.Normalize();
        return result;
    }
}