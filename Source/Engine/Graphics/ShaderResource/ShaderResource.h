//ShaderResource.h
//Created 11/07/17
//Created By Daniel Bowler
//
//Represents a single shader (unlinked) loaded from file - currently, this
//should be pre-compiled. 

#pragma once

#include <Config/EngineConfig.h> //Build settings

#ifdef ENGINE_CONFIG_GRAPHICS_API_D3D11
#include "D3D11/D3D11ShaderResource.h"
#endif

namespace EngineAPI
{
	namespace Graphics
	{
		class ShaderResource : public RENDERING_PLATFORM_IMPLEMENTATION(ShaderResource)
		{
		public:
			ShaderResource() {};
			virtual ~ShaderResource() = 0 {};
		};
	};
};