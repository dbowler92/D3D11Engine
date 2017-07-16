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
				virtual ~D3D11Texture2D(){};
				
				//Shutsdown the underlying Texture2D object
				virtual void Shutdown() override;
				 
				//Public init function for Texture objects
				//
				//General all-purpose Texture2D init function
				bool InitTexture2D(EngineAPI::Graphics::GraphicsDevice* device, 
					uint32_t textureWidth, uint32_t textureHeight, uint32_t msaaSampleCount = 1,
					uint32_t mipLevels = 1, uint32_t arraySize = 1,
					ResourceMiscFlag miscFlags = 0,
					SubResourceData* resourceInitDataPerSubresource = nullptr,
					ResourceFormat textureFormat = RESOURCE_FORMAT_R8G8B8A8_UNORM,
					ResourceUsage textureUsage = RESOURCE_USAGE_DYNAMIC,
					ResourceCPUAccessFlag textureCpuAccess = RESOURCE_CPU_ACCESS_WRITE_BIT,
					ResourceBindFlag textureBindFlag = RESOURCE_BIND_SHADER_RESOURCE_BIT,
					std::string debugName = std::string(""));

				//Loads a texture with data from file
				//
				//DDSTextureLoader: Resource will use default usage settings - IE, its an default shader resource with no CPU access. TODO:
				//Add support for the extra functions of the DDS loader (eg: auto mips generation)
				bool InitTexture2DFromDDSFile(EngineAPI::Graphics::GraphicsDevice* device, 
					std::string ddsFilePath, bool doEnableAutoMipGeneration,
					std::string debugName = std::string(""));

				//LodePNG: 
				bool InitTexture2DFromPNGFile(EngineAPI::Graphics::GraphicsDevice* device, 
					std::string pngFilePath, bool doEnableAutoMipGeneration,
					std::string debugName = std::string(""));

			public:
				//OVerride base class(es) functions:
				//
				//Override the SetDebugName function() -> Set the debug name
				//of the D3D11 resource
				void SetDebugName(std::string s) override;

				//Mapping resource
				virtual bool MapResource(EngineAPI::Graphics::GraphicsDevice* device,
					UINT subresourceIndex, ResourceMappingMode mapMode, MappedResourceData* mappedResourceOut) override;
				virtual void UnmapResource(EngineAPI::Graphics::GraphicsDevice* device, 
					UINT subresourceIndex) override;
			
				//Update the (sub)resource
				virtual void UpdateSubresourceFull(EngineAPI::Graphics::GraphicsDevice* device, 
					UINT subresourceIndex, SubResourceData* subresourceData);

			public:
				//Asks this texture to automatically generate mipmaps via D3D11
				bool AutoGenerateMipmaps(EngineAPI::Graphics::GraphicsDevice* device,
					EngineAPI::Graphics::ShaderResourceView* srv);

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
				
				//Mips
				uint32_t GetMipLevels() { return textureDesc.MipLevels; };
				uint32_t GetMostDetailedMipLevel() { return 0; }

			protected:
				//Texture description
				D3D11_TEXTURE2D_DESC textureDesc = {};

				//Texture handle
				ID3D11Texture2D* texture2DHandle = nullptr;

			private:
				//Given a resource size, calculates the number of mip levels
				//that will be generated for a full mip-map chain
				uint32_t CalculateFullMipmapChainCount(uint32_t w, uint32_t h);
			};
		};
	};
};