//HullShader.h
//Created 24/07/17
//Created By Daniel Bowler
//
//Programmable hull shader. 

#pragma once

#include <Config/EngineConfig.h> //Build settings

#ifdef ENGINE_CONFIG_GRAPHICS_API_D3D11
#include "D3D11/D3D11HullShader.h"
#endif

namespace EngineAPI
{
	namespace Graphics
	{
		class HullShader : public RENDERING_PLATFORM_IMPLEMENTATION(HullShader)
		{
		public:
			HullShader() {};
			virtual ~HullShader() {};
		};
	};
};