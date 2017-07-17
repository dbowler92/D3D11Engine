#include "RenderTarget.h"

//Graphics manager / subsystem
#include <Graphics/GraphicsManager/GraphicsManager.h>

using namespace EngineAPI::Rendering;

void RenderTarget::Shutdown() 
{
	//Shutdown self
	//
	renderTargetTexture2D.Shutdown();
	depthStencilTexture2D.Shutdown();

	renderTargetRTV.Shutdown();
	renderTargetSRV.Shutdown();

	depthStencilDSV.Shutdown();
	depthStencilSRV.Shutdown();

	//Shutdown super
	__super::Shutdown();
}

void RenderTarget::SetDebugName(std::string s)
{
	//Name object
	__super::SetDebugName(s);

	//Name of graphics resources
	//
	renderTargetTexture2D.SetDebugName(s + "_RenderTexture2D");
	depthStencilTexture2D.SetDebugName(s + "_DepthStencilTexture2D");
	
	renderTargetRTV.SetDebugName(s + "_RenderTarget_RTV");
	renderTargetSRV.SetDebugName(s + "_RenderTarget_SRV");

	depthStencilDSV.SetDebugName(s + "_DepthStencil_DSV");
	depthStencilSRV.SetDebugName(s + "_DepthStencil_SRV");
}

bool RenderTarget::InitRenderTarget(uint32_t width, uint32_t height, 
	ResourceFormat textureFormat, bool isUsedAsShaderResource,
	std::string debugName)
{
	return InitMSAARenderTarget(width, height, 1, textureFormat, isUsedAsShaderResource, debugName);
}

bool RenderTarget::InitMSAARenderTarget(uint32_t width, uint32_t height,
	uint32_t msaaSampleCount, ResourceFormat textureFormat,
	bool isUsedAsShaderResource,
	std::string debugName)
{
	EngineAPI::Graphics::GraphicsDevice* device = nullptr;
	device = EngineAPI::Graphics::GraphicsManager::GetInstance()->GetDevice();

	//Usage flags and the like
	ResourceUsage usg = RESOURCE_USAGE_DEFAULT; //GPU Read-Write
	ResourceCPUAccessFlag cpuAccess = 0; //No CPU access
	ResourceBindFlag resourceBinding = RESOURCE_BIND_RENDER_TARGET_BIT;
	if (isUsedAsShaderResource)
		resourceBinding |= RESOURCE_BIND_SHADER_RESOURCE_BIT;

	//Init texture2D
	if (!renderTargetTexture2D.InitRenderTexture2D(device,
		width, height, msaaSampleCount,
		textureFormat,
		usg, cpuAccess, resourceBinding,
		std::string(debugName + "_RenderTexture2D")))
	{
		//Error
		std::string o = "RenderTarget Error: Failed to init the RenderTexture2D. DebugName: " + debugName;
		EngineAPI::Debug::DebugLog::PrintErrorMessage(o.c_str());
		return false;
	}

	//Go ahead and init RenderTarget view to this texture - assume that the
	//format is the same as the resource format (TODO: Typeless formats)
	if (!renderTargetRTV.InitRenderTargetView(device,
		&renderTargetTexture2D, true, RESOURCE_FORMAT_USE_UNDERLYING_RESOURCE_FORMAT,
		std::string(debugName + "_RenderTarget_RTV")))
	{
		//Error
		std::string o = "RenderTarget Error: Failed to init the RenderTargetView to the RenderTexture2D. DebugName: " + debugName;
		EngineAPI::Debug::DebugLog::PrintErrorMessage(o.c_str());
		return false;
	}

	//If we want this as a shader resource, go ahead and init the SRV now
	if (isUsedAsShaderResource)
	{
		if (!renderTargetSRV.InitShaderResourceViewToTexture2D(device,
			renderTargetTexture2D.GetTexture2D(), true, RESOURCE_FORMAT_USE_UNDERLYING_RESOURCE_FORMAT, false,
			std::string(debugName + "_RenderTarget_SRV")))
		{
			//Error
			std::string o = "RenderTarget Error: Failed to init the ShaderResourceView to the RenderTexture2D. DebugName: " + debugName;
			EngineAPI::Debug::DebugLog::PrintErrorMessage(o.c_str());
			return false;
		}
	}

	//Set isInited flag
	isRenderTargetTextureAndViewsInited = true;

	//Done
	return true;
}

bool RenderTarget::InitRenderTargetWithDepthStencilTexture(uint32_t width, uint32_t height,
	ResourceFormat renderTargetTextureFormat, DepthStencilTextureFormat depthStencilTextureFormat,
	bool isUsedAsShaderResource,
	std::string debugName)
{
	//1) Init the render target + views
	bool state = InitRenderTarget(width, height, renderTargetTextureFormat, isUsedAsShaderResource, debugName);
	if (state == false)
		return false;

	//2) Init the depth/stencil texture + views
	if (!InitDepthStencilTextureAndViews(width, height, 1, depthStencilTextureFormat, isUsedAsShaderResource, debugName))
		return false;

	//Done
	return true;
}

bool RenderTarget::InitMSAARenderTargetWithDepthStencilTexture(uint32_t width, uint32_t height,
	uint32_t msaaSampleCount,
	ResourceFormat renderTargetTextureFormat, DepthStencilTextureFormat depthStencilTextureFormat,
	bool isUsedAsShaderResource,
	std::string debugName)
{
	//1) Init the MSAA render target + views
	if (!InitMSAARenderTarget(width, height, msaaSampleCount, renderTargetTextureFormat, isUsedAsShaderResource, debugName))
		return false;

	//2) Init the depth/stencil texture + views
	if (!InitDepthStencilTextureAndViews(width, height, msaaSampleCount, depthStencilTextureFormat, isUsedAsShaderResource, debugName))
		return false;

	//Done
	return true;
}

//
//Internal
//

bool RenderTarget::InitDepthStencilTextureAndViews(uint32_t width, uint32_t height, uint32_t msaaSampleCount,
	DepthStencilTextureFormat depthTexFormat, bool isUsedAsShaderResource,
	std::string debugName)
{
	assert(isRenderTargetTextureAndViewsInited == true);

	//TODO


	//Set isInited flag
	isDepthStencilTextureAndViewsInited = true;

	//Done
	return true;
}