//GraphicsDevice.h
//Created 09/07/17
//Created By Daniel Bowler
//
//Interface to the GPU

#pragma once

#include <Config/EngineConfig.h> //Build settings

#ifdef ENGINE_CONFIG_GRAPHICS_API_D3D11
#include "D3D11/D3D11GraphicsDevice.h"
#endif

namespace EngineAPI
{
	namespace Graphics
	{
		class GraphicsDevice : public RENDERING_PLATFORM_IMPLEMENTATION(GraphicsDevice)
		{
		public:
			GraphicsDevice() {};
			~GraphicsDevice() {};
		};
	};
};