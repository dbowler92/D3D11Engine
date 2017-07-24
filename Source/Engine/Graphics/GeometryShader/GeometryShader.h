//GeometryShader.h
//Created 24/07/17
//Created By Daniel Bowler
//
//Represents a geometry shader

#pragma once

#include <Config/EngineConfig.h> //Build settings

#ifdef ENGINE_CONFIG_GRAPHICS_API_D3D11
#include "D3D11/D3D11GeometryShader.h"
#endif

namespace EngineAPI
{
	namespace Graphics
	{
		class GeometryShader : public RENDERING_PLATFORM_IMPLEMENTATION(GeometryShader)
		{
		public:
			GeometryShader() {};
			virtual ~GeometryShader() {};
		};
	};
};