//BaseResource.h
//Created 14/04/17
//Created By Daniel Bowler
//
//Top of the graphics resource inheritance tree - *API*CommonResource
//inherits from this (which stores engine agnostic data && provides the 
//virtual functions that resources must implement at somepoint). This class
//will inherit from the API agnostic CoreObject

#pragma once

//Parent class
#include <Core/CoreObject/CoreObject.h>

//Graphics includes - inc D3D11 / GL etc
#include <Includes/GraphicsIncludes.h>

//Device used to create texture, buffers && views
#include <Graphics/GraphicsDevice/GraphicsDevice.h>

namespace EngineAPI
{
	namespace Graphics
	{
		class BaseResource : public EngineAPI::Core::CoreObject
		{
		public:
			BaseResource() {};
			virtual ~BaseResource() = 0 {};

			//Virtual functions that will be inherited by all resources
			//
			//Shutdown function - we will ensure to call up the inheritance chain
			virtual void Shutdown() override = 0;

			//Map and unmap functionality for resources for CPU reads and writes
			virtual bool MapResource(EngineAPI::Graphics::GraphicsDevice* device,
				UINT subresourceIndex, ResourceMappingMode mapMode, MappedResourceData* mappedResourceOut) = 0;
			virtual void UnmapResource(EngineAPI::Graphics::GraphicsDevice* device,
				UINT subresourceIndex) = 0;

			//CopyResource and CopySubresourceRegion functionality for resources which
			//are staging resources -> Data can be copied from staging buffers to GPU
			//visible resources

			//UpdateSubresource and UpdateSubresourceRegion functionality for default 
			//resources - this can be used to update default buffers with new data.

		public:
			//Getters:
			ResourceType GetResourceType() { return resourceType; };
			ResourceUsage GetResourceUsage() { return resourceUsage; };
			ResourceCPUAccessFlag GetResourceCPUAccessFlag() { return resourceCPUAccessFlag; };
			ResourceBindFlag GetResourceBindingFlag() { return resourceBindingFlag; };

			//Is the resource currently mapped?
			bool IsResourceCurrentlyMapped() { return isResourceCurrentlyMapped; };

		protected:
			//Init the base resource data - Should be called by texture resource
			//and buffer resource
			void InitBaseResourceUsageData(ResourceType type,
				ResourceUsage resourceUsage, ResourceCPUAccessFlag cpuAccess, ResourceBindFlag resourceBindingFlag);

			//Verifies if this resource can actually be mapped
			bool CanPerformMapOperation(ResourceMappingMode mapMode);

		protected:
			//Set this to true when the resource has been successfully mapped
			bool isResourceCurrentlyMapped = false;

		private:
			//API agnostic data
			ResourceType resourceType = RESOURCE_TYPE_UNDEFINED;
			ResourceUsage resourceUsage;
			ResourceCPUAccessFlag resourceCPUAccessFlag;
			ResourceBindFlag resourceBindingFlag;
		};
	};
};