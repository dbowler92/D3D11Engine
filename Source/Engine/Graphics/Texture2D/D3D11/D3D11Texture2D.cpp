#include "D3D11Texture2D.h"

using namespace EngineAPI::Graphics::Platform;


void D3D11Texture2D::Shutdown()
{
	//Release texture
	ReleaseCOM(texture2DHandle);

	//Shutdown up the chain
	__super::Shutdown();
}

bool D3D11Texture2D::InitTexture2D(EngineAPI::Graphics::GraphicsDevice* device, 
	bool doInitWitInitialData,
	ResourceUsage resourceUsage, ResourceCPUAccessFlag cpuAccess, ResourceBindFlag resourceBindingFlag,
	std::string debugName)
{
	//Destroy old texture
	if (texture2DHandle)
	{
		std::string o = std::string(__FUNCTION__) + ": " + "Releasing old texture: " + GetDebugName();
		EngineAPI::Debug::DebugLog::PrintWarningMessage(o.c_str());
		ReleaseCOM(texture2DHandle);
	}

	//Print message saying we are creating a texture
	std::string o = std::string(__FUNCTION__) + ": " + "Creating Texture: " + debugName;
	EngineAPI::Debug::DebugLog::PrintInfoMessage(o.c_str());

	//Init texture with initial data?
	D3D11_SUBRESOURCE_DATA* initialData = nullptr;
	if (doInitWitInitialData)
		initialData = &textureInitialData;

	HR(device->GetD3D11Device()->CreateTexture2D(&textureDesc, initialData, &texture2DHandle));
	if (texture2DHandle == nullptr)
		return false;

	//Debug name
	SetDebugName(debugName);

	//Init common resource with API agnostic usage data
	CommonResource::InitCommonResourceUsageData(resourceUsage, cpuAccess, resourceBindingFlag);

	//Done
	return true;
}

void D3D11Texture2D::SetDebugName(std::string s)
{
	//Set engine debug name
	__super::SetDebugName(s);

	//Set resource debug name
	if (texture2DHandle)
		texture2DHandle->SetPrivateData(WKPDID_D3DDebugObjectName, s.size(), s.c_str());
}
