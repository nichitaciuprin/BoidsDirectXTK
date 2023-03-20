#pragma once
#include "DeviceResources.h"
#include "StepTimer.h"

class Game final : public DX::IDeviceNotify
{
public:
    Game(HINSTANCE hInstance, int nCmdShow);
    void Initialize(HWND window, int width, int height);
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
    void Render();
private:
    void Update(DX::StepTimer const& timer);
    void Clear();
    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();
    std::unique_ptr<DX::DeviceResources>    m_deviceResources;
    DX::StepTimer                           m_timer;
};

void GameWindow_Render(Game* game);