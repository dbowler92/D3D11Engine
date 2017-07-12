//D3D11BlendState.h
//Created 12/07/17
//Created By Daniel Bowler
//
//ID3D11BlendState

#pragma once

//Parent class
#include "../../Graphics/PipelineStateResource/PipelineStateResource.h"

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

				//Shutsdown the DSS state object
				virtual void Shutdown() override;
			};
		};
	};
};