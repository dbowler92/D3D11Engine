#include "D3D11DepthStencilState.h"

using namespace EngineAPI::Graphics::Platform;

void D3D11DepthStencilState::Shutdown()
{
	ReleaseCOM(dss);

	//Cleanup super
	__super::Shutdown();
}

bool D3D11DepthStencilState::InitDepthStencilState(EngineAPI::Graphics::GraphicsDevice* device, 
	DepthStencilPipelineStateDescription* description,
	std::string debugName)
{
	//Must pass a desc - can use the default description if you want. 
	assert(description != nullptr);

	//Clear old DSS
	if (dss)
	{
		std::string o = std::string(__FUNCTION__) + ": " + "Releasing old DepthStencilState: " + GetDebugName();
		EngineAPI::Debug::DebugLog::PrintWarningMessage(o.c_str());
		ReleaseCOM(dss);
	}

	//Store debug name
	SetDebugName(debugName);

	//Fill D3D11 description
	FilloutD3D11DepthStencilDesc(description);

	//Print message saying we are creating a DSS
	std::string o = std::string(__FUNCTION__) + ": " + "Creating DepthStencilState: " + GetDebugName();
	EngineAPI::Debug::DebugLog::PrintInfoMessage(o.c_str());

	//Create the DSS object
	HR(device->GetD3D11Device()->CreateDepthStencilState(&dssDesc, &dss));
	if (dss == nullptr)
		return false;

	//Debug name
	SetD3D11ResourceDebugName(dss);

	//Done
	return true;
}

void D3D11DepthStencilState::BindDepthStencilStateToPipeline(EngineAPI::Graphics::GraphicsDevice* device, UINT stencilRef)
{
	device->GetD3D11ImmediateContext()->OMSetDepthStencilState(dss, stencilRef);
}

void D3D11DepthStencilState::FilloutD3D11DepthStencilDesc(DepthStencilPipelineStateDescription* state)
{
	//Depth:
	dssDesc.DepthEnable = state->DepthTestEnabled;
	dssDesc.DepthWriteMask = (D3D11_DEPTH_WRITE_MASK)state->DepthWriteMask;
	dssDesc.DepthFunc = (D3D11_COMPARISON_FUNC)state->DepthTestFunction;

	//Stencil:
	dssDesc.StencilEnable = state->StencilTestEnabled;
	dssDesc.StencilReadMask = state->StencilReadMask;
	dssDesc.StencilWriteMask = state->StencilWriteMask;

	//Op
	dssDesc.FrontFace.StencilFailOp = (D3D11_STENCIL_OP)state->FrontFaceOp.OnStencilFail;
	dssDesc.FrontFace.StencilPassOp = (D3D11_STENCIL_OP)state->FrontFaceOp.OnStencilPassDepthPass;
	dssDesc.FrontFace.StencilDepthFailOp = (D3D11_STENCIL_OP)state->FrontFaceOp.OnStencilPassDepthFail;
	dssDesc.FrontFace.StencilFunc = (D3D11_COMPARISON_FUNC)state->FrontFaceOp.StencilComparisonFunction;

	dssDesc.BackFace.StencilFailOp = (D3D11_STENCIL_OP)state->BackFaceOp.OnStencilFail;
	dssDesc.BackFace.StencilPassOp = (D3D11_STENCIL_OP)state->BackFaceOp.OnStencilPassDepthPass;
	dssDesc.BackFace.StencilDepthFailOp = (D3D11_STENCIL_OP)state->BackFaceOp.OnStencilPassDepthFail;
	dssDesc.BackFace.StencilFunc = (D3D11_COMPARISON_FUNC)state->BackFaceOp.StencilComparisonFunction;
}