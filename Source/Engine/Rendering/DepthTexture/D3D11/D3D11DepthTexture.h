//D3D11DepthTexture.h
//Created 10/07/17
//Created By Daniel Bowler
//
//Represents a D3D11 depth texture

#pragma once

//Is a Texture2D object at heart!
#include "../../Texture2D/Texture2D.h"

enum DepthStencilTextureFormat
{
	DEPTH_STENCIL_FORMAT_D16_UNORM,			//Shadow map
	DEPTH_STENCIL_FORMAT_D24_UNORM_S8_UINT,	//Default depth/stencil format
	DEPTH_STENCIL_FORMAT_D32_FLOAT			
};

namespace EngineAPI
{
	namespace Rendering
	{
		namespace Platform
		{
			class D3D11DepthTexture : public EngineAPI::Rendering::Texture2D
			{
			public:
				D3D11DepthTexture() {};
				virtual ~D3D11DepthTexture() = 0 {};

				//Inits the depth texture
				bool InitDepthTexture(EngineAPI::Graphics::GraphicsDevice* device, 
					uint32_t textureWidth, uint32_t textureHeight, 
					DepthStencilTextureFormat textureFormat  = DEPTH_STENCIL_FORMAT_D24_UNORM_S8_UINT, 
					uint32_t msaaSampleCount = 1,
					bool willBeUsedAsShaderSamplerInput = false,    
					std::string debugName = std::string(""));

				//Shutsdown the depth texture
				void Shutdown();

			public:
				//Getters
				DepthStencilTextureFormat GetDepthTextureFormat() { return depthTextureFormat; };
				bool CanCreateShaderResourceViewForDepthTexture() { return willBeUsedAsShaderSamplerInput; };

			protected:
				//My engine depth/stencil texture format.
				DepthStencilTextureFormat depthTextureFormat;

				//Will we be using this texture as shader input (eg: Shadow map)?
				bool willBeUsedAsShaderSamplerInput = false;
			};
		}
	};
};