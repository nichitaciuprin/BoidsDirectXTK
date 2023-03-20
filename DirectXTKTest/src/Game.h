#pragma once
#include "DeviceResources.h"
#include "StepTimer.h"

class Game final : public DX::IDeviceNotify
{
public:
    Game(HINSTANCE hInstance, int nCmdShow);
    void Initialize(HWND window, int width, int height);
    void Tick();
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
    void Update(DX::StepTimer const& timer);
    void Render();
    void Clear();
    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();
    std::unique_ptr<DX::DeviceResources>    m_deviceResources;
    DX::StepTimer                           m_timer;
};

int GameWindow_Create(HINSTANCE hInstance, int nCmdShow, Game* game);
void GameWindow_Render(Game* game);
bool GameWindow_ShouldQuit();