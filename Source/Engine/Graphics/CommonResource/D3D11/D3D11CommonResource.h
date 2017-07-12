//D3D11CommonResource.h
//Created 10/07/17
//Created By Daniel Bowler
//
//Base class that all D3D11 graphics resources (textures, buffers, views) will
//inherit from. Provides functionality that all types have in common

#pragma once

//Parent class
#include "../../Core/CoreObject/CoreObject.h"

//Graphics includes - inc D3D11
#include "../../Includes/GraphicsIncludes.h"

//Device used to create texture, buffers && views
#include "../../Graphics/GraphicsDevice/GraphicsDevice.h"

namespace EngineAPI
{
	namespace Graphics
	{
		namespace Platform
		{
			class D3D11CommonResource : public EngineAPI::Core::CoreObject
			{
			public:
				D3D11CommonResource() {};
				virtual ~D3D11CommonResource() = 0 {};

				//Maps the resource for writing
				//virtual void* MapResource(EngineAPI::Graphics::GraphicsDevice* device) = 0;

			protected:
				//Use this function to set the debug name of a D3D11 resource - sets
				//it the same string as CoreObject::_debugNameString
				void SetD3D11ResourceDebugName(ID3D11DeviceChild* resource);
			};
		};
	};
};