//RenderTexture2D.h
//Created 14/07/17
//Created By Daniel Bowler
//
//Represents a 2D Texture that we can render in to (with an RTV) - Eg: on render target
//in the GBuffer for deferred rendering

#pragma once

#include <Config/EngineConfig.h> //Build settings

#ifdef ENGINE_CONFIG_GRAPHICS_API_D3D11
#include "D3D11/D3D11RenderTexture2D.h"
#endif

namespace EngineAPI
{
	namespace Graphics
	{
		class RenderTexture2D : public RENDERING_PLATFORM_IMPLEMENTATION(RenderTexture2D)
		{
		public:
			RenderTexture2D() {};
			~RenderTexture2D() {};
		};
	};
};