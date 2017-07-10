#include "D3D11GraphicsManager.h"

using namespace EngineAPI::Graphics::Platform;

bool D3D11GraphicsManager::InitSubsystem(EngineAPI::OS::OSWindow* osWindow)
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage(__FUNCTION__);

	//Create the device && context(TODO: s -. Deferred context)
	assert(device.InitD3D11DeviceAndImmediateContext(osWindow));

	//Create swapchain + depth buffer
#if GRAPHICS_CONFIG_DO_USE_4XMSAA
	assert(swapchain.InitD3D11Swapchain(&device, osWindow, 1, 4, true));
#else
	assert(swapchain.InitD3D11Swapchain(&device, osWindow, 1, 1, true));
#endif

	return true;
}

void D3D11GraphicsManager::ShutdownSubsystem()
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage(__FUNCTION__);

	//Destroy system swapchain
	swapchain.ShutdownD3D11Swapchain();

	//Lastly, shutdown device and context
	device.ShutdownD3D11DeviceAndContext();
}

bool D3D11GraphicsManager::OnResize(EngineAPI::OS::OSWindow* osWindow)
{
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
	const float clearColour[4] = { 0.0f, 0.0f, 1.0f, 0.0f };
	swapchain.ClearSwapchainBackbufferRenderTarget(&device, clearColour);
	swapchain.ClearDepthStencilBuffer(&device, true, true, 1.0f, 0);

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