//DomainShader.h
//Created 24/07/17
//Created By Daniel Bowler
//
//Represents a domain shader

#pragma once

#include <Config/EngineConfig.h> //Build settings

#ifdef ENGINE_CONFIG_GRAPHICS_API_D3D11
#include "D3D11/D3D11DomainShader.h"
#endif

namespace EngineAPI
{
	namespace Graphics
	{
		class DomainShader : public RENDERING_PLATFORM_IMPLEMENTATION(DomainShader)
		{
		public:
			DomainShader() {};
			virtual ~DomainShader() {};
		};
	};
};