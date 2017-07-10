//D3D11DepthTexture.h
//Created 10/07/17
//Created By Daniel Bowler
//
//Represents a depth texture - for the backbuffer depth(stencil) buffer or
//shadow maps etc.

#pragma once

#include "../../Config/EngineConfig.h" //Build settings

#ifdef ENGINE_CONFIG_GRAPHICS_API_D3D11
#include "D3D11\D3D11DepthTexture.h"
#endif

namespace EngineAPI
{
	namespace Rendering
	{
		class DepthTexture : public RENDERING_PLATFORM_IMPLEMENTATION(DepthTexture)
		{
		public:
			DepthTexture() {};
			~DepthTexture() {};
		};
	};
};