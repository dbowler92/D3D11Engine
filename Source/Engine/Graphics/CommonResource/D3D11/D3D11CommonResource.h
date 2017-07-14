//D3D11CommonResource.h
//Created 10/07/17
//Created By Daniel Bowler
//
//Base class that all D3D11 graphics resources (textures, buffers, views) will
//inherit from. Provides functionality that all types have in common

#pragma once

//Parent class
#include <Graphics/BaseResource/BaseResource.h>
//#include <Core/CoreObject/CoreObject.h>

//Statics
#include <Statics/D3D11Statics/D3D11Statics.h>

namespace EngineAPI
{
	namespace Graphics
	{
		namespace Platform
		{
			class D3D11CommonResource : public EngineAPI::Graphics::BaseResource
			{
			public:
				D3D11CommonResource() {};
				virtual ~D3D11CommonResource() = 0 {};

				//Shutdown any common resource data
				virtual void Shutdown() override = 0;
			};
		};
	};
};