#include "D3D11RenderTargetView.h"

using namespace EngineAPI::Rendering::Platform;

bool D3D11RenderTargetView::InitRenderTargetView(EngineAPI::Graphics::GraphicsDevice* device,
	std::string debugName)
{
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
		std::string o = std::string(__FUNCTION__) + ": " + "Releasing old RenderTargetView: " + debugIDString;
		EngineAPI::Debug::DebugLog::PrintWarningMessage(o.c_str());
		ReleaseCOM(rtv);
	}

	//RTV description - Empty = use default settings
	rtvDesc = {};

	//Create
	debugIDString = debugName;
	std::string o = std::string(__FUNCTION__) + ": " + "Creating RenderTargetView: " + debugIDString;
	EngineAPI::Debug::DebugLog::PrintInfoMessage(o.c_str());

	//Create RTV
	HR(device->GetD3D11Device()->CreateRenderTargetView(d3d11TextureResource, nullptr, &rtv));

	//Debug name
	rtv->SetPrivateData(WKPDID_D3DDebugObjectName, debugIDString.size(), debugIDString.c_str());

	//Done
	return true;
}

void D3D11RenderTargetView::Shutdown()
{
	ReleaseCOM(rtv);
}