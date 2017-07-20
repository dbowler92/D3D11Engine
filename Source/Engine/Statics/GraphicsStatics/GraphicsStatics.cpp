#include "GraphicsStatics.h"

//Needs to know the size of light data to create a CBuffer for
#include <Rendering/DirectionalLight/DirectionalLight.h>

using namespace EngineAPI::Statics;

EngineAPI::Graphics::VertexShader GraphicsStatics::LightPass_DirectionalLight_VS;
EngineAPI::Graphics::PixelShader GraphicsStatics::LightPass_DirectionalLight_PS;
EngineAPI::Graphics::ConstantBuffer GraphicsStatics::LightPass_DirectionalLight_LightDataCB;

bool GraphicsStatics::InitAllGraphicsStatics(EngineAPI::Graphics::GraphicsDevice* device)
{
	assert(device);

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

	//Done
	return true;
}

void GraphicsStatics::ShutdownAllGraphicsStatics()
{
	LightPass_DirectionalLight_VS.Shutdown();
	LightPass_DirectionalLight_PS.Shutdown();
	LightPass_DirectionalLight_LightDataCB.Shutdown();
}