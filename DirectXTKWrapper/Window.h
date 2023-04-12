#pragma once

#include "GeometricPrimitive.h"
#include "winuser.h"
#include "DeviceResources.h"
#include "Console.h"
#include "Mouse.h"
#include "Helper.h"
using namespace std;
using namespace DX;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

class Window
{
public:
    Window(HINSTANCE hInstance, string windowName, int x, int y, int width, int height)
    {
        MaybeRegisterClass(hInstance);

        RECT rc = { x, y, width, height };
        AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

        auto width2 = rc.right - rc.left;
        auto height2 = rc.bottom - rc.top;
        m_hwnd = CreateWindowExW(0 /*WS_EX_TOPMOST*/, className, ToLongString(windowName), WS_OVERLAPPEDWINDOW,
            x, y, width2, height2, nullptr, nullptr, hInstance, nullptr);
        if (!m_hwnd) throw;
        ShowWindow(m_hwnd, SW_SHOWNORMAL);
        GetClientRect(m_hwnd, &rc);

        SetInstance(m_hwnd,this);

        // mouse = make_unique<Mouse>();
        // mouse->SetWindow(m_hwnd);
        // mouse->SetMode(Mouse::MODE_RELATIVE);

        m_deviceResources = make_unique<DeviceResources>();
        m_deviceResources->SetWindow(m_hwnd, width2, height2);
        m_deviceResources->CreateDeviceResources();
        m_deviceResources->CreateWindowSizeDependentResources();
        auto context = m_deviceResources->GetD3DDeviceContext();
        m_shape = GeometricPrimitive::CreateSphere(context,1);
        m_box = GeometricPrimitive::CreateBox(context,Vector3(1,1,1));
        m_ground = GeometricPrimitive::CreateBox(context,Vector3(100,1,100));
    }
    void Update()
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
    Vector2 DirectionWASD()
    {
        auto axisY = Window::key_w + Window::key_s;
        auto axisX = Window::key_a + Window::key_d;
        auto result = Vector2(axisX, axisY);
        result.Normalize();
        return result;
    }
    Vector2 MouseLook()
    {
        return Vector2::Zero;
        // auto state = Window::mouse->GetState();
        // return Vector2((float)state.x,(float)-state.y);
    }
    void ToFullscreen()
    {
        SetWindowLongPtr(m_hwnd, GWL_STYLE, WS_POPUP);
        SetWindowLongPtr(m_hwnd, GWL_EXSTYLE, WS_EX_TOPMOST);
        auto uFlags = SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED;
        SetWindowPos(m_hwnd, HWND_TOP, 0, 0, 0, 0, uFlags);
        ShowWindow(m_hwnd, SW_SHOWMAXIMIZED);
        fullscreen = true;
    }
    void ToDefaultSize()
    {
        SetWindowLongPtr(m_hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
        SetWindowLongPtr(m_hwnd, GWL_EXSTYLE, 0);
        auto uFlags = SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED;
        SetWindowPos(m_hwnd, HWND_TOP, 0, 0, defaultWidth, defaultHeight, uFlags);
        ShowWindow(m_hwnd, SW_SHOWNORMAL);
        fullscreen = false;
    }
    void PaintStart()
    {
        m_deviceResources->PIXBeginEvent(L"Render");
    }
    void PaintEnd()
    {
        m_deviceResources->PIXEndEvent();
        m_deviceResources->Present();
    }
    void PaintSetCamera(Vector3 position, Vector3 target, Vector3 up)
    {
        m_view = Matrix::CreateLookAt(position, target, up);
    }
    void PaintSetPerpective()
    {
        auto size = m_deviceResources->GetOutputSize();
        m_proj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f, float(size.right) / float(size.bottom), 0.1f, 1000.f);
    }
    void PaintGround()
    {
        auto groundPosition = Matrix::CreateWorld(Vector3::Zero+Vector3::Down/2, Vector3::Forward, Vector3::Up);
        m_ground->Draw(groundPosition,m_view,m_proj,Colors::Black);
    }
    void PaintSphere(Vector3 position)
    {
        auto m_world = Matrix::CreateWorld(position, Vector3::Forward, Vector3::Up);
        m_shape->Draw(m_world,m_view,m_proj);
    }
private:
    static bool classRegistered;
    static const LPCWSTR className;
    static const LPCWSTR iconName;
    const int defaultWidth = 800;
    const int defaultHeight = 600;
    bool sizemove = false;
    bool minimized = false;
    bool fullscreen = false;
    bool windowClosed = false;
    float key_w = 0.0f;
    float key_a = 0.0f;
    float key_s = 0.0f;
    float key_d = 0.0f;
    HWND m_hwnd;
    MSG msg = {};
    unique_ptr<DeviceResources> m_deviceResources;
    unique_ptr<GeometricPrimitive> m_shape;
    unique_ptr<GeometricPrimitive> m_box;
    unique_ptr<GeometricPrimitive> m_ground;
    // unique_ptr<Mouse> mouse;
    Matrix m_proj;
    Matrix m_view;
    static void MaybeRegisterClass(HINSTANCE hInstance)
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
    static Vector3 ToDirection(Vector2 mousePosition)
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
    static void SetInstance(HWND hwnd, Window* window)
    {
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
    }
    static Window* GetInstance(HWND hwnd)
    {
        return reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        auto window = GetInstance(hwnd);

        if (window == NULL)
            return DefWindowProc(hwnd, message, wParam, lParam);

        auto m_deviceResources = window->m_deviceResources.get();
        if (m_deviceResources == NULL)
            return DefWindowProc(hwnd, message, wParam, lParam);

        switch (message)
        {
            // case WM_MOUSEACTIVATE: return MA_ACTIVATEANDEAT;
            // case WM_ACTIVATE:
            // case WM_ACTIVATEAPP:
            // case WM_INPUT:
            // case WM_MOUSEMOVE:
            // case WM_LBUTTONDOWN:
            // case WM_LBUTTONUP:
            // case WM_RBUTTONDOWN:
            // case WM_RBUTTONUP:
            // case WM_MBUTTONDOWN:
            // case WM_MBUTTONUP:
            // case WM_MOUSEWHEEL:
            // case WM_XBUTTONDOWN:
            // case WM_XBUTTONUP:
            // case WM_MOUSEHOVER:
            //     Mouse::ProcessMessage(message, wParam, lParam);
            //     break;
            case WM_DISPLAYCHANGE:
                m_deviceResources->UpdateColorSpace();
                break;
            case WM_MOVE:
                auto outputSize = m_deviceResources->GetOutputSize();
                m_deviceResources->WindowSizeChanged(outputSize.right, outputSize.bottom);
                break;
            case WM_SIZE:
                if (wParam == SIZE_MINIMIZED && !window->minimized)
                {
                    window->minimized = true;
                }
                else if (window->minimized)
                {
                    window->minimized = false;
                }
                else if (!window->sizemove)
                {
                    window->OnWindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
                }
                break;
            case WM_EXITSIZEMOVE:
                window->sizemove = false;
                RECT rc;
                GetClientRect(hwnd, &rc);
                window->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
                break;
            case WM_DESTROY:
                window->windowClosed = true;
                break;
            case WM_SYSKEYDOWN:
                switch (wParam)
                {
                    case VK_RETURN: // Alt+Enter
                        if (window->fullscreen)
                            window->ToDefaultSize();
                        else
                            window->ToFullscreen();
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
                    case 'W': window->key_w =  1.0f; break;
                    case 'A': window->key_a = -1.0f; break;
                    case 'S': window->key_s = -1.0f; break;
                    case 'D': window->key_d =  1.0f; break;
                    default: break;
                }
                break;
            case WM_KEYUP:
                switch (wParam)
                {
                    case 'W': window->key_w = 0.0f; break;
                    case 'A': window->key_a = 0.0f; break;
                    case 'S': window->key_s = 0.0f; break;
                    case 'D': window->key_d = 0.0f; break;
                    default: break;
                }
                break;
        }

        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    static LPCWSTR ToLongString(const string str)
    {
        return wstring(str.begin(), str.end()).c_str();
    }
    void OnWindowSizeChanged(int width, int height)
    {
        if (!m_deviceResources->WindowSizeChanged(width, height))
            return;
    }
};
bool Window::classRegistered = false;
const LPCWSTR Window::className = L"MyWindow";
const LPCWSTR Window::iconName = L"IDI_ICON";