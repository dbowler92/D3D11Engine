//RenderTargetSet.h
//Created 18/07/17
//Created By Daniel Bowler
//
//Represents a set of render targets (I believe 8
//render targets can be bound at once in D3D11, so that is our
//cap) that can be bound to the pipeline and rendered in to. Eg:
//GBuffer in deferred rendering

#pragma once

//Parent
#include <Core/CoreObject/CoreObject.h>

//Manages a set of RenderTargets
#include <Rendering/RenderTarget/RenderTarget.h>

//Defines
#define MAX_RENDER_TARGETS_BOUND 8

namespace EngineAPI
{
	namespace Rendering
	{
		class RenderTargetSet : public EngineAPI::Core::CoreObject
		{
		public:
			RenderTargetSet() {};
			~RenderTargetSet() {};

			//Override shutdown
			void Shutdown() override;

			//Overrdie SetDebugName() so we can set the debug name
			//of our resources
			void SetDebugName(std::string s) override;

			//Inits the render target set
			bool InitRenderTargetSet(uint32_t width, uint32_t height,
				uint32_t renderTargetCount,
				ResourceFormat* textureFormatsArray, bool isUsedAsShaderResource,
				std::string debugName = std::string(""));

			//Inits the render target set. In addition, renderTargets[0] will
			//create and manage a depth texture of a given format
			bool InitRenderTargetSetWithADepthStencilTexture(uint32_t width, uint32_t height,
				uint32_t renderTargetCount,
				ResourceFormat* renderTextureFormatsArray, DepthStencilTextureFormat depthStencilFormat,
				bool isUsedAsShaderResource,
				std::string debugName = std::string(""));


			//
			//TODO: Init render target set MSAA
			//

			//
			//TODO: Resize
			//
			bool Resize(uint32_t newWidth, uint32_t newHeight);

			//
			//Clearing
			//
			void ClearRenderTarget(uint32_t targetIndex, Float32Colour clearColour);
			void ClearAllRenderTargets(Float32Colour clearColour);
			void ClearAllRenderTargets(Float32Colour* clearColoursArray);
			void ClearDepthStencilTexture(DepthStencilClearFlag depthStencilClearFlag,
				float depthClear = 1.0f, UINT8 stencilClear = 0);

			//
			//Pipeline binding
			//
			//Binds a single render target (targetIndex) - null depth
			void BindRenderTargetAtIndexAsOutput(uint32_t targetIndex);

			//Binds a single render target (targetIndex) with the managed
			//depth/stencil texture
			void BindRenderTargetAtIndexAndDepthStencilTextureAsOutput(uint32_t targetIndex, bool doUseReadWriteDepth = true);

			//Binds a single render target (targetIndex) with an externally
			//managed depth/stencil texture
			void BindRenderTargetAtIndexWithExternalDepthStencilViewAsOutput(uint32_t targetIndex,
				EngineAPI::Graphics::DepthStencilView* externalDSV);

			//Binds all render targets as output - without the depth/stencil
			//texture - null depth
			void BindAllRenderTargetsAsOutput();

			//Binds all render textures and the managed depth/stencil texture
			void BindAllRenderTargetsAndDepthStencilTextureAsOutput(bool doUseReadWriteDepth = true);

			//Binds all render textures and an externally managed depth/stencil
			//texture
			void BindAllRenderTargetsAndExternalDepthStencilViewAsOutput(
				EngineAPI::Graphics::DepthStencilView* externalDSV);

		public:
			//Getters
			uint32_t GetRenderTargetsCount() { return renderTargetsCount; };
			bool DoesManageDepthStencilTexture() { return doesManageDepthBuffer ; };

			//Each render target - 0 based index here
			EngineAPI::Rendering::RenderTarget* GetRenderTargetAtIndex(uint32_t index)  {  return ((index < renderTargetsCount) ? &renderTargets[index] : nullptr); };

			//Gets the render target responsible for managing the depth
			//stencil texture. Nullptr if no depth/stencil texture was created
			EngineAPI::Rendering::RenderTarget* GetRenderTargetThatManagesDepthStencilTexture() { return ((doesManageDepthBuffer) ? &renderTargets[0] : nullptr); };
	
		private:
			//Array of render targets 
			EngineAPI::Rendering::RenderTarget renderTargets[MAX_RENDER_TARGETS_BOUND];
		
			//Is inited?
			bool isInited = false;
			
			//Do we manage a depth texture in renderTargets[0]
			bool doesManageDepthBuffer = false;

			//Set data
			uint32_t renderTargetsCount = 0;
		};
	};
};
