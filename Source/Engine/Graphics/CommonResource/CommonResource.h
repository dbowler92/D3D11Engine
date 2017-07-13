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

namespace EngineAPI
{
	namespace Graphics
	{
		class CommonResource : public RENDERING_PLATFORM_IMPLEMENTATION(CommonResource)
		{
		public:
			CommonResource() {};
			virtual ~CommonResource() = 0 {};

			//Shutdown the common resource data
			virtual void Shutdown() override = 0;

			//Virtual mapping functions - implemented by BufferResource
			//and each texture type (Texture2D, Texture3D etc. We could put this
			//in TextureResource && ask that to cache a pointer to the ID3D11Resource
			//that represents the texture...)
			virtual bool MapResource(EngineAPI::Graphics::GraphicsDevice* device,
				UINT subresourceIndex, ResourceMappingMode mapMode, MappedResourceData* mappedResourceOut) = 0;
			virtual void UnmapResource(EngineAPI::Graphics::GraphicsDevice* device, 
				UINT subresourceIndex) = 0;
			
		public:
			//Get resource usage data
			ResourceType GetResourceType() { return resourceType; };
			ResourceUsage GetResourceUsage() { return resourceUsage; };
			ResourceCPUAccessFlag GetResourceCPUAccessFlag() { return resourceCPUAccessFlag; };
			ResourceBindFlag GetResourceBindingFlag() { return resourceBindingFlag; };

			//Is the resource currently mapped?
			bool IsResourceCurrentlyMapped() { return isResourceCurrentlyMapped; };

		protected:
			//Inits the common resource data - Should be called by texture resource
			//and buffer resource
			void InitCommonResourceUsageData(ResourceType type,
				ResourceUsage resourceUsage, ResourceCPUAccessFlag cpuAccess, ResourceBindFlag resourceBindingFlag);
		
		protected:
			//Verifies if this resource can actually be mapped
			bool CanPerformMapOperation(ResourceMappingMode mapMode);

		protected:
			//Set this to true when the resource has been successfully mapped
			bool isResourceCurrentlyMapped = false;

		private:
			ResourceType resourceType = RESOURCE_TYPE_UNDEFINED;
			ResourceUsage resourceUsage;
			ResourceCPUAccessFlag resourceCPUAccessFlag;
			ResourceBindFlag resourceBindingFlag;
		};
	};
};