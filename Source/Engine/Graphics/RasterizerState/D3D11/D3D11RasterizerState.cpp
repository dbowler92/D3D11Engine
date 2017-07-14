#include "D3D11RasterizerState.h"

using namespace EngineAPI::Graphics::Platform;

void D3D11RasterizerState::Shutdown()
{
	ReleaseCOM(rss);

	//Cleanup super
	__super::Shutdown();
}

bool D3D11RasterizerState::InitRasterizerState(EngineAPI::Graphics::GraphicsDevice* device,
	RasterizerPipelineStateDescription* description,
	std::string debugName)
{
	assert(description);

	//Clear old RSS
	if (rss)
	{
		std::string o = std::string(__FUNCTION__) + ": " + "Releasing old RasterizerState: " + GetDebugName();
		EngineAPI::Debug::DebugLog::PrintWarningMessage(o.c_str());
		ReleaseCOM(rss);
	}

	//Store debug name
	SetDebugName(debugName);

	//Fill D3D11 desc struct
	FilloutD3D11RasterizerDesc(description);

	//Print message saying we are creating an RSS
	std::string o = std::string(__FUNCTION__) + ": " + "Creating RasterizerState: " + GetDebugName();
	EngineAPI::Debug::DebugLog::PrintInfoMessage(o.c_str());

	//Create the RSS object
	HR_CHECK_ERROR(device->GetD3D11Device()->CreateRasterizerState(&rssDesc, &rss));
	if (rss == nullptr)
		return false;

	//Debug name
	EngineAPI::Statics::D3D11ResourceStatics::SetD3D11ResourceDebugName(rss, debugName);

	//Done
	return true;
}

void D3D11RasterizerState::BindRasterizerStateToPipeline(EngineAPI::Graphics::GraphicsDevice* device)
{
	device->GetD3D11ImmediateContext()->RSSetState(rss);
}

void D3D11RasterizerState::FilloutD3D11RasterizerDesc(RasterizerPipelineStateDescription* state)
{
	rssDesc.FillMode = (D3D11_FILL_MODE)state->FillMode;
	rssDesc.CullMode = (D3D11_CULL_MODE)state->FaceCullingMode;

	//Winding order
	rssDesc.FrontCounterClockwise = (state->WindingOrder == POLYGON_WINDING_ORDER_FRONT_COUNTER_CLOCKWISE) ? TRUE : FALSE;

	rssDesc.DepthBias = state->DepthBias;
	rssDesc.DepthBiasClamp = state->DepthBiasClamp;
	rssDesc.SlopeScaledDepthBias = state->SlopeScaledDepthBias;
	rssDesc.DepthClipEnable = state->DepthClipEnabled;
	rssDesc.ScissorEnable = state->ScissorEnabled;
	rssDesc.MultisampleEnable = state->MultisampleEnabled;
	rssDesc.AntialiasedLineEnable = state->AntialiasedLineEnabled;
}