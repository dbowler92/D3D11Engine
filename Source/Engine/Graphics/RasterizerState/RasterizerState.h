//RasterizerState.h
//Created 12/07/17
//Created By Daniel Bowler
//
//RS state (Eg: culling, wireframe etc)

#pragma once

#include <Config/EngineConfig.h> //Build settings

#ifdef ENGINE_CONFIG_GRAPHICS_API_D3D11
#include "D3D11/D3D11RasterizerState.h"
#endif

namespace EngineAPI
{
	namespace Graphics
	{
		class RasterizerState : public RENDERING_PLATFORM_IMPLEMENTATION(RasterizerState)
		{
		public:
			RasterizerState() {};
			virtual ~RasterizerState() {};
		};
	};
};