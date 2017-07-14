#include "D3D11RenderTargetView.h"

//RenderTexture2D
#include <Graphics/RenderTexture2D/RenderTexture2D.h>

//Needs to get D3D11 DSV from depth stencil view when binding
#include <Graphics/DepthStencilView/DepthStencilView.h>

using namespace EngineAPI::Graphics::Platform;

void D3D11RenderTargetView::Shutdown()
{
	ReleaseCOM(rtv);

	//Shutdown core object
	__super::Shutdown();
}

bool D3D11RenderTargetView::InitRenderTargetView(EngineAPI::Graphics::GraphicsDevice* device,
	EngineAPI::Graphics::RenderTexture2D* renderTexture,
	std::string debugName)
{
	assert(renderTexture != nullptr);



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
	std::string o = std::string(__FUNCTION__) + ": " + "Creating RenderTargetView: " + debugName;
	EngineAPI::Debug::DebugLog::PrintInfoMessage(o.c_str());

	//Create RTV
	HR_CHECK_ERROR(device->GetD3D11Device()->CreateRenderTargetView(d3d11TextureResource, nullptr, &rtv));
	if (rtv == nullptr)
		return false;

	//Cache a reference to the ID3D11View pointer - BEFORE setting the D3D11
	//resource debug name
	CacheD3D11ViewInterface(rtv);

	//Debug name
	SetDebugName(debugName);

	//Done
	return true;
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