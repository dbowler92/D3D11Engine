//D3D11BlendState.h
//Created 12/07/17
//Created By Daniel Bowler
//
//ID3D11BlendState

#pragma once

//Parent class
#include <Graphics/PipelineStateResource/PipelineStateResource.h>

namespace EngineAPI
{
	namespace Graphics
	{
		namespace Platform
		{
			class D3D11BlendState : public EngineAPI::Graphics::PipelineStateResource
			{
			public:
				D3D11BlendState() {};
				virtual ~D3D11BlendState() = 0 {};

				//Inits the blend state object
				bool InitBlendState(EngineAPI::Graphics::GraphicsDevice* device,
					BlendPipelineStateDescription* description,
					std::string debugName = std::string(""));

				//Shutsdown the DSS state object
				virtual void Shutdown() override;

			public:
				//Binds the blend state to the OM stage
				void BindBlendStateToPipeline(EngineAPI::Graphics::GraphicsDevice* device, const float blendFactor[4], UINT sampleMask);

			protected:
				//Manages an ID3D11BlendState
				ID3D11BlendState* bs = nullptr;

				//Blend state description
				D3D11_BLEND_DESC bsDesc = {};

			private:
				//Internal funciton which converts from API agnostic description
				//struct to a D3D11_BLEND_DESC
				void FilloutD3D11BlendDesc(BlendPipelineStateDescription* state);
			};
		};
	};
};