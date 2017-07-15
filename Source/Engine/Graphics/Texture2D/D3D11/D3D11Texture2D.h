//D3D11Texture2D.h
//Created 10/07/17
//Created by Daniel Bowler
//
//Base class for all D3D11 texture(2D) resources. Eg: DepthTexture

#pragma once

//Parent class
#include <Graphics/TextureResource/TextureResource.h>

//Debug
#include <Debug/Log/DebugLog.h>

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
				
				//Shutsdown the underlying Texture2D object
				virtual void Shutdown() override = 0;
				 
				//Override the SetDebugName function() -> Set the debug name
				//of the D3D11 resource
				void SetDebugName(std::string s) override;

				//Mapping resource
				virtual bool MapResource(EngineAPI::Graphics::GraphicsDevice* device,
					UINT subresourceIndex, ResourceMappingMode mapMode, MappedResourceData* mappedResourceOut) override;
				virtual void UnmapResource(EngineAPI::Graphics::GraphicsDevice* device, 
					UINT subresourceIndex) override;
			
			public:
				//Getters 
				ID3D11Texture2D* GetD3D11Texture2DHandle() { return texture2DHandle ;};
				const D3D11_TEXTURE2D_DESC& GetD3D11Texture2DDescData() { return textureDesc; };
			
				//Specific getters
				DXGI_FORMAT GetD3D11Texture2DResourceFormat() { return textureDesc.Format; };
				bool IsMSAATexture() { return textureDesc.SampleDesc.Count > 1; };

				//Width and height of the texture
				uint32_t GetTextureWidth() { return textureDesc.Width; };
				uint32_t GetTextureHeight() { return textureDesc.Height; }; 
				
				//MSAA count and quality 
				uint32_t GetTextureMSAASampleCount()  { return textureDesc.SampleDesc.Count; };
				uint32_t GetTextureMSAAQuality() { return textureDesc.SampleDesc.Quality; };
				
			protected:
				//Texture description
				D3D11_TEXTURE2D_DESC textureDesc = {};
				D3D11_SUBRESOURCE_DATA textureInitialData = {};

				//Texture handle
				ID3D11Texture2D* texture2DHandle = nullptr;

			protected:
				//Inits the underlying texture object
				bool InitTexture2D(EngineAPI::Graphics::GraphicsDevice* device, 
					bool doInitWitInitialData, ResourceType resourceType,
					ResourceUsage resourceUsage, ResourceCPUAccessFlag cpuAccess, ResourceBindFlag resourceBindingFlag,
					std::string debugName = "");
			};
		};
	};
};