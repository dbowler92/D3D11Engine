//D3D11Texture2D.h
//Created 10/07/17
//Created by Daniel Bowler
//
//Base class for all D3D11 texture(2D) resources. Eg: DepthTexture

#pragma once

//Parent class
#include "../../Graphics/TextureResource/TextureResource.h"

//Graphics includes - inc D3D11
#include "../../Includes/GraphicsIncludes.h"

//Debug
#include "../../Debug/Log/DebugLog.h"

//Device used to create texture && views
#include "../../Graphics/GraphicsDevice/GraphicsDevice.h"

//D3D11_USAGE
enum ResourceUsage
{
	RESOURCE_USAGE_DEFAULT = 0,
	RESOURCE_USAGE_IMMUTABLE = 1,
	RESOURCE_USAGE_DYNAMIC = 2,
	RESOUCE_USAGE_STAGING = 3,
};

namespace EngineAPI
{
	namespace Graphics
	{
		namespace Platform
		{
			class D3D11Texture2D : public EngineAPI::Graphics::TextureResource
			{
			public:
				D3D11Texture2D() {};
				virtual ~D3D11Texture2D() = 0 {};

				//Inits the underlying texture object
				bool InitTexture2D(EngineAPI::Graphics::GraphicsDevice* device, 
					D3D11_SUBRESOURCE_DATA* textureInitData, std::string debugName = "");

				//Shutsdown the underlying texture object
				void Shutdown();

			public:
				//Getters 
				ID3D11Texture2D* GetD3D11Texture2DHandle() { return texture2DHandle ;};
				const D3D11_TEXTURE2D_DESC& GetD3D11Texture2DDescData() { return textureDesc; };
			
				//Specific getters
				DXGI_FORMAT GetD3D11Texture2DResourceFormat() { return textureDesc.Format; };
				bool IsMSAATexture() { return textureDesc.SampleDesc.Count > 1; };

			protected:
				//Texture description
				D3D11_TEXTURE2D_DESC textureDesc = {};

				//Texture handle
				ID3D11Texture2D* texture2DHandle = nullptr;
			};
		}
	};
};