//ShaderResourceView.h
//Created 15/07/17
//Created By Daniel Bowler
//
//Represents a view to a resource that can be bound to to the 
//pipeline for shader access (Eg: A texture map)

#pragma once

#include <Config/EngineConfig.h> //Build settings

#ifdef ENGINE_CONFIG_GRAPHICS_API_D3D11
#include "D3D11/D3D11ShaderResourceView.h"
#endif

namespace EngineAPI
{
	namespace Graphics
	{
		class ShaderResourceView : public RENDERING_PLATFORM_IMPLEMENTATION(ShaderResourceView)
		{
		public:
			ShaderResourceView() {};
			~ShaderResourceView() {};
		};
	};
};