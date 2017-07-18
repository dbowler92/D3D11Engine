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
			//TODO: Clearing
			//

			//
			//TODO: Pipeline binding
			//
			

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
