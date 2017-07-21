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

		public:
			//Engine statics
			//
			//Lighting pass:
			//
			//	DirectionalLight:
			static EngineAPI::Graphics::VertexShader   LightPass_DirectionalLight_VS;
			static EngineAPI::Graphics::PixelShader    LightPass_DirectionalLight_PS;
			static EngineAPI::Graphics::ConstantBuffer LightPass_DirectionalLight_LightDataCB;

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
			static void InitDefaultStates(EngineAPI::Graphics::GraphicsDevice* device);
			static void InitLightPass(EngineAPI::Graphics::GraphicsDevice* device);
			static void InitGBufferVis(EngineAPI::Graphics::GraphicsDevice* device);
		};
	};
};
