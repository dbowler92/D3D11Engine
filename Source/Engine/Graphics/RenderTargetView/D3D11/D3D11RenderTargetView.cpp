#include "D3D11RenderTargetView.h"

//Needs to get D3D11 DSV from depth stencil view when binding
#include "../../DepthStencilView/DepthStencilView.h"

using namespace EngineAPI::Graphics::Platform;

bool D3D11RenderTargetView::InitRenderTargetView(EngineAPI::Graphics::GraphicsDevice* device,
	std::string debugName)
{
	//TODO

	//Done
	return true;
}

bool D3D11RenderTargetView::InitRenderTargetViewDirectFromD3D11Texture2D(EngineAPI::Graphics::GraphicsDevice* device,
	ID3D11Texture2D* d3d11TextureResource,
	std::string debugName)
{
	assert(d3d11TextureResource != nullptr);

	//Release old data
	if (rtv)
	{
		std::string o = std::string(__FUNCTION__) + ": " + "Releasing old RenderTargetView: " + GetDebugName();
		EngineAPI::Debug::DebugLog::PrintWarningMessage(o.c_str());
		ReleaseCOM(rtv);
	}

	//RTV description - Empty = use default settings
	rtvDesc = {};

	//Create
	SetDebugName(debugName);
	std::string o = std::string(__FUNCTION__) + ": " + "Creating RenderTargetView: " + GetDebugName();
	EngineAPI::Debug::DebugLog::PrintInfoMessage(o.c_str());

	//Create RTV
	HR(device->GetD3D11Device()->CreateRenderTargetView(d3d11TextureResource, nullptr, &rtv));
	if (rtv == nullptr)
		return false;

	//Debug name
	EngineAPI::Statics::D3D11ResourceStatics::SetD3D11ResourceDebugName(rtv, debugName);

	//Done
	return true;
}

void D3D11RenderTargetView::Shutdown()
{
	ReleaseCOM(rtv);
}

void D3D11RenderTargetView::ClearRenderTargetView(EngineAPI::Graphics::GraphicsDevice* device, const float* col)
{
	device->GetD3D11ImmediateContext()->ClearRenderTargetView(rtv, col);
}

void D3D11RenderTargetView::BindAsRenderTarget(EngineAPI::Graphics::GraphicsDevice* device,
	EngineAPI::Graphics::DepthStencilView* optionalDSV)
{
	ID3D11DeviceContext* immediateContext = device->GetD3D11ImmediateContext();

	//This RTV - just one for now
	ID3D11RenderTargetView* rtvs[1];
	rtvs[0] = rtv;

	//Bind.
	if (optionalDSV)
		immediateContext->OMSetRenderTargets(1, &rtvs[0], optionalDSV->GetD3D11DepthStencilView());
	else
		immediateContext->OMSetRenderTargets(1, &rtvs[0], nullptr);
}