//BufferResource.h
//Created 10/07/17
//Created By Daniel Bowler
//
//Common parent class of buffer objects - Eg: VB, IB, stream-out buffers
//etc

#pragma once

#include "../../Config/EngineConfig.h" //Build settings

#ifdef ENGINE_CONFIG_GRAPHICS_API_D3D11
#include "D3D11\D3D11BufferResource.h"
#endif

namespace EngineAPI
{
	namespace Graphics
	{
		class BufferResource : public RENDERING_PLATFORM_IMPLEMENTATION(BufferResource)
		{
		public:
			BufferResource() {};
			virtual ~BufferResource() = 0 {};
		};
	};
};