#include "ATBManager.h"

//Device - needed to init ATW
#include <Graphics/GraphicsDevice/GraphicsDevice.h>

using namespace EngineAPI::Debug;

//Static data
bool EngineAPI::Debug::ATBManager::isATBActive = true;

bool ATBManager::InitATB(EngineAPI::Graphics::GraphicsDevice* device, 
	uint32_t windowWidth, uint32_t windowHeight)
{
	assert(device != nullptr);

#ifdef ENGINE_CONFIG_GRAPHICS_API_D3D11
	assert(TwInit(TW_DIRECT3D11, device->GetD3D11Device()) != 0);
#endif

	//Set window size
	assert(ATBManager::OnResizeATB(windowWidth, windowHeight));

	//Done
	return true;
}

bool ATBManager::ShutdownATB()
{
	assert(TwTerminate() != 0);

	//Done
	return true;
}

bool ATBManager::RenderATB()
{
	if (ATBManager::isATBActive)
		assert(TwDraw() != 0);
	
	//Done
	return true;
}

bool ATBManager::OnResizeATB(uint32_t newWidth, uint32_t newHeight)
{
	assert(TwWindowSize(newWidth, newHeight) != 0);

	//Done
	return true;
}

#ifdef ENGINE_CONFIG_PLATFORM_WIN32
bool ATBManager::UpdateATB_Win32(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ATBManager::isATBActive)
	{
		if (TwEventWin(wnd, msg, wParam, lParam))
			return true;
		else
			return false;
	}

	//Didnt update, but this is not an error
	return false;
}
#endif