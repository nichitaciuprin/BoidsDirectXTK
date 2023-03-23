#pragma once
#include "pch.h"
#include "World.h"
#include "DeviceResources.h"
#include "WorldWindow.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

using namespace DX;

class WorldWindow final : public IDeviceNotify
{
public:
    WorldWindow(HINSTANCE hInstance);
    void Render(const World* world);
    bool quit = false;
    void OnDeviceLost() override;
    void OnDeviceRestored() override;
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowMoved();
    void OnDisplayChange();
    void OnWindowSizeChanged(int width, int height);
    void GetDefaultSize(int& width, int& height ) const noexcept;
    bool key_w = false;
    bool key_a = false;
    bool key_s = false;
    bool key_d = false;
    Vector2 DirectionWASD()
    {
        auto value_1 = key_w ? 1 : 0;
        auto value_2 = key_a ? -1 : 0;
        auto value_3 = key_s ? -1 : 0;
        auto value_4 = key_d ? 1 : 0;
        auto axisY = value_3 + value_1;
        auto axisX = value_2 + value_4;
        Vector2 result = Vector2(axisX, axisY);
        result.Normalize();
        return result;
    }
private:
    void Clear()
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
    void Paint(const World* world);
    void CreateWindowSizeDependentResources();
    unique_ptr<DeviceResources> m_deviceResources;
    HWND m_hwnd;
    MSG msg = {};
    unique_ptr<DirectX::GeometricPrimitive> m_shape;
};
