//VertexBuffer.h
//Created 10/07/17
//Created By Daniel Bowler
//
//Buffer object holding per vertex && per instance data.

#pragma once

#include <Config/EngineConfig.h> //Build settings

#ifdef ENGINE_CONFIG_GRAPHICS_API_D3D11
#include "D3D11/D3D11VertexBuffer.h"
#endif

namespace EngineAPI
{
	namespace Graphics
	{
		class VertexBuffer : public RENDERING_PLATFORM_IMPLEMENTATION(VertexBuffer)
		{
		public:
			VertexBuffer() {};
			virtual ~VertexBuffer() {};
		};
	};
};