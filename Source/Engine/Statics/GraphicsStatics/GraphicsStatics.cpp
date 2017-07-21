#include "GraphicsStatics.h"

//Needs to know the size of light data to create a CBuffer for
#include <Rendering/DirectionalLight/DirectionalLight.h>

using namespace EngineAPI::Statics;

EngineAPI::Graphics::VertexShader GraphicsStatics::LightPass_DirectionalLight_VS;
EngineAPI::Graphics::PixelShader GraphicsStatics::LightPass_DirectionalLight_PS;
EngineAPI::Graphics::ConstantBuffer GraphicsStatics::LightPass_DirectionalLight_LightDataCB;

EngineAPI::Graphics::VertexShader GraphicsStatics::Blit_VS;
EngineAPI::Graphics::PixelShader  GraphicsStatics::Blit_PS;

EngineAPI::Graphics::VertexShader GraphicsStatics::Debug_GBufferVis_Depth_VS;
EngineAPI::Graphics::PixelShader  GraphicsStatics::Debug_GBufferVis_Depth_PS;
EngineAPI::Graphics::VertexShader GraphicsStatics::Debug_GBufferVis_Colour_VS;
EngineAPI::Graphics::PixelShader  GraphicsStatics::Debug_GBufferVis_Colour_PS;
EngineAPI::Graphics::VertexShader GraphicsStatics::Debug_GBufferVis_SpecIntensity_VS;
EngineAPI::Graphics::PixelShader  GraphicsStatics::Debug_GBufferVis_SpecIntensity_PS;
EngineAPI::Graphics::VertexShader GraphicsStatics::Debug_GBufferVis_Normal_VS;
EngineAPI::Graphics::PixelShader  GraphicsStatics::Debug_GBufferVis_Normal_PS;
EngineAPI::Graphics::VertexShader GraphicsStatics::Debug_GBufferVis_SpecPower_VS;
EngineAPI::Graphics::PixelShader  GraphicsStatics::Debug_GBufferVis_SpecPower_PS;
EngineAPI::Graphics::SamplerState GraphicsStatics::Debug_GBufferVis_SamplerState;

bool GraphicsStatics::InitAllGraphicsStatics(EngineAPI::Graphics::GraphicsDevice* device)
{
	assert(device);

	//Light pass
	GraphicsStatics::InitLightPass(device);

	//Debug
	GraphicsStatics::InitGBufferVis(device);

	//Blit shader
	assert(Blit_VS.InitCompiledVertexShaderFromFile(device,
		ENGINE_SHADER_COMPILED_ASSETS_FOLDER"BlitVS.cso",
		nullptr, 0, "Blit_VS"));

	assert(Blit_PS.InitCompiledPixelShaderFromFile(device,
		ENGINE_SHADER_COMPILED_ASSETS_FOLDER"BlitPS.cso",
		"Blit_PS"));

	//Done
	return true;
}

void GraphicsStatics::ShutdownAllGraphicsStatics()
{
	LightPass_DirectionalLight_VS.Shutdown();
	LightPass_DirectionalLight_PS.Shutdown();
	LightPass_DirectionalLight_LightDataCB.Shutdown();

	Blit_VS.Shutdown();
	Blit_PS.Shutdown();

	Debug_GBufferVis_Depth_VS.Shutdown();
	Debug_GBufferVis_Depth_PS.Shutdown();
	Debug_GBufferVis_Colour_VS.Shutdown();
	Debug_GBufferVis_Colour_PS.Shutdown();
	Debug_GBufferVis_SpecIntensity_VS.Shutdown();
	Debug_GBufferVis_SpecIntensity_PS.Shutdown();
	Debug_GBufferVis_Normal_VS.Shutdown();
	Debug_GBufferVis_Normal_PS.Shutdown();
	Debug_GBufferVis_SpecPower_VS.Shutdown();
	Debug_GBufferVis_SpecPower_PS.Shutdown();
	Debug_GBufferVis_SamplerState.Shutdown();
}

void GraphicsStatics::InitLightPass(EngineAPI::Graphics::GraphicsDevice* device)
{
	//
	//Lighting pass:
	//
	//	Directional Light:
	assert(LightPass_DirectionalLight_VS.InitCompiledVertexShaderFromFile(device,
		ENGINE_SHADER_COMPILED_ASSETS_FOLDER"L_DirectionalLightVS.cso",
		nullptr, 0, "LightPass_DirectionalLight_VS"));

	assert(LightPass_DirectionalLight_PS.InitCompiledPixelShaderFromFile(device,
		ENGINE_SHADER_COMPILED_ASSETS_FOLDER"L_DirectionalLightPS.cso",
		"LightPass_DirectionalLight_PS"));

	assert(LightPass_DirectionalLight_LightDataCB.InitConstantBuffer(device,
		sizeof(DirectionalLightGraphicsData), nullptr,
		RESOURCE_USAGE_DYNAMIC, RESOURCE_CPU_ACCESS_WRITE_BIT,
		RESOURCE_BIND_CONSTANT_BUFFER_BIT,
		"LightPass_DirectionalLight_LightDataCB"));
}

void GraphicsStatics::InitGBufferVis(EngineAPI::Graphics::GraphicsDevice* device)
{
	//Shaders
	assert(Debug_GBufferVis_Colour_VS.InitCompiledVertexShaderFromFile(device, ENGINE_SHADER_COMPILED_ASSETS_FOLDER"GBufferVis_ColourVS.cso", nullptr, 0, "Debug_GBufferVis_ColourVS"));
	assert(Debug_GBufferVis_Colour_PS.InitCompiledPixelShaderFromFile(device, ENGINE_SHADER_COMPILED_ASSETS_FOLDER"GBufferVis_ColourPS.cso", "Debug_GBufferVis_ColourPS"));

	//Sampler state
	SamplerStateDescription ssDesc = {};
	ssDesc.AddressModeU = TEXTURE_ADDRESS_CLAMP;
	ssDesc.AddressModeV = TEXTURE_ADDRESS_CLAMP;
	ssDesc.AddressModeW = TEXTURE_ADDRESS_CLAMP;
	ssDesc.FilterMode = TEXTURE_FILTER_MIN_MAG_MIP_POINT;
	assert(Debug_GBufferVis_SamplerState.InitSamplerState(device, &ssDesc, "Debug_GBufferVis_SamplerState"));
}