#pragma once
#include "pch.h"
#include "DeviceResources.h"
#include "StepTimer.h"

using namespace DX;

class WorldWindow final : public IDeviceNotify
{
public:
    WorldWindow(HINSTANCE hInstance, int nCmdShow);
    void Render();
    bool ShouldQuit();
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
    void Paint();
private:
    void Update(StepTimer const& timer);
    void Clear();
    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();
    unique_ptr<DeviceResources> m_deviceResources;
};
