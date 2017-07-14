//BlendState.h
//Created 12/07/17
//Created By Daniel Bowler
//
//Blending state

#pragma once

#include <Config/EngineConfig.h> //Build settings

#ifdef ENGINE_CONFIG_GRAPHICS_API_D3D11
#include "D3D11/D3D11BlendState.h"
#endif

namespace EngineAPI
{
	namespace Graphics
	{
		class BlendState : public RENDERING_PLATFORM_IMPLEMENTATION(BlendState)
		{
		public:
			BlendState() {};
			virtual ~BlendState() {};
		};
	};
};