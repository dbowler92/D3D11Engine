#include "D3D11DepthTexture2D.h"

using namespace EngineAPI::Graphics::Platform;

void D3D11DepthTexture2D::Shutdown()
{
	//Shutdown the texture
	depthStencilTexture2D.Shutdown();

	//Shutdown the core object
	__super::Shutdown();
}

bool D3D11DepthTexture2D::InitDepthTexture2D(EngineAPI::Graphics::GraphicsDevice* device,
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

	/*
	//DXGI formats
	DXGI_FORMAT dxgiTyplessFmt;
	DXGI_FORMAT dxgiDSVFmt;
	DXGI_FORMAT dxgiSRVFmt; //Ignore stencil
	if (textureFormat == DEPTH_STENCIL_FORMAT_D16_UNORM)
	{
		dxgiTyplessFmt = DXGI_FORMAT_R16_TYPELESS;
		dxgiDSVFmt = DXGI_FORMAT_D16_UNORM;
		dxgiSRVFmt = DXGI_FORMAT_R16_UNORM;
	}
	else if (textureFormat == DEPTH_STENCIL_FORMAT_D24_UNORM_S8_UINT)
	{
		dxgiTyplessFmt = DXGI_FORMAT_R24G8_TYPELESS;
		dxgiDSVFmt = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dxgiSRVFmt = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	}
	else if (textureFormat == DEPTH_STENCIL_FORMAT_D32_FLOAT)
	{
		dxgiTyplessFmt = DXGI_FORMAT_R32_TYPELESS;
		dxgiDSVFmt = DXGI_FORMAT_D32_FLOAT;
		dxgiSRVFmt = DXGI_FORMAT_R32_FLOAT;
	}

	//Underlying texture description
	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = dxgiTyplessFmt;

	if (msaaSampleCount == 1)
	{
		textureDesc.SampleDesc.Count = 1;   //No MSAA
		textureDesc.SampleDesc.Quality = 0; //No MSAA
	}
	else
	{
		uint32_t maxMSAAQuality = 0;
		device->GetD3D11Device()->CheckMultisampleQualityLevels(dxgiTyplessFmt, msaaSampleCount, &maxMSAAQuality);
		assert(maxMSAAQuality > 0);

		//Enable MSAA
		uint32_t msaaQualityLevel = maxMSAAQuality - 1;
		textureDesc.SampleDesc.Count = msaaSampleCount;
		textureDesc.SampleDesc.Quality = msaaQualityLevel;
	}

	textureDesc.Usage = D3D11_USAGE_DEFAULT; //No CPU access. 
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	//Shader input in addition to the Depth stencil view?
	if (willBeUsedAsShaderSamplerInput)
		textureDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;

	textureDesc.CPUAccessFlags = NULL; //No CPU access for depth textures
	textureDesc.MiscFlags = NULL;   

	//API agnostic usage flags
	ResourceUsage resourceUsage = (ResourceUsage)textureDesc.Usage;
	ResourceCPUAccessFlag cpuAccess = (ResourceCPUAccessFlag)textureDesc.CPUAccessFlags;
	ResourceBindFlag resourceBindingFlag = (ResourceBindFlag)textureDesc.BindFlags;

	//Init the underlying texture - no initial data
	if (!Internal_InitTexture2D(device, false, RESOURCE_TYPE_TEXTURE_2D, resourceUsage, cpuAccess, resourceBindingFlag, debugName))
		return false;

	//Done
	return true;
	*/
}