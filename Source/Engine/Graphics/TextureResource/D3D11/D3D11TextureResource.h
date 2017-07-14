//D3D11TextureResource.h
//Created 10/07/17
//Created By Daniel Bowler
//
//Common parent class of all D3D11 texture objects

#pragma once

//Parent class
#include <Graphics/CommonResource/CommonResource.h>

//DDS Texture loading in to an ID3D11* resource
#include <3rdParty/DDSTextureLoader/DDSTextureLoader.h>

namespace EngineAPI
{
	namespace Graphics
	{
		namespace Platform
		{
			class D3D11TextureResource : public EngineAPI::Graphics::CommonResource
			{
			public:
				D3D11TextureResource() {};
				virtual ~D3D11TextureResource() = 0 {};

				//Textures should override shutdown
				virtual void Shutdown() override = 0;

				//Map the textures for read/write operations - Should be 
				//implemented by subclasses -> Eg: Texture2D, Texture3D etc
				virtual bool MapResource(EngineAPI::Graphics::GraphicsDevice* device,
					UINT subresourceIndex, ResourceMappingMode mapMode, MappedResourceData* mappedResourceOut) override = 0;
				virtual void UnmapResource(EngineAPI::Graphics::GraphicsDevice* device, UINT subresourceIndex) override = 0;
			};
		};
	};
};