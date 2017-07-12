//D3D11PipelineStateResource.h
//Created 12/07/17
//Created By Daniel Bowler
//
//Base class for each D3D11 pipeline state object

#pragma once

//Parent class
#include "../../Graphics/CommonResource/CommonResource.h"

namespace EngineAPI
{
	namespace Graphics
	{
		namespace Platform
		{
			class D3D11PipelineStateResource : public EngineAPI::Graphics::CommonResource
			{
			public:
				D3D11PipelineStateResource() {};
				virtual ~D3D11PipelineStateResource() = 0 {};

				//Shutsdown the pipeline state
				virtual void Shutdown() = 0;
			};
		};
	};
};