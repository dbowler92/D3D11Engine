#include "D3D11ShaderResourceView.h"

//Include textures
#include <Graphics/Texture2D/Texture2D.h>
#include <Graphics/RenderTexture2D/RenderTexture2D.h>
#include <Graphics/DepthStencilTexture2D/DepthStencilTexture2D.h>

using namespace EngineAPI::Graphics::Platform;

void D3D11ShaderResourceView::Shutdown()
{
	//Shutdown self
	ReleaseCOM(srv);

	//Shutdown parent
	__super::Shutdown();
}

bool D3D11ShaderResourceView::InitShaderResourceViewToTexture2D(EngineAPI::Graphics::GraphicsDevice* device,
	EngineAPI::Graphics::Texture2D* texture,
	bool doUseUnderlyingResourceFormatForView,
	ResourceFormat shaderResourceViewTextureFormat,
	bool doUseFirstMipOnly,
	std::string debugName)
{
	//Clear old srv
	if (srv)
	{
		std::string o = std::string(__FUNCTION__) + ": " + "Releasing old ShaderResourceView: " + GetDebugName();
		EngineAPI::Debug::DebugLog::PrintWarningMessage(o.c_str());
		ReleaseCOM(srv);
	}

	//Fillout srv desc
	srvDesc = {};

	//Format - important if the resource was created typeless
	if (doUseUnderlyingResourceFormatForView)
		srvDesc.Format = texture->GetD3D11Texture2DResourceFormat();
	else
		srvDesc.Format = (DXGI_FORMAT)shaderResourceViewTextureFormat;

	//Fillout rest of the description structure. 
	if (texture->IsMSAATexture())
	{
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
		srvDesc.Texture2DMS = {};
	}
	else
	{
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		if (doUseFirstMipOnly)
			srvDesc.Texture2D.MipLevels = 1;
		else
			srvDesc.Texture2D.MipLevels = -1; // -1 == texture->GetMipLevels() == ALL mips;

		srvDesc.Texture2D.MostDetailedMip = texture->GetMostDetailedMipLevel();
	}

	//Print creation message
	std::string o = std::string(__FUNCTION__) + ": " + "Creating ShaderResourceView: " + debugName;
	EngineAPI::Debug::DebugLog::PrintInfoMessage(o.c_str());

	//Create SRV
	HR_CHECK_ERROR(device->GetD3D11Device()->CreateShaderResourceView(texture->GetD3D11Texture2DHandle(), &srvDesc, &srv));
	if (srv == nullptr)
		return false;

	//Cache a reference to the ID3D11View pointer - BEFORE setting the D3D11
	//resource debug name
	CacheD3D11ViewInterface(srv);

	//Debug name inc D3D11 resource debug name
	SetDebugName(debugName);

	//Done
	return true;
}

bool D3D11ShaderResourceView::InitShaderResourceViewToDepthStencilTexture2D(EngineAPI::Graphics::GraphicsDevice* device,
	EngineAPI::Graphics::DepthStencilTexture2D* depthStencilTexture,
	bool doUseFirstMipOnly,
	std::string debugName)
{
	//Calculate the format for the view
	ResourceFormat depthStencilTextureSRVFormat;
	DepthStencilTextureFormat depthStencilTextureFormat = depthStencilTexture->GetDepthStencilTextureFormat();
	if (depthStencilTextureFormat == DEPTH_STENCIL_FORMAT_D16_UNORM)
		depthStencilTextureSRVFormat = RESOURCE_FORMAT_R16_UNORM;

	else if (depthStencilTextureFormat == DEPTH_STENCIL_FORMAT_D24_UNORM_S8_UINT)
		depthStencilTextureSRVFormat = RESOURCE_FORMAT_R24_UNORM_X8_TYPELESS; 

	else if (depthStencilTextureFormat == DEPTH_STENCIL_FORMAT_D32_FLOAT)
		depthStencilTextureSRVFormat = RESOURCE_FORMAT_R32_FLOAT;				//TODO: Allow this as R32_UNORM

	//Init SRV
	return (InitShaderResourceViewToTexture2D(device, depthStencilTexture->GetTexture2D(), 
		false, depthStencilTextureSRVFormat, doUseFirstMipOnly, debugName));
}


void D3D11ShaderResourceView::BindShaderResourceViewToVertexShader(EngineAPI::Graphics::GraphicsDevice* device, UINT bindingSlotIndex)
{
	device->GetD3D11ImmediateContext()->VSSetShaderResources(bindingSlotIndex, 1, &srv);
}

void D3D11ShaderResourceView::BindShaderResourceViewToPixelShader(EngineAPI::Graphics::GraphicsDevice* device, UINT bindingSlotIndex)
{
	device->GetD3D11ImmediateContext()->PSSetShaderResources(bindingSlotIndex, 1, &srv);
}

bool D3D11ShaderResourceView::AutoGenerateMipmaps(EngineAPI::Graphics::GraphicsDevice* device)
{
	if (srv)
	{
		device->GetD3D11ImmediateContext()->GenerateMips(srv);
		return true;
	}
	
	return false;
}