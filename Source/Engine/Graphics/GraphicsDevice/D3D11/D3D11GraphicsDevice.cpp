#include "D3D11GraphicsDevice.h"

//Forward declarations
#include "../../OS/OSWindow/OSWindow.h"

using namespace EngineAPI::Graphics::Platform;

bool D3D11GraphicsDevice::InitD3D11DeviceAndImmediateContext(EngineAPI::OS::OSWindow* osWindow)
{
	//If debug mode, set the D3D11_CREATE_DEVICE_DEBUG flag
	UINT createDeviceFlags = 0;
#if defined (DEBUG) || defined (_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//Hardware driver type
	D3D_DRIVER_TYPE d3dDriverType = D3D_DRIVER_TYPE_HARDWARE;

	//Feature levels struct to be passed to D3D11CreateDevice()
	D3D_FEATURE_LEVEL featureLevel;

	//Create the device.
	HRESULT hr = D3D11CreateDevice(
		NULL,                 //Default adapter
		d3dDriverType,
		NULL,
		createDeviceFlags,
		NULL, NULL,            //Default feature levels array
		D3D11_SDK_VERSION,
		&d3dDevice,
		&featureLevel,
		&d3dImmediateContext);

	//Check for error
	if (FAILED(hr))
	{
		ReleaseCOM(d3dDevice);
		ReleaseCOM(d3dImmediateContext);

		EngineAPI::Debug::DebugLog::PrintErrorMessage("D3D11GraphicsDevice::InitD3D11Device() - D3D11CreateDevice() failed!");
		return false;
	}

	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		ReleaseCOM(d3dDevice);
		ReleaseCOM(d3dImmediateContext);

		//Another error
		EngineAPI::Debug::DebugLog::PrintErrorMessage("D3D11GraphicsDevice::InitD3D11Device() - D3D feature level is not 11_0");
		return false;
	}

	//Done
	return true;
}

void D3D11GraphicsDevice::ShutdownD3D11DeviceAndContext()
{
	ReleaseCOM(d3dImmediateContext);
	ReleaseCOM(d3dDevice);
}