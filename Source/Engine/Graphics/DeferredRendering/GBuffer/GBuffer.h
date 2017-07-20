//GBuffer.h
//Created 19/07/17
//Created By Daniel Bowler
//
//Geometry buffer for deferred rendering

#pragma once

//Parent
#include <Core/CoreObject/CoreObject.h>

//Manages a render target set
#include <Rendering/RenderTargetSet/RenderTargetSet.h>

const unsigned GBUFFER_RENDER_TARGETS_COUNT = 3;
const ResourceFormat GBUFFER_RENDER_TARGETS_FORMATS[GBUFFER_RENDER_TARGETS_COUNT] =
{
	RESOURCE_FORMAT_R8G8B8A8_UNORM,		//Diff colour + Spec Intensity
	RESOURCE_FORMAT_R11G11B10_FLOAT,	//Normal
	RESOURCE_FORMAT_R8G8B8A8_UNORM,     //Spec power + empty[3]
};

namespace EngineAPI
{
	namespace Graphics
	{
		namespace DeferredRendering
		{
			class GBuffer : public EngineAPI::Core::CoreObject
			{
			public:
				GBuffer() {};
				~GBuffer() {};

				//Override the shutdown function
				void Shutdown() override;

				//Override the SetDebugName function to name
				//the render target set correctly
				void SetDebugName(std::string s) override;

				//Inits the GBuffer
				bool InitGBuffer(uint32_t width, uint32_t height,
					std::string debugName = "");

				//OnResize
				bool OnResize(uint32_t newWidth, uint32_t newHeight);

				//Clears the GBuffer
				void Clear();

				//Binds the GBuffer for geometry pass
				void BindGBufferForGeometryPass(EngineAPI::Graphics::DepthStencilView* depthGBufferDSV);

				//unbinds the GBuffer from the output merger
				void UnbindGBufferAfterGeometryPass();

				//Binds the GBuffer for the lighting pass - shader resources
				void BindGBufferForLightPass(EngineAPI::Graphics::ShaderResourceView* depthGBufferSRV);

				//Unbinds the GBuffer (SRVs) after lighting 
				void UnbindGBufferAfterLightPass();

			protected:
				//Render targets
				EngineAPI::Rendering::RenderTargetSet gBufferRenderTargetSet;
			};
		};
	};
};