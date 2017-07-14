//TextureResource.h
//Created 10/07/17
//Created By Daniel Bowler
//
//Common parent class of all texture objects

#pragma once

#include <Config/EngineConfig.h> //Build settings

#ifdef ENGINE_CONFIG_GRAPHICS_API_D3D11
#include "D3D11/D3D11TextureResource.h"
#endif

namespace EngineAPI
{
	namespace Graphics
	{
		class TextureResource : public RENDERING_PLATFORM_IMPLEMENTATION(TextureResource)
		{
		public:
			TextureResource() {};
			virtual ~TextureResource() = 0 {};
		};
	};
};