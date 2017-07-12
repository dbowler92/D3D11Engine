#include "D3D11DepthStencilView.h"

#include "../../DepthTexture2D/DepthTexture2D.h"

using namespace EngineAPI::Graphics::Platform;

bool D3D11DepthStencilView::InitDepthStencilView(EngineAPI::Graphics::GraphicsDevice* device,
	EngineAPI::Graphics::DepthTexture2D* depthTextureResource,
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
	DepthStencilTextureFormat texFormat = depthTextureResource->GetDepthTextureFormat();
	if (texFormat == DEPTH_STENCIL_FORMAT_D16_UNORM)
		dsvDesc.Format = DXGI_FORMAT_D16_UNORM;
	else if (texFormat == DEPTH_STENCIL_FORMAT_D24_UNORM_S8_UINT)
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	else if (texFormat == DEPTH_STENCIL_FORMAT_D32_FLOAT)
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;

	//MSAA?
	if (depthTextureResource->IsMSAATexture())
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
	SetDebugName(debugName);
	std::string o = std::string(__FUNCTION__) + ": " + "Creating DepthStencilView: " + GetDebugName();
	EngineAPI::Debug::DebugLog::PrintInfoMessage(o.c_str());

	//Create DSV
	HR(device->GetD3D11Device()->CreateDepthStencilView(depthTextureResource->GetD3D11Texture2DHandle(), &dsvDesc, &dsv));
	if (dsv == nullptr)
		return false;

	//Debug name
	SetD3D11ResourceDebugName(dsv);

	//Done
	return true;
}

void D3D11DepthStencilView::Shutdown()
{
	ReleaseCOM(dsv);
}

bool D3D11DepthStencilView::ClearDepthStencil(EngineAPI::Graphics::GraphicsDevice* device,
	UINT clearFlag, float depthClear, uint8_t stencilClear)
{
	if (dsv)
	{
		ID3D11DeviceContext* immediateContext = device->GetD3D11ImmediateContext();
		immediateContext->ClearDepthStencilView(dsv, clearFlag, depthClear, stencilClear);
	
		return true;
	}
	else
		return false; 	//Error - DSV has been released.
}