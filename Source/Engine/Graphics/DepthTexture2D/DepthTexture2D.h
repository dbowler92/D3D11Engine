//DepthTexture2D.h
//Created 10/07/17
//Created By Daniel Bowler
//
//Represents a depth texture - for the backbuffer depth(stencil) buffer or
//shadow maps etc.

#pragma once

#include "../../Config/EngineConfig.h" //Build settings

#ifdef ENGINE_CONFIG_GRAPHICS_API_D3D11
#include "D3D11\D3D11DepthTexture2D.h"
#endif

namespace EngineAPI
{
	namespace Graphics
	{
		class DepthTexture2D : public RENDERING_PLATFORM_IMPLEMENTATION(DepthTexture2D)
		{
		public:
			DepthTexture2D() {};
			~DepthTexture2D() {};
		};
	};
};