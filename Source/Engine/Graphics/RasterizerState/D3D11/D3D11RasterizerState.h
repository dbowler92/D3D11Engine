//D3D11RasterizerState.h
//Created 12/07/17
//Created By Daniel Bowler
//
//ID3D11RasterizerState*

#pragma once

//Parent class
#include "../../Graphics/PipelineStateResource/PipelineStateResource.h"

namespace EngineAPI
{
	namespace Graphics
	{
		namespace Platform
		{
			class D3D11RasterizerState : public EngineAPI::Graphics::PipelineStateResource
			{
			public:
				D3D11RasterizerState() {};
				virtual ~D3D11RasterizerState() = 0 {};

				//Inits the rasterizer state
				bool InitRasterizerState(EngineAPI::Graphics::GraphicsDevice* device, 
					RasterizerPipelineStateDescription* description,
					std::string debugName = std::string(""));

				//Shutsdown the RS state object
				virtual void Shutdown() override;

			public:
				//Binds the rasterizer state to the RS stage of the pipeline
				void BindRasterizerStateToPipeline(EngineAPI::Graphics::GraphicsDevice* device);

			protected:
				//Manages a ID3D11RasterizerState
				ID3D11RasterizerState* rss = nullptr;

				//Description used to init the RSS
				D3D11_RASTERIZER_DESC rssDesc = {};

			protected:
				//Fills out the rasterizer description struct (D3D11)
				void FilloutD3D11RasterizerDesc(RasterizerPipelineStateDescription* state);
			};
		};
	};
};