//Texture2D.h
//Created 10/07/17
//Created by Daniel Bowler
//
//Base class for all texture(2D) resources

#pragma once

#include "../../Config/EngineConfig.h" //Build settings

#ifdef ENGINE_CONFIG_GRAPHICS_API_D3D11
#include "D3D11\D3D11Texture2D.h"
#endif

namespace EngineAPI
{
	namespace Graphics
	{
		class Texture2D : public RENDERING_PLATFORM_IMPLEMENTATION(Texture2D)
		{
		public:
			Texture2D() {};
			virtual ~Texture2D() {};
		};
	};
};