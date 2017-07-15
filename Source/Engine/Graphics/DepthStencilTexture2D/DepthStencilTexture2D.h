//DepthStencilTexture2D .h
//Created 10/07/17
//Created By Daniel Bowler
//
//Represents a depth & stencil texture - for the backbuffer depth(stencil) buffer or
//shadow maps etc.

#pragma once

#include <Config/EngineConfig.h> //Build settings

#ifdef ENGINE_CONFIG_GRAPHICS_API_D3D11
#include "D3D11/D3D11DepthStencilTexture2D.h"
#endif

namespace EngineAPI
{
	namespace Graphics
	{
		class DepthStencilTexture2D : public RENDERING_PLATFORM_IMPLEMENTATION(DepthStencilTexture2D)
		{
		public:
			DepthStencilTexture2D() {};
			~DepthStencilTexture2D() {};
		};
	};
};