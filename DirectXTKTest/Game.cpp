//
// Game.cpp
//

#include "pch.h"
#include "Game.h"

extern void ExitGame() noexcept;

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false)
{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    // TODO: Provide parameters for swapchain format, depth/stencil format, and backbuffer count.
    //   Add DX::DeviceResources::c_AllowTearing to opt-in to variable rate displays.
    //   Add DX::DeviceResources::c_EnableHDR for HDR10 display.
    m_deviceResources->RegisterDeviceNotify(this);
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_deviceResources->SetWindow(window, width, height);

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    float elapsedTime = float(timer.GetElapsedSeconds());
    float time = float(m_timer.GetTotalSeconds());
    auto size = m_deviceResources->GetOutputSize();
    // Vector3 cameraPos = Vector3::Lerp(Vector3(2.f, 2.f, 2.f),Vector3(2.f, 0.f, 2.f), cos(time));
    // Vector3 cameraPos = Vector3::Lerp(Vector3(2.f, 2.f, 2.f),Vector3::Zero, cos(time));
    // Vector3 cameraPos = Vector3(2,0,5);
    // Vector3 cameraPos = Vector3::Backward*5;
    Vector3 cameraPos = Vector3::Zero;
    Vector3 looktAt = cameraPos+Vector3::Forward;
    // Vector3 looktAt = Vector3::Zero;

    // m_world = Matrix::CreateRotationX(time);
    m_world = Matrix::CreateWorld(Vector3::Forward*20,Vector3::Forward,Vector3::Up);
    m_world_2 = Matrix::CreateWorld(Vector3::Forward*20+Vector3::Right*2,Vector3::Forward,Vector3::Up);
    // m_world_2 = Matrix::Identity;
    m_view = Matrix::CreateLookAt(cameraPos, looktAt, Vector3::UnitY);


    m_proj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f, float(size.right) / float(size.bottom), 0.1f, 100.f);
    // m_proj = Matrix::CreateOrthographicOffCenter(-10,10,-10,10,0,10);

    // m_world = Matrix::CreateRotationZ(cosf(time) * 2.f);

    elapsedTime;
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0) return;
    Clear();
    m_deviceResources->PIXBeginEvent(L"Render");
    auto context = m_deviceResources->GetD3DDeviceContext();

    m_shape->Draw(m_world, m_view, m_proj);
    m_shape->Draw(m_world_2, m_view, m_proj);

    m_deviceResources->PIXEndEvent();
    m_deviceResources->Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
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
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowMoved()
{
    auto const r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Game::OnDisplayChange()
{
    m_deviceResources->UpdateColorSpace();
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
    auto context = m_deviceResources->GetD3DDeviceContext();
    // m_shape = GeometricPrimitive::CreateSphere(context);
    m_shape = GeometricPrimitive::CreateCube(context);
    m_shape_2 = GeometricPrimitive::CreateCube(context);
    m_world = Matrix::Identity;
    m_world_2 = Matrix::Identity;

    // auto device = m_deviceResources->GetD3DDevice();

    // m_states = std::make_unique<CommonStates>(device);

    // m_effect = std::make_unique<BasicEffect>(device);
    // m_effect->SetVertexColorEnabled(true);

    // DX::ThrowIfFailed(
    //     CreateInputLayoutFromEffect<VertexType>(device, m_effect.get(),
    //         m_inputLayout.ReleaseAndGetAddressOf())
    //     );

    // auto context = m_deviceResources->GetD3DDeviceContext();
    // m_batch = std::make_unique<PrimitiveBatch<VertexType>>(context);
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // auto size = m_deviceResources->GetOutputSize();
    // // m_view = Matrix::CreateLookAt(Vector3(2.f, 2.f, 2.f), Vector3::Zero, Vector3::UnitY);
    // m_view = Matrix::CreateLookAt(Vector3::Forward*5, Vector3::Zero, Vector3::UnitY);
    // m_proj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f, float(size.right) / float(size.bottom), 0.1f, 10.f);
}

void Game::OnDeviceLost()
{
    m_shape.reset();
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion
