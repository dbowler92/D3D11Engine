//BaseViewResource.h
//Created 14/07/17
//Created By Daniel Bowler
//
//API agnostic base for each view resource -> *API*ViewResource
//will inherit from this

#pragma once

//Parent class
#include "../../Core/CoreObject/CoreObject.h"

//Graphics includes - inc D3D11 / GL etc
#include "../../Includes/GraphicsIncludes.h"

//Device used to create texture, buffers && views
#include "../../Graphics/GraphicsDevice/GraphicsDevice.h"

namespace EngineAPI
{
	namespace Graphics
	{
		class BaseViewResource : public EngineAPI::Core::CoreObject
		{
		public:
			BaseViewResource() {};
			virtual ~BaseViewResource() = 0 {};

			//Virtual functions that will be inherited by all view objects
			//
			//Shutdown function - we will ensure to call up the inheritance chain
			virtual void Shutdown() override = 0;
		};
	};
};