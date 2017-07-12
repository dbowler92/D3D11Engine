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

				//Shutsdown the RS state object
				virtual void Shutdown() override;
			};
		};
	};
};