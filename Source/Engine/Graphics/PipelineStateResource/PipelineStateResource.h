//PipelineStateResource.h
//Created 12/07/17
//Created By Daniel Bowler
//
//Base class for each pipeline state object

#pragma once

#include <Config/EngineConfig.h> //Build settings

#ifdef ENGINE_CONFIG_GRAPHICS_API_D3D11
#include "D3D11/D3D11PipelineStateResource.h"
#endif

namespace EngineAPI
{
	namespace Graphics
	{
		class PipelineStateResource : public RENDERING_PLATFORM_IMPLEMENTATION(PipelineStateResource)
		{
		public:
			PipelineStateResource() {};
			virtual ~PipelineStateResource() = 0 {};
		};
	};
};