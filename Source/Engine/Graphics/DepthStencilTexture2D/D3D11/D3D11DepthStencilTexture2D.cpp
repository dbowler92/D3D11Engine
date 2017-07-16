#include "D3D11DepthStencilTexture2D.h"

using namespace EngineAPI::Graphics::Platform;

void D3D11DepthStencilTexture2D::Shutdown()
{
	//Shutdown the texture
	depthStencilTexture2D.Shutdown();

	//Shutdown the core object
	__super::Shutdown();
}

bool D3D11DepthStencilTexture2D::InitDepthStencilTexture2D(EngineAPI::Graphics::GraphicsDevice* device,
	uint32_t textureWidth, uint32_t textureHeight, uint32_t msaaSampleCount,
	DepthStencilTextureFormat textureFormat,
	bool willBeUsedAsShaderSamplerInput,
	std::string debugName)
{
	//Store format
	this->depthTextureFormat = textureFormat;

	//Store if we are planning on using this as a shader input
	this->willBeUsedAsShaderSamplerInput = willBeUsedAsShaderSamplerInput;

	//Usage is GPU read-write only
	ResourceUsage resourceUsage = RESOURCE_USAGE_DEFAULT;
	ResourceCPUAccessFlag resourceCPUAccess = NULL;
	
	//Resource binding is depth_stencil + shader_resource if desired
	ResourceBindFlag resourceBinding = RESOURCE_BIND_DEPTH_STENCIL_TARGET_BIT;
	if (willBeUsedAsShaderSamplerInput)
		resourceBinding |= RESOURCE_BIND_SHADER_RESOURCE_BIT;

	//Calculate resource format in typeless form!
	ResourceFormat resourceFormat = RESOURCE_FORMAT_UNKNOWN;
	if (textureFormat == DEPTH_STENCIL_FORMAT_D16_UNORM)
		resourceFormat = RESOURCE_FORMAT_R16_TYPELESS;

	else if (textureFormat == DEPTH_STENCIL_FORMAT_D24_UNORM_S8_UINT)
		resourceFormat = RESOURCE_FORMAT_R24G8_TYPELESS;

	else if (textureFormat == DEPTH_STENCIL_FORMAT_D32_FLOAT)
		resourceFormat = RESOURCE_FORMAT_R32_TYPELESS;
	
	//Init the texture
	if (!depthStencilTexture2D.InitTexture2D(device, 
		textureWidth, textureHeight, msaaSampleCount, 
		1, 1, NULL, 
		nullptr,
		resourceFormat, resourceUsage, resourceCPUAccess, resourceBinding, 
		debugName))
	{
		//Error
		std::string o = std::string(__FUNCTION__) + " Failed to InitTexture2D. DebugName: " + debugName;
		EngineAPI::Debug::DebugLog::PrintErrorMessage(o.c_str());
		return false;
	}

	//Set core object debug name
	SetDebugName(debugName + "_D3D11DepthTexture2D");

	//Done
	return true;
}