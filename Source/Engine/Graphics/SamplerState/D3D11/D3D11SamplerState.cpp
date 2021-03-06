#include "D3D11SamplerState.h"

using namespace EngineAPI::Graphics::Platform;

void D3D11SamplerState::Shutdown()
{
	ReleaseCOM(samplerState);

	//Cleanup super
	__super::Shutdown();
}

bool D3D11SamplerState::InitSamplerState(EngineAPI::Graphics::GraphicsDevice* device,
	SamplerStateDescription* description,
	std::string debugName)
{
	//Must pass a desc - can use the default description if you want. 
	assert(description != nullptr);

	//Clear old sampler state
	if (samplerState)
	{
		std::string o = std::string(__FUNCTION__) + ": " + "Releasing old SamplerState: " + GetDebugName();
		EngineAPI::Debug::DebugLog::PrintWarningMessage(o.c_str());
		ReleaseCOM(samplerState);
	}

	//Store debug name
	SetDebugName(debugName);

	//Fill D3D11 description
	FilloutD3D11SamplerDesc(description);

	//Print message saying we are creating a sampler state
	std::string o = std::string(__FUNCTION__) + ": " + "Creating SamplerState: " + GetDebugName();
	EngineAPI::Debug::DebugLog::PrintInfoMessage(o.c_str());

	//Create the DSS object
	HR_CHECK_ERROR(device->GetD3D11Device()->CreateSamplerState(&samplerDesc, &samplerState));
	if (samplerState == nullptr)
		return false;

	//Debug name
	EngineAPI::Statics::D3D11ResourceStatics::SetD3D11ResourceDebugName(samplerState, debugName);

	//Done
	return true;
}

void D3D11SamplerState::BindSamplerStateToVertexShader(EngineAPI::Graphics::GraphicsDevice* device, UINT samplerSlot)
{
	device->GetD3D11ImmediateContext()->VSSetSamplers(samplerSlot, 1, &samplerState);
}

void D3D11SamplerState::BindSamplerStateToPixelShader(EngineAPI::Graphics::GraphicsDevice* device, UINT samplerSlot)
{
	device->GetD3D11ImmediateContext()->PSSetSamplers(samplerSlot, 1, &samplerState);
}

void D3D11SamplerState::FilloutD3D11SamplerDesc(SamplerStateDescription* state)
{
	samplerDesc.Filter = (D3D11_FILTER)state->FilterMode;
	samplerDesc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)state->AddressModeU;
	samplerDesc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)state->AddressModeV;
	samplerDesc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)state->AddressModeW;
	samplerDesc.MipLODBias = state->MipLODBias;
	samplerDesc.MaxAnisotropy = state->MaxAnisotropy;
	samplerDesc.ComparisonFunc = (D3D11_COMPARISON_FUNC)state->ComparisonFunc;
	memcpy(&samplerDesc.BorderColor, &state->BorderColour, sizeof(FLOAT) * 4);
	samplerDesc.MinLOD = state->MinLOD;
	samplerDesc.MaxLOD = state->MaxLOD;
}