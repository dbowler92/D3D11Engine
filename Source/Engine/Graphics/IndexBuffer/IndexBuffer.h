//IndexBuffer.h
//Created 12/07/17
//Created By Daniel Bowler
//
//Represents an index buffer - either UINT16 or UINT32

#pragma once

#include <Config/EngineConfig.h> //Build settings

#ifdef ENGINE_CONFIG_GRAPHICS_API_D3D11
#include "D3D11/D3D11IndexBuffer.h"
#endif

namespace EngineAPI
{
	namespace Graphics
	{
		class IndexBuffer : public RENDERING_PLATFORM_IMPLEMENTATION(IndexBuffer)
		{
		public:
			IndexBuffer() {};
			virtual ~IndexBuffer() {};
		};
	};
};