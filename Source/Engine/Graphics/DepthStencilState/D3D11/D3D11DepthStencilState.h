//D3D11DepthStencilState.h
//Created 12/07/17
//Created By Daniel Bowler
//
//Represents the depth && stencil state object in D3D11

#pragma once

//Parent class
#include <Graphics/PipelineStateResource/PipelineStateResource.h>

namespace EngineAPI
{
	namespace Graphics
	{
		namespace Platform
		{
			class D3D11DepthStencilState : public EngineAPI::Graphics::PipelineStateResource
			{
			public:
				D3D11DepthStencilState() {};
				virtual ~D3D11DepthStencilState() = 0 {};
				
				//Inits the depth stencil state
				bool InitDepthStencilState(EngineAPI::Graphics::GraphicsDevice* device,
					DepthStencilPipelineStateDescription* description,
					std::string debugName = std::string(""));

				//Shutsdown the DSS state object
				virtual void Shutdown() override;

			public:
				//Bind to OM stage of the pipeline
				void BindDepthStencilStateToPipeline(EngineAPI::Graphics::GraphicsDevice* device, UINT stencilRef);

			protected:
				//Manages an ID3D11DepthStencilState object
				ID3D11DepthStencilState* dss = nullptr;

				//State description - D3D11
				D3D11_DEPTH_STENCIL_DESC dssDesc = {};

			protected:
				//Fills the D3D11_DEPTH_STENCIL_DESC struct based on the platform agnostic
				//DepthStencilPipelineStateDescription struct
				void FilloutD3D11DepthStencilDesc(DepthStencilPipelineStateDescription* state);
			};
		};
	};
};