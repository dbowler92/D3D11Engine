//SamplerState.h
//Created 16/07/17
//Created By Daniel Bowler
//
//ID3D11SamplerState

#pragma once

//Parent class
#include <Graphics/PipelineStateResource/PipelineStateResource.h>

namespace EngineAPI
{
	namespace Graphics
	{
		namespace Platform
		{
			class D3D11SamplerState : public EngineAPI::Graphics::PipelineStateResource
			{
			public:
				D3D11SamplerState() {};
				virtual ~D3D11SamplerState() = 0 {};

				//Inits the sampler state
				bool InitSamplerState(EngineAPI::Graphics::GraphicsDevice* device,
					SamplerStateDescription* description,
					std::string debugName = std::string(""));

				//Shutsdown the sampler state object
				virtual void Shutdown() override;

			protected:
				//Manages an ID3D11SamplerState object
				ID3D11SamplerState* samplerState = nullptr;

				//State description - D3D11
				D3D11_SAMPLER_DESC samplerDesc = {};

			protected:
				//Fills the D3D11_SAMPLER_DESC struct based on the platform agnostic
				//DepthStencilPipelineStateDescription struct
				void FilloutD3D11SamplerDesc(SamplerStateDescription* state);
			};
		};
	};
};