//GraphicsStatics.h
//Created 20/07/17
//Created By Daniel Bowler
//
//Graphics statics - eg: states, shaders etc
//will go here. These are things created at init time
//and remain throughout the entire life of the program

#pragma once

//Graphics
#include <Includes/GraphicsIncludes.h>

//States (Pipeline)
#include <Graphics/BlendState/BlendState.h>
#include <Graphics/DepthStencilState/DepthStencilState.h>
#include <Graphics/RasterizerState/RasterizerState.h>

//Shaders
#include <Graphics/VertexShader/VertexShader.h>
#include <Graphics/HullShader/HullShader.h>
#include <Graphics/DomainShader/DomainShader.h>
#include <Graphics/PixelShader/PixelShader.h>

//CBuffer
#include <Graphics/ConstantBuffer/ConstantBuffer.h>

//Sampler state
#include <Graphics/SamplerState/SamplerState.h>

namespace EngineAPI
{
	namespace Statics
	{
		class GraphicsStatics
		{
			MAKE_STATICS_CLASS_ONLY(GraphicsStatics);
		public:
			//Inits everything - called once the engine has
			//been loaded. 
			static bool InitAllGraphicsStatics(EngineAPI::Graphics::GraphicsDevice* device);
			static void ShutdownAllGraphicsStatics();

		public:
			//Default pipeline states
			static EngineAPI::Graphics::BlendState DefaultPipelineState_Blend;
			static EngineAPI::Graphics::DepthStencilState DefaultPipelineState_DepthStencil;
			static EngineAPI::Graphics::RasterizerState DefaultPipelineState_Rasterizer;

			//Custom pipeline states
			//
			//Blend modes
			//
			//	Additive Blending - used during the lighting pass
			static EngineAPI::Graphics::BlendState PipelineState_Blend_Additive;

			//Depth Stencil modes:
			//
			//	Mark stencil buffer - during geometry pass, we will mark the stencil buffer for every
			//visible fragment. We can then limit lighting to just those pixels that actually contain 
			//some rendered geometry.
			static EngineAPI::Graphics::DepthStencilState PipelineState_DepthStencil_DepthDefault_MarkStencilBuffer;
			
			//	See: PipelineState_DepthStencil_DepthDefault_MarkStencilBuffer. This DSS enables stencil testing
			//so that pixels that have no rendered geometry are discarded. Used during lighting to prevent
			//empty pixels from being lit (Waste of resources - especially in the case of DLights which
			//render a full screen quad)
			static EngineAPI::Graphics::DepthStencilState PipelineState_DepthStencil_DepthDefault_StencilTestNotEqual;

			//Rasterizer modes

		public:
			//Engine statics
			//
			//Lighting pass:
			//
			//	Directional light:
			static EngineAPI::Graphics::VertexShader      LightPass_DirectionalLight_VS;
			static EngineAPI::Graphics::PixelShader       LightPass_DirectionalLight_PS;
			static EngineAPI::Graphics::ConstantBuffer	  LightPass_DirectionalLight_LightDataCB;
			static EngineAPI::Graphics::DepthStencilState LightPass_DirectionalLight_DSS;

			//	Point light:
			static EngineAPI::Graphics::VertexShader	  LightPass_PointLight_VS;
			static EngineAPI::Graphics::HullShader		  LightPass_PointLight_HS;
			static EngineAPI::Graphics::DomainShader	  LightPass_PointLight_DS;
			static EngineAPI::Graphics::PixelShader		  LightPass_PointLight_PS;
			static EngineAPI::Graphics::ConstantBuffer	  LightPass_PointLight_LightDataCB;
			static EngineAPI::Graphics::DepthStencilState LightPass_PointLight_DSS;
			static EngineAPI::Graphics::RasterizerState   LightPass_PointLight_RZS;

			//	Spot light:

			//	Capsule light:

		public:
			//Simple blit shader
			static EngineAPI::Graphics::VertexShader Blit_VS;
			static EngineAPI::Graphics::PixelShader  Blit_PS;

		public:
			//GBuffer visualization shaders
			//
			//
			static EngineAPI::Graphics::VertexShader Debug_GBufferVis_VS;

			static EngineAPI::Graphics::PixelShader  Debug_GBufferVis_Depth_PS;
			static EngineAPI::Graphics::PixelShader  Debug_GBufferVis_Colour_PS;
			static EngineAPI::Graphics::PixelShader  Debug_GBufferVis_SpecIntensity_PS;
			static EngineAPI::Graphics::PixelShader  Debug_GBufferVis_UnpackedNormal_PS;
			static EngineAPI::Graphics::PixelShader  Debug_GBufferVis_PackedNormal_PS;
			static EngineAPI::Graphics::PixelShader  Debug_GBufferVis_SpecPower_PS;

			//Sampler state
			static EngineAPI::Graphics::SamplerState Debug_GBufferVis_SamplerState;

		protected:
			//Split up init for less clutter
			static void InitPipelineStates(EngineAPI::Graphics::GraphicsDevice* device);
			static void InitLightPass(EngineAPI::Graphics::GraphicsDevice* device);
			static void InitGBufferVis(EngineAPI::Graphics::GraphicsDevice* device);
		};
	};
};
