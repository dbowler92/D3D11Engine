//VertexShader.h
//Created 11/07/17
//Created By Daniel Bowler
//
//Raw vertex shader

#pragma once

#include "../../Config/EngineConfig.h" //Build settings

#ifdef ENGINE_CONFIG_GRAPHICS_API_D3D11
#include "D3D11\D3D11VertexShader.h"
#endif

namespace EngineAPI
{
	namespace Graphics
	{
		class VertexShader : public RENDERING_PLATFORM_IMPLEMENTATION(VertexShader)
		{
		public:
			VertexShader() {};
			virtual ~VertexShader() {};
		};
	};
};