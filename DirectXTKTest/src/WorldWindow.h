#pragma once
#include "pch.h"
#include "World.h"
#include "DeviceResources.h"

using namespace DX;

class WorldWindow final : public IDeviceNotify
{
public:
    WorldWindow(HINSTANCE hInstance);
    void Render(World* World);
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
    Vector2 DirectionWASD();
private:
    void Clear();
    void Paint(World* World);
    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();
    unique_ptr<DeviceResources> m_deviceResources;
    HWND m_hwnd;
    MSG msg = {};
    unique_ptr<DirectX::GeometricPrimitive> m_shape;
};
