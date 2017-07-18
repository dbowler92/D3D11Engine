#include "D3D11DepthStencilView.h"

#include <Graphics/DepthStencilTexture2D/DepthStencilTexture2D.h>

using namespace EngineAPI::Graphics::Platform;

void D3D11DepthStencilView::Shutdown()
{
	ReleaseCOM(dsv);

	//Shutdown core object
	__super::Shutdown();
}

bool D3D11DepthStencilView::InitDepthStencilView(EngineAPI::Graphics::GraphicsDevice* device,
	EngineAPI::Graphics::DepthStencilTexture2D* depthTextureResource,
	bool isReadOnlyDSV,
	std::string debugName)
{
	assert(depthTextureResource != nullptr);

	//Clear old DSV
	if (dsv)
	{
		std::string o = std::string(__FUNCTION__) + ": " + "Releasing old DepthStencilView: " + GetDebugName();
		EngineAPI::Debug::DebugLog::PrintWarningMessage(o.c_str());
		ReleaseCOM(dsv);
	}

	//Fillout DSV desc
	dsvDesc = {};

	//Calculate the format for the DSV based on depth buffer format. 
	DepthStencilTextureFormat texFormat = depthTextureResource->GetDepthStencilTextureFormat();
	if (texFormat == DEPTH_STENCIL_FORMAT_D16_UNORM)
		dsvDesc.Format = DXGI_FORMAT_D16_UNORM;
	else if (texFormat == DEPTH_STENCIL_FORMAT_D24_UNORM_S8_UINT)
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	else if (texFormat == DEPTH_STENCIL_FORMAT_D32_FLOAT)
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;

	//MSAA?
	if (depthTextureResource->GetTexture2D()->IsMSAATexture())
	{
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	}
	else
	{
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;
	}

	dsvDesc.Flags = 0;
	if (isReadOnlyDSV)
		dsvDesc.Flags = D3D11_DSV_READ_ONLY_DEPTH;

	//Create
	std::string o = std::string(__FUNCTION__) + ": " + "Creating DepthStencilView: " + debugName;
	EngineAPI::Debug::DebugLog::PrintInfoMessage(o.c_str());

	//Create DSV
	HR_CHECK_ERROR(device->GetD3D11Device()->CreateDepthStencilView(depthTextureResource->GetTexture2D()->GetD3D11Texture2DHandle(), &dsvDesc, &dsv));
	if (dsv == nullptr)
		return false;

	//Cache a reference to the ID3D11View pointer - BEFORE setting the D3D11
	//resource debug name
	CacheD3D11ViewInterface(dsv);

	//Debug name
	SetDebugName(debugName);

	//Done
	return true;
}

void D3D11DepthStencilView::ClearDepthStencilView(EngineAPI::Graphics::GraphicsDevice* device,
	DepthStencilClearFlag depthStencilBufferClearFlag, 
	float depthClear, uint8_t stencilClear)
{
	if (dsv)
		device->GetD3D11ImmediateContext()->ClearDepthStencilView(dsv, depthStencilBufferClearFlag, depthClear, stencilClear);
}

void D3D11DepthStencilView::BindDepthStencilView(EngineAPI::Graphics::GraphicsDevice* device)
{
	if (dsv)
	{
		//device->GetD3D11ImmediateContext()->OMSetRenderTargets(0, nullptr, dsv);

		ID3D11RenderTargetView* nullRenderTarget[1] = { NULL };
		device->GetD3D11ImmediateContext()->OMSetRenderTargets(1, nullRenderTarget, dsv);
	}
}