//PixelShader.h
//Created 11/07/17
//Created By Daniel Bowler
//
//Raw pixel/fragment shader

#pragma once

#include "../../Config/EngineConfig.h" //Build settings

#ifdef ENGINE_CONFIG_GRAPHICS_API_D3D11
#include "D3D11\D3D11PixelShader.h"
#endif

namespace EngineAPI
{
	namespace Graphics
	{
		class PixelShader : public RENDERING_PLATFORM_IMPLEMENTATION(PixelShader)
		{
		public:
			PixelShader() {};
			virtual ~PixelShader() {};
		};
	};
};