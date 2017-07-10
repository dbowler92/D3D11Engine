//RenderTargetView.h
//Created 10/04/17
//Created By Daniel Bowler
//
//Way of binding a texture to be rendered in to.

#pragma once

#include "../../Config/EngineConfig.h" //Build settings

#ifdef ENGINE_CONFIG_GRAPHICS_API_D3D11
#include "D3D11\D3D11RenderTargetView.h"
#endif

namespace EngineAPI
{
	namespace Graphics
	{
		class RenderTargetView : public RENDERING_PLATFORM_IMPLEMENTATION(RenderTargetView)
		{
		public:
			RenderTargetView() {};
			~RenderTargetView() {};
		};
	};
};