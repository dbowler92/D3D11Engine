//CommonResource.h
//Created 10/07/17
//Created By Daniel Bowler
//
//Base class that all graphics resources (textures, buffers, views) will
//inherit from. Provides functionality that all types have in common - eg:
//a debug name

#pragma once

#include "../../Config/EngineConfig.h" //Build settings

#ifdef ENGINE_CONFIG_GRAPHICS_API_D3D11
#include "D3D11\D3D11CommonResource.h"
#endif

//Same as D3D11_USAGE
enum ResourceUsage
{
	RESOURCE_USAGE_DEFAULT = 0,			//GPU read-write
	RESOURCE_USAGE_IMMUTABLE = 1,	    //GPU read-only -> Data set at init time
	RESOURCE_USAGE_DYNAMIC = 2,	        //CPU write -> Via Map()
	RESOUCE_USAGE_STAGING = 3,			//CPU read.
};

namespace EngineAPI
{
	namespace Graphics
	{
		class CommonResource : public RENDERING_PLATFORM_IMPLEMENTATION(CommonResource)
		{
		public:
			CommonResource() {};
			virtual ~CommonResource() = 0 {};
		};
	};
};