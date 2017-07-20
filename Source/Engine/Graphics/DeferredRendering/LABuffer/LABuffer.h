//LABuffer.h
//Created 19/07/17
//created By Daniel Bowler
//
//LA Buffer used in deferred rendering (Light Accumulation) 

#pragma once

//Parent
#include <Core/CoreObject/CoreObject.h>

//Manages a render target
#include <Rendering/RenderTarget/RenderTarget.h>

//
//TODO: HDR format
//
const ResourceFormat LABUFFER_RENDER_TARGET_FORMAT = RESOURCE_FORMAT_R8G8B8A8_UNORM;

namespace EngineAPI
{
	namespace Graphics
	{
		namespace DeferredRendering
		{
			class LABuffer : public EngineAPI::Core::CoreObject
			{
			public:
				LABuffer() {};
				~LABuffer() {};

				//Override the shutdown function
				void Shutdown() override;

				//Override the SetDebugName function to name
				//the render targets name correctly
				void SetDebugName(std::string s) override;

				//Inits the LABuffer
				bool InitLABuffer(uint32_t width, uint32_t height,
					std::string debugName = "");

				//OnResize
				bool OnResize(uint32_t newWidth, uint32_t newHeight);

				//Clears the LABuffer
				void Clear();

				//Binds the LABuffer for lighting pass - takes a depth 
				//texture view as we can enable depth test to cull
				//lights - this should be a read-only depth view
				void BindLABufferForLightingPass(EngineAPI::Graphics::DepthStencilView* depthGBufferReadOnlyDSV);

			protected:
				//Render targets
				EngineAPI::Rendering::RenderTarget laBufferRenderTarget;
			};
		};
	};
};