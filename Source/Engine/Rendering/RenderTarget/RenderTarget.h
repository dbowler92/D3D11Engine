//RenderTarget.h
//Created 17/07/17
//Created By Daniel Bowler
//
//A higher level class that manages a single render target and
//its view(s)

#pragma once

//Parent
#include <Core/CoreObject/CoreObject.h>

//Graphics objects
#include <Graphics/RenderTexture2D/RenderTexture2D.h>
#include <Graphics/DepthStencilTexture2D/DepthStencilTexture2D.h>
#include <Graphics/RenderTargetView/RenderTargetView.h>
#include <Graphics/ShaderResourceView/ShaderResourceView.h>
#include <Graphics/DepthStencilView/DepthStencilView.h>

namespace EngineAPI
{
	namespace Rendering
	{
		class RenderTarget : public EngineAPI::Core::CoreObject
		{
		public:
			//Default constructor and destructor
			RenderTarget() {};
			~RenderTarget() {};

			//Override the shutdown function
			void Shutdown() override;

			//Override the SetDebugName function so that we can
			//set the debug names of our resources
			void SetDebugName(std::string s) override;

			//
			//Inits the render target (and optional depth/stencil texture) and views
			//
			//Standard render target
			bool InitRenderTarget(uint32_t width, uint32_t height, 
				ResourceFormat textureFormat, bool isUsedAsShaderResource,
				std::string debugName = std::string(""));

			//MSAA Render target
			bool InitMSAARenderTarget(uint32_t width, uint32_t height,
				uint32_t msaaSampleCount, ResourceFormat textureFormat,
				bool isUsedAsShaderResource,
				std::string debugName = std::string(""));

			//Standard render target + depth stencil texture
			bool InitRenderTargetWithDepthStencilTexture(uint32_t width, uint32_t height,
				ResourceFormat renderTargetTextureFormat, DepthStencilTextureFormat depthStencilTextureFormat,
				bool isUsedAsShaderResource,
				std::string debugName = std::string(""));

			//MSAA Render target + depth stencil texture
			bool InitMSAARenderTargetWithDepthStencilTexture(uint32_t width, uint32_t height,
				uint32_t msaaSampleCount, 
				ResourceFormat renderTargetTextureFormat, DepthStencilTextureFormat depthStencilTextureFormat,
				bool isUsedAsShaderResource,
				std::string debugName = std::string(""));

			//
			//TODO: When creating the RenderTarget with depth/stencil texture, 
			//allow the user to pass a custom SRV format for the depth texture to
			//enable DEPTH_STENCIL_FORMAT_R32_FLOAT to be read as RESOURCE_FORMAT_R32_UNORM
			//by a shader?
			//

			//
			//TODO: Support typeless formats by splitting up the texture and view
			//creation code!
			//

			//
			//Destroys and recreates the render target (and depth stencil buffer)
			//with a new size
			//
			bool Resize(uint32_t newWidth, uint32_t newHeight);

			//
			//Clearing
			//
			void ClearRenderTarget(Float32Colour clearColour);
			void ClearDepthStencilTexture(DepthStencilClearFlag clearFlag,
				float depthClear = 1.0f, uint8_t stencilClear = 0);

			//
			//Pipeline binding - Ouput
			//
			void BindRenderTargetOnlyAsOutput();
			void BindDepthStencilTextureOnlyAsOutput(bool readWriteDepthTexture = true);
			void BindRenderTargetAndDepthStencilTextureAsOutput(bool readWriteDepthTexture = true);
			void BindRenderTargetWithExternalDepthStencilViewAsOutput(
				EngineAPI::Graphics::DepthStencilView* externalDSV);

			//
			//TODO: Pipeline Binding - Make sure to be able to bind only the colour render
			//target. Make sure to bind just the depth buffer. etc
			//

			//
			//TODO: Bind as shader resource
			//

		public:
			//Getters:
			//
			bool DoesManageADepthStencilTexture() { return doesManageADepthStencilTexture; };
			
			uint32_t GetRenderTargetWidth() { return renderTargetWidth; };
			uint32_t GetRenderTargetHeight() { return renderTargetHeight; };
			uint32_t GetRenderTargetMSAASampleCount() { return msaaSampleCount; };

			ResourceFormat GetRenderTargetTextureFormat() { return renderTargetTextureFormat; };
			DepthStencilTextureFormat GetDepthStencilTextureFormat() { return renderTargetDepthStencilTextureFormat; };
			bool CanBeUsedAsShaderResource() { return isUsedAsShaderResource; };

			EngineAPI::Graphics::RenderTexture2D* GetRenderTargetTexture2D() { return &renderTargetTexture2D; };
			EngineAPI::Graphics::DepthStencilTexture2D* GetDepthStencilTexture2D() { return ((doesManageADepthStencilTexture == true) ? &depthStencilTexture2D : nullptr); };

			EngineAPI::Graphics::RenderTargetView* GetRenderTargetView() { return &renderTargetRTV; };
			EngineAPI::Graphics::ShaderResourceView* GetRenderTargetShaderResourceView() { return ((isUsedAsShaderResource == true) ? &renderTargetSRV : nullptr); };

			EngineAPI::Graphics::DepthStencilView* GetDepthStencilViewReadWrite() { return ((doesManageADepthStencilTexture == true) ? &depthStencilDSV : nullptr); };
			EngineAPI::Graphics::DepthStencilView* GetDepthStencilViewReadOnly() { return ((doesManageADepthStencilTexture == true) ? &depthStencilReadOnlyDSV : nullptr); };
			EngineAPI::Graphics::ShaderResourceView* GetDepthStencilTextureShaderResourceView() { return ((doesManageADepthStencilTexture == true && isUsedAsShaderResource == true) ? &depthStencilSRV : nullptr); };

		private:
			//Render target texture
			EngineAPI::Graphics::RenderTexture2D renderTargetTexture2D;

			//Depth stencil texture we can create alongside the render target
			EngineAPI::Graphics::DepthStencilTexture2D depthStencilTexture2D;

			//Views to the render target texture
			EngineAPI::Graphics::RenderTargetView renderTargetRTV;
			EngineAPI::Graphics::ShaderResourceView renderTargetSRV;

			//Views to the depth stencil buffer
			EngineAPI::Graphics::DepthStencilView depthStencilDSV;
			EngineAPI::Graphics::DepthStencilView depthStencilReadOnlyDSV; //Depth test (no writes) + sample depth buffer in shader - Used in lighting pass for deferred rendering
			EngineAPI::Graphics::ShaderResourceView depthStencilSRV;

		private:
			//Init state
			bool isRenderTargetTextureAndViewsInited = false;
			bool isDepthStencilTextureAndViewsInited = false;
			
			//Render target data
			uint32_t renderTargetWidth = 0;  //Depth tex is the same size
			uint32_t renderTargetHeight = 0; //Depth tex is the same size
			uint32_t msaaSampleCount = 1;	 //Depth tex is created with same MSAA sample count
			ResourceFormat renderTargetTextureFormat;
			DepthStencilTextureFormat renderTargetDepthStencilTextureFormat;
			bool isUsedAsShaderResource = false;
			bool doesManageADepthStencilTexture = false;

		private:
			//Internal function which inits the depth stencil texture & views
			//if required
			bool InitDepthStencilTextureAndViews(uint32_t width, uint32_t height, uint32_t msaaSampleCount,
				DepthStencilTextureFormat depthTexFormat, bool isUsedAsShaderResource, 
				std::string debugName);
		};
	}
}