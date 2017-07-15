//D3D11DepthTexture2D.h
//Created 10/07/17
//Created By Daniel Bowler
//
//Represents a single D3D11 depth texture

#pragma once

//Parent
#include <Core/CoreObject/CoreObject.h>

//Manages a Texture2D
#include <Graphics/Texture2D/Texture2D.h>

enum DepthStencilTextureFormat
{
	DEPTH_STENCIL_FORMAT_D16_UNORM,			//Shadow map
	DEPTH_STENCIL_FORMAT_D24_UNORM_S8_UINT,	//Default depth/stencil format
	DEPTH_STENCIL_FORMAT_D32_FLOAT			
};

namespace EngineAPI
{
	namespace Graphics
	{
		namespace Platform
		{
			class D3D11DepthTexture2D : public EngineAPI::Core::CoreObject
			{
			public:
				D3D11DepthTexture2D() {};
				virtual ~D3D11DepthTexture2D() = 0 {};

				//Inits the depth texture
				bool InitDepthTexture2D(EngineAPI::Graphics::GraphicsDevice* device, 
					uint32_t textureWidth, uint32_t textureHeight, uint32_t msaaSampleCount = 1,
					DepthStencilTextureFormat textureFormat  = DEPTH_STENCIL_FORMAT_D24_UNORM_S8_UINT, 
					bool willBeUsedAsShaderSamplerInput = false,    
					std::string debugName = std::string(""));

				//Shutsdown the depth texture
				virtual void Shutdown() override;

			public:
				//Gets the underlying Texture2D object we are managing
				EngineAPI::Graphics::Texture2D* GetTexture2D() { return &depthStencilTexture2D; };

				//API Agnostic info
				DepthStencilTextureFormat GetDepthTextureFormat() { return depthTextureFormat; };
				bool CanCreateShaderResourceViewForDepthTexture() { return willBeUsedAsShaderSamplerInput; };

			protected:
				//The Texture2D we manage
				EngineAPI::Graphics::Texture2D depthStencilTexture2D;

				//My engine depth/stencil texture format.
				DepthStencilTextureFormat depthTextureFormat;

				//Will we be using this texture as shader input (eg: Shadow map)?
				bool willBeUsedAsShaderSamplerInput = false;
			};
		}
	};
};