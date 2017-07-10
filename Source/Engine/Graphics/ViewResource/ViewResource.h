//ViewResource.h
//Created 10/07/17
//Created By Daniel Bowler
//
//Common parent class of view objects - they are used when interacting with
//the pipeline rather than the raw resources themselves (D3D11 style)

#pragma once

#include "../../Config/EngineConfig.h" //Build settings

#ifdef ENGINE_CONFIG_GRAPHICS_API_D3D11
#include "D3D11\D3D11ViewResource.h"
#endif

namespace EngineAPI
{
	namespace Graphics
	{
		class ViewResource : public RENDERING_PLATFORM_IMPLEMENTATION(ViewResource)
		{
		public:
			ViewResource() {};
			virtual ~ViewResource() = 0 {};
		};
	};
};