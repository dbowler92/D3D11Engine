#include "D3D11RenderTexture2D.h"

using namespace EngineAPI::Graphics::Platform;

void D3D11RenderTexture2D::Shutdown()
{
	//Shutdown the texture
	renderTexture2D.Shutdown();

	//Shutdown super
	__super::Shutdown();
}

bool D3D11RenderTexture2D::InitRenderTexture2D(EngineAPI::Graphics::GraphicsDevice* device,
	uint32_t textureWidth, uint32_t textureHeight, uint32_t msaaSampleCount,
	ResourceFormat textureFormat,
	ResourceUsage textureUsage,
	ResourceCPUAccessFlag textureCpuAccess,
	ResourceBindFlag textureBindingFlag,
	std::string debugName)
{
	//Error checking
	//
	//Check usage
	D3D11_USAGE d3d11Usage = D3D11_USAGE_DEFAULT;
	UINT d3d11CpuAccess = NULL;
	assert(EngineAPI::Statics::D3D11ResourceStatics::CalculateD3D11UsageFlags(textureUsage, textureCpuAccess,
		d3d11Usage, d3d11CpuAccess));

	//Ensure that we want render target binding
	if ((textureBindingFlag & RESOURCE_BIND_RENDER_TARGET_BIT) == false)
		textureBindingFlag |= RESOURCE_BIND_RENDER_TARGET_BIT;

	//Init the texture
	if (!renderTexture2D.InitTexture2D(device,
		textureWidth, textureHeight, msaaSampleCount,
		1, 1, NULL,
		nullptr, 0, 
		textureFormat, textureUsage, textureCpuAccess, textureBindingFlag,
		debugName))
	{
		//Error
		std::string o = std::string(__FUNCTION__) + " Failed to InitTexture2D. DebugName: " + debugName;
		EngineAPI::Debug::DebugLog::PrintErrorMessage(o.c_str());
		return false;
	}

	//Set core object debug name
	SetDebugName(debugName + "_D3D11RenderTexture2D");

	//Done
	return true;
}
