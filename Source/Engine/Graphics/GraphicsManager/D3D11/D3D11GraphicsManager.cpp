#include "D3D11GraphicsManager.h"

using namespace EngineAPI::Graphics::Platform;

bool D3D11GraphicsManager::InitSubsystem(EngineAPI::OS::OSWindow* osWindow)
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage(__FUNCTION__);

	//Create the device && context
	assert(device.InitD3D11DeviceAndContext(osWindow));

	//Create swapchain + depth buffer
	assert(swapchain.InitD3D11Swapchain(&device, osWindow, 1, 1, true));

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
	return true;
}

bool D3D11GraphicsManager::OnEndRender()
{
	return true;
}