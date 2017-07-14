//ConstantBuffer.h
//Created 12/07/17
//Created By Daniel Bowler
//
//Represents a constant buffer resource. General data
//that can be sent to a shader. 

#pragma once

#include <Config/EngineConfig.h> //Build settings

#ifdef ENGINE_CONFIG_GRAPHICS_API_D3D11
#include "D3D11/D3D11ConstantBuffer.h"
#endif

namespace EngineAPI
{
	namespace Graphics
	{
		class ConstantBuffer : public RENDERING_PLATFORM_IMPLEMENTATION(ConstantBuffer)
		{
		public:
			ConstantBuffer() {};
			virtual ~ConstantBuffer() {};
		};
	};
};