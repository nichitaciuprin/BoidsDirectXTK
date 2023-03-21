#pragma once
#include "pch.h"
#include "DeviceResources.h"

using namespace DX;

class WorldWindow final : public IDeviceNotify
{
public:
    WorldWindow(HINSTANCE hInstance, int nCmdShow);
    void Render();
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
private:
    void Clear();
    void Paint();
    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();
    unique_ptr<DeviceResources> m_deviceResources;
    HWND m_hwnd;
    MSG msg = {};
    unique_ptr<DirectX::GeometricPrimitive> m_shape;
};
