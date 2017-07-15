//TextureFile2D.h
//Created 15/07/17
//Created By Daniel Bowler
//
//Texture2D that supports loading from file

#pragma once

#include <Config/EngineConfig.h> //Build settings

#ifdef ENGINE_CONFIG_GRAPHICS_API_D3D11
#include "D3D11/D3D11TextureFile2D.h"
#endif

namespace EngineAPI
{
	namespace Graphics
	{
		class TextureFile2D : public RENDERING_PLATFORM_IMPLEMENTATION(TextureFile2D)
		{
		public:
			TextureFile2D() {};
			~TextureFile2D() {};
		};
	};
};