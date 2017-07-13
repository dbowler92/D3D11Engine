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
#include "../../Core/CoreObject/CoreObject.h"

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

			//CopyResource and CopySubresourceRegion functionality for resources which
			//are staging resources -> Data can be copied from staging buffers to GPU
			//visible resources

			//UpdateSubresource and UpdateSubresourceRegion functionality for default 
			//resources - this can be used to update default buffers with new data.

		};
	};
};