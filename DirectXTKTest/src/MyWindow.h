#pragma once
#include "pch.h"
#include "World.h"
#include "DeviceResources.h"
using namespace std;
using namespace DX;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

namespace MyWindow
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

        m_deviceResources = make_unique<DeviceResources>();
        m_deviceResources->SetWindow(m_hwnd, width, height);
        m_deviceResources->CreateDeviceResources();
        m_deviceResources->CreateWindowSizeDependentResources();
        auto context = m_deviceResources->GetD3DDeviceContext();
        m_shape = GeometricPrimitive::CreateSphere(context);
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
    void Paint(const World* world)
    {
        m_deviceResources->PIXBeginEvent(L"Render");

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