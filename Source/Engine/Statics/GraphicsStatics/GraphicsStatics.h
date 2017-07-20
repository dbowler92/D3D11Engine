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
			//Engine statics
			//
			//Lighting pass:
			//
			static EngineAPI::Graphics::SamplerState   LightPass_PointSamplerState;

			//	DirectionalLight:
			static EngineAPI::Graphics::VertexShader   LightPass_DirectionalLight_VS;
			static EngineAPI::Graphics::PixelShader    LightPass_DirectionalLight_PS;
			static EngineAPI::Graphics::ConstantBuffer LightPass_DirectionalLight_LightDataCB;

		};
	};
};
