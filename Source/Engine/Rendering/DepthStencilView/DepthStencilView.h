//DepthStencilView.h
//Created 10/07/17
//Created By Daniel Bowler
//
//Interface to bind depth textures as output.

#pragma once

#include "../../Config/EngineConfig.h" //Build settings

#ifdef ENGINE_CONFIG_GRAPHICS_API_D3D11
#include "D3D11\D3D11DepthStencilView.h"
#endif

namespace EngineAPI
{
	namespace Rendering
	{
		class DepthStencilView : public RENDERING_PLATFORM_IMPLEMENTATION(DepthStencilView)
		{
		public:
			DepthStencilView() {};
			~DepthStencilView() {};
		};
	};
};