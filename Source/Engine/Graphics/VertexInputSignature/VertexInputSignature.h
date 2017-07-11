//D3D11VertexInputSignature.h
//Created 11/07/17
//Created By Daniel Bowler
//
//Describes how we will map data from a vertex buffer (or buffers) to
//shader input - per vertex and per instance data

#pragma once

#include "../../Config/EngineConfig.h" //Build settings

#ifdef ENGINE_CONFIG_GRAPHICS_API_D3D11
#include "D3D11\D3D11VertexInputSignature.h"
#endif

namespace EngineAPI
{
	namespace Graphics
	{
		class VertexInputSignature : public RENDERING_PLATFORM_IMPLEMENTATION(VertexInputSignature)
		{
		public:
			VertexInputSignature() {};
			virtual ~VertexInputSignature() {};
		};
	};
};
