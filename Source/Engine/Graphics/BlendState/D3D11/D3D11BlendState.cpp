#include "D3D11BlendState.h"

using namespace EngineAPI::Graphics::Platform;

void D3D11BlendState::Shutdown()
{
	ReleaseCOM(bs);

	//Cleanup super
	__super::Shutdown();
}

bool D3D11BlendState::InitBlendState(EngineAPI::Graphics::GraphicsDevice* device,
	BlendPipelineStateDescription* description,
	std::string debugName)
{
	//Must pass a desc - can use the default description if you want. 
	assert(description != nullptr);

	//Clear old blend state
	if (bs)
	{
		std::string o = std::string(__FUNCTION__) + ": " + "Releasing old BlendState: " + GetDebugName();
		EngineAPI::Debug::DebugLog::PrintWarningMessage(o.c_str());
		ReleaseCOM(bs);
	}

	//Store debug name
	SetDebugName(debugName);

	//Fill D3D11 description
	FilloutD3D11BlendDesc(description);

	//Print message saying we are creating a new blend state object
	std::string o = std::string(__FUNCTION__) + ": " + "Creating BlendState: " + GetDebugName();
	EngineAPI::Debug::DebugLog::PrintInfoMessage(o.c_str());

	//Create the blend state object
	HR_CHECK_ERROR(device->GetD3D11Device()->CreateBlendState(&bsDesc, &bs));
	if (bs == nullptr)
		return false;

	//Debug name
	EngineAPI::Statics::D3D11ResourceStatics::SetD3D11ResourceDebugName(bs, debugName);

	//Done 
	return true;
}
void D3D11BlendState::BindBlendStateToPipeline(EngineAPI::Graphics::GraphicsDevice* device, const float blendFactor[4], UINT sampleMask)
{
	device->GetD3D11ImmediateContext()->OMSetBlendState(bs, blendFactor, sampleMask);
}


void D3D11BlendState::FilloutD3D11BlendDesc(BlendPipelineStateDescription* state)
{
	bsDesc.AlphaToCoverageEnable = state->AlphaToCoverageEnable;
	bsDesc.IndependentBlendEnable = state->IndependentBlendEnable;

	for (int i = 0; i < MAX_RENDER_TARGETS_THAT_CAN_BE_BOUND; ++i)
	{
		bsDesc.RenderTarget[0].BlendEnable = state->RenderTargetsBlendState[i].BlendEnabled;

		bsDesc.RenderTarget[0].SrcBlend  = (D3D11_BLEND)state->RenderTargetsBlendState[i].SourceBlend;
		bsDesc.RenderTarget[0].DestBlend = (D3D11_BLEND)state->RenderTargetsBlendState[i].DestinationBlend;
		bsDesc.RenderTarget[0].BlendOp   = (D3D11_BLEND_OP)state->RenderTargetsBlendState[i].BlendOp;

		bsDesc.RenderTarget[0].SrcBlendAlpha  = (D3D11_BLEND)state->RenderTargetsBlendState[i].SourceAlphaBlend;
		bsDesc.RenderTarget[0].DestBlendAlpha = (D3D11_BLEND)state->RenderTargetsBlendState[i].DestinationAlphaBlend;
		bsDesc.RenderTarget[0].BlendOpAlpha   = (D3D11_BLEND_OP)state->RenderTargetsBlendState[i].BlendAlphaOp;

		bsDesc.RenderTarget[0].RenderTargetWriteMask = state->RenderTargetsBlendState[i].RenderTargetWriteMask;
	}
}