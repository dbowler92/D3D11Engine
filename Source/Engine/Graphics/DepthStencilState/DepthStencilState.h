//DepthStencilState.h
//Created 12/07/17
//Created By Daniel Bowler
//
//Depth stencil state (eg: depth test, stencil comparison state etc)

#pragma once

#include "../../Config/EngineConfig.h" //Build settings

#ifdef ENGINE_CONFIG_GRAPHICS_API_D3D11
#include "D3D11\D3D11DepthStencilState.h"
#endif

namespace EngineAPI
{
	namespace Graphics
	{
		class DepthStencilState : public RENDERING_PLATFORM_IMPLEMENTATION(DepthStencilState)
		{
		public:
			DepthStencilState() {};
			virtual ~DepthStencilState() {};
		};
	};
};