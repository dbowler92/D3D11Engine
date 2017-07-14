//GraphicsSwapchain.h
//Created 09/07/17
//Created By Daniel Bowler
//
//Interface to the swapchain + back buffer mechanisms. 

#pragma once

#include <Config/EngineConfig.h> //Build settings

#ifdef ENGINE_CONFIG_GRAPHICS_API_D3D11
#include "D3D11/D3D11GraphicsSwapchain.h"
#endif

namespace EngineAPI
{
	namespace Graphics
	{
		class GraphicsSwapchain : public RENDERING_PLATFORM_IMPLEMENTATION(GraphicsSwapchain)
		{
		public:
			GraphicsSwapchain() {};
			~GraphicsSwapchain() {};
		};
	};
};