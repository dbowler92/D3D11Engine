//SamplerState.h
//Created 16/07/17
//Created By Daniel Bowler
//
//Instructions on how we want to sample a texture in a shader

#pragma once

#include <Config/EngineConfig.h> //Build settings

#ifdef ENGINE_CONFIG_GRAPHICS_API_D3D11
#include "D3D11/D3D11SamplerState.h"
#endif

namespace EngineAPI
{
	namespace Graphics
	{
		class SamplerState : public RENDERING_PLATFORM_IMPLEMENTATION(SamplerState)
		{
		public:
			SamplerState() {};
			virtual ~SamplerState() {};
		};
	};
};