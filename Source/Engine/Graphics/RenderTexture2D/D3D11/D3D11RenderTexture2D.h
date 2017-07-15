//D3D11RenderTexture2D.h
//Created 14/07/17
//Created By Daniel Bowler
//
//Represents a 2D Texture that we can render in to.

#pragma once

//Parent
#include <Core/CoreObject/CoreObject.h>

//Manages a Texture2D object
#include <Graphics/Texture2D/Texture2D.h>

namespace EngineAPI
{
	namespace Graphics
	{
		namespace Platform
		{
			class D3D11RenderTexture2D : public EngineAPI::Core::CoreObject
			{
			public:
				D3D11RenderTexture2D() {};
				virtual ~D3D11RenderTexture2D() = 0 {};

				//Inits the render texture
				bool InitRenderTexture2D(EngineAPI::Graphics::GraphicsDevice* device,
					uint32_t textureWidth, uint32_t textureHeight, uint32_t msaaSampleCount = 1,
					void* initialData = nullptr,
					ResourceFormat textureFormat = RESOURCE_FORMAT_R8G8B8A8_UNORM,  
					ResourceUsage textureUsage = RESOURCE_USAGE_DEFAULT, 
					ResourceCPUAccessFlag textureCpuAccess = (ResourceCPUAccessFlag)0,
					ResourceBindFlag textureBindFlag = RESOURCE_BIND_RENDER_TARGET_BIT,
					std::string debugName = std::string(""));

				//Shutsdown the render texture
				virtual void Shutdown() override;

			public:
				//Gets the actual texture object
				EngineAPI::Graphics::Texture2D* GetTexture2D() { return &renderTexture2D; };

			protected:
				//The actual GPU Texture object
				EngineAPI::Graphics::Texture2D renderTexture2D;
			};
		}
	};
};