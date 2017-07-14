#include "D3D11GraphicsManager.h"

using namespace EngineAPI::Graphics::Platform;

bool D3D11GraphicsManager::InitSubsystem(EngineAPI::OS::OSWindow* osWindow)
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage(__FUNCTION__);

	SetDebugName("D3D11GraphicsManager");

	//Cache window size
	windowWidth = osWindow->GetWindowWidth();
	windowHeight = osWindow->GetWindowHeight();

	//Create the device && context(TODO: s -. Deferred context)
	assert(device.InitD3D11DeviceAndImmediateContext(osWindow));

	//Create swapchain + depth buffer
	assert(swapchain.InitD3D11Swapchain(&device, osWindow, 
		GRAPHICS_CONFIG_BACKBUFFER_COUNT, GRAPHICS_CONFIG_MSAA_SAMPLE_COUNT, GRAPHICS_CONFIG_DO_CREATE_DEPTH_BUFFER_WITH_SWAPCHAIN));

	return true;
}

void D3D11GraphicsManager::ShutdownSubsystem()
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage(__FUNCTION__);

	//Destroy system swapchain
	swapchain.Shutdown();

	//Lastly, shutdown device and context
	device.Shutdown();

	//Super
	__super::ShutdownSubsystem();
}

bool D3D11GraphicsManager::OnResize(EngineAPI::OS::OSWindow* osWindow)
{
	//Cache new window size
	windowWidth = osWindow->GetWindowWidth();
	windowHeight = osWindow->GetWindowHeight();

	//Resize the swapchain
	if (!swapchain.OnResize(&device, osWindow))
		return false;

	//Done
	return true;
}

bool D3D11GraphicsManager::OnBeginRender()
{
	//TEMP:
	//
	//Clear swapchain buffer (+ depth) ready for rendering
	const Float32Colour clearColour = { 0.0f, 0.0f, 0.0f, 0.0f };
	swapchain.ClearSwapchainBackbufferRenderTarget(&device, (float*)&clearColour);
	swapchain.ClearDepthStencilBuffer(&device, DEPTH_STENCIL_BUFFER_CLEAR_DEPTH_BIT | DEPTH_STENCIL_BUFFER_CLEAR_STENCIL_BIT, 1.0f, 0);

	//Bind
	swapchain.BindSwapchainBackbufferAsRenderTarget(&device, true);

	//Set viewport -> Render to the full screen
	swapchain.SetFullResolutionViewport(&device);

	//Done
	return true;
}

bool D3D11GraphicsManager::OnEndRender()
{
	//Present
	swapchain.PresentSwapchainBackbuffer();

	//Done
	return true;
}