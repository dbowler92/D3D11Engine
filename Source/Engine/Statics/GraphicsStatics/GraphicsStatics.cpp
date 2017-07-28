#include "GraphicsStatics.h"

//Needs to know the size of light data to create a CBuffer for
#include <Rendering/DirectionalLight/DirectionalLight.h>
#include <Rendering/PointLight/PointLight.h>
#include <Rendering/SpotLight/SpotLight.h>

using namespace EngineAPI::Statics;

EngineAPI::Graphics::BlendState GraphicsStatics::PipelineState_Blend_Debug;
EngineAPI::Graphics::DepthStencilState GraphicsStatics::PipelineState_DepthStencil_Debug;
EngineAPI::Graphics::RasterizerState GraphicsStatics::PipelineState_Rasterizer_Debug;

EngineAPI::Graphics::BlendState GraphicsStatics::DefaultPipelineState_Blend;
EngineAPI::Graphics::DepthStencilState GraphicsStatics::DefaultPipelineState_DepthStencil;
EngineAPI::Graphics::RasterizerState GraphicsStatics::DefaultPipelineState_Rasterizer;

EngineAPI::Graphics::BlendState GraphicsStatics::PipelineState_Blend_Additive;

EngineAPI::Graphics::DepthStencilState GraphicsStatics::PipelineState_DepthStencil_DepthDefault_MarkStencilBuffer;
EngineAPI::Graphics::DepthStencilState GraphicsStatics::PipelineState_DepthStencil_DepthDefault_StencilTestNotEqual;

EngineAPI::Graphics::VertexShader GraphicsStatics::LightPass_DirectionalLight_VS;
EngineAPI::Graphics::PixelShader GraphicsStatics::LightPass_DirectionalLight_PS;
EngineAPI::Graphics::ConstantBuffer GraphicsStatics::LightPass_DirectionalLight_LightDataCB;
EngineAPI::Graphics::DepthStencilState GraphicsStatics::LightPass_DirectionalLight_DSS;

EngineAPI::Graphics::VertexShader   GraphicsStatics::LightPass_PointLight_VS;
EngineAPI::Graphics::HullShader     GraphicsStatics::LightPass_PointLight_HS;
EngineAPI::Graphics::DomainShader   GraphicsStatics::LightPass_PointLight_DS;
EngineAPI::Graphics::PixelShader    GraphicsStatics::LightPass_PointLight_PS;
EngineAPI::Graphics::ConstantBuffer GraphicsStatics::LightPass_PointLight_LightDataCB;
EngineAPI::Graphics::DepthStencilState GraphicsStatics::LightPass_PointLight_DSS;
EngineAPI::Graphics::RasterizerState   GraphicsStatics::LightPass_PointLight_RZS;

EngineAPI::Graphics::VertexShader	   GraphicsStatics::LightPass_SpotLight_VS;
EngineAPI::Graphics::HullShader		   GraphicsStatics::LightPass_SpotLight_HS;
EngineAPI::Graphics::DomainShader	   GraphicsStatics::LightPass_SpotLight_DS;
EngineAPI::Graphics::PixelShader	   GraphicsStatics::LightPass_SpotLight_PS;
EngineAPI::Graphics::ConstantBuffer	   GraphicsStatics::LightPass_SpotLight_LightDataCB;
EngineAPI::Graphics::DepthStencilState GraphicsStatics::LightPass_SpotLight_DSS;
EngineAPI::Graphics::RasterizerState   GraphicsStatics::LightPass_SpotLight_RZS;

EngineAPI::Graphics::VertexShader GraphicsStatics::Blit_VS;
EngineAPI::Graphics::PixelShader  GraphicsStatics::Blit_PS;

EngineAPI::Graphics::VertexShader GraphicsStatics::Debug_GBufferVis_VS;
EngineAPI::Graphics::PixelShader  GraphicsStatics::Debug_GBufferVis_Depth_PS;
EngineAPI::Graphics::PixelShader  GraphicsStatics::Debug_GBufferVis_Colour_PS;
EngineAPI::Graphics::PixelShader  GraphicsStatics::Debug_GBufferVis_SpecIntensity_PS;
EngineAPI::Graphics::PixelShader  GraphicsStatics::Debug_GBufferVis_UnpackedNormal_PS;
EngineAPI::Graphics::PixelShader  GraphicsStatics::Debug_GBufferVis_PackedNormal_PS;
EngineAPI::Graphics::PixelShader  GraphicsStatics::Debug_GBufferVis_SpecPower_PS;
EngineAPI::Graphics::SamplerState GraphicsStatics::Debug_GBufferVis_SamplerState;

bool GraphicsStatics::InitAllGraphicsStatics(EngineAPI::Graphics::GraphicsDevice* device)
{
	assert(device);

	//Debug
	GraphicsStatics::InitDebugPipelineStates(device);

	//Default pipeline states
	GraphicsStatics::InitPipelineStates(device);

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
	PipelineState_Blend_Debug.Shutdown();
	PipelineState_DepthStencil_Debug.Shutdown();
	PipelineState_Rasterizer_Debug.Shutdown();

	DefaultPipelineState_Blend.Shutdown();
	DefaultPipelineState_DepthStencil.Shutdown();
	DefaultPipelineState_Rasterizer.Shutdown();

	PipelineState_Blend_Additive.Shutdown();
	PipelineState_DepthStencil_DepthDefault_MarkStencilBuffer.Shutdown();
	PipelineState_DepthStencil_DepthDefault_StencilTestNotEqual.Shutdown();

	LightPass_DirectionalLight_VS.Shutdown();
	LightPass_DirectionalLight_PS.Shutdown();
	LightPass_DirectionalLight_LightDataCB.Shutdown();
	LightPass_DirectionalLight_DSS.Shutdown();

	LightPass_PointLight_VS.Shutdown();
	LightPass_PointLight_HS.Shutdown();
	LightPass_PointLight_DS.Shutdown();
	LightPass_PointLight_PS.Shutdown();
	LightPass_PointLight_LightDataCB.Shutdown();
	LightPass_PointLight_DSS.Shutdown();
	LightPass_PointLight_RZS.Shutdown();

	LightPass_SpotLight_VS.Shutdown();
	LightPass_SpotLight_HS.Shutdown();
	LightPass_SpotLight_DS.Shutdown();
	LightPass_SpotLight_PS.Shutdown();
	LightPass_SpotLight_LightDataCB.Shutdown();
	LightPass_SpotLight_DSS.Shutdown();
	LightPass_SpotLight_RZS.Shutdown();

	Blit_VS.Shutdown();
	Blit_PS.Shutdown();

	Debug_GBufferVis_VS.Shutdown();
	Debug_GBufferVis_Depth_PS.Shutdown();
	Debug_GBufferVis_Colour_PS.Shutdown();
	Debug_GBufferVis_SpecIntensity_PS.Shutdown();
	Debug_GBufferVis_UnpackedNormal_PS.Shutdown();
	Debug_GBufferVis_PackedNormal_PS.Shutdown();
	Debug_GBufferVis_SpecPower_PS.Shutdown();
	Debug_GBufferVis_SamplerState.Shutdown();
}

void GraphicsStatics::InitDebugPipelineStates(EngineAPI::Graphics::GraphicsDevice* device)
{
	BlendPipelineStateDescription bsDesc = {};
	DepthStencilPipelineStateDescription dssDesc = {};
	RasterizerPipelineStateDescription rsDesc = {};

	assert(PipelineState_Blend_Debug.InitBlendState(device, &bsDesc, "PipelineState_Blend_Debug"));
	
	dssDesc.DepthTestEnabled = false;
	dssDesc.StencilTestEnabled = false;
	assert(PipelineState_DepthStencil_Debug.InitDepthStencilState(device, &dssDesc, "PipelineState_DepthStencil_Debug"));
	
	rsDesc.FillMode = POLYGON_FILL_WIREFRAME;
	rsDesc.WindingOrder = POLYGON_WINDING_ORDER_FRONT_CLOCKWISE;
	rsDesc.FaceCullingMode = POLYGON_FACE_CULL_NONE;
	assert(PipelineState_Rasterizer_Debug.InitRasterizerState(device, &rsDesc, "PipelineState_Rasterizer_Debug"));
}

void GraphicsStatics::InitPipelineStates(EngineAPI::Graphics::GraphicsDevice* device)
{
	//Use default settings...
	BlendPipelineStateDescription bsDesc = {};
	DepthStencilPipelineStateDescription dssDesc = {};
	RasterizerPipelineStateDescription rsDesc = {};

	assert(DefaultPipelineState_Blend.InitBlendState(device, &bsDesc, "DefaultPipelineState_Blend"));
	assert(DefaultPipelineState_DepthStencil.InitDepthStencilState(device, &dssDesc, "DefaultPipelineState_DepthStencil"));
	assert(DefaultPipelineState_Rasterizer.InitRasterizerState(device, &rsDesc, "DefaultPipelineState_Rasterizer"));

	//Additive blend
	bsDesc = BlendPipelineStateDescription();
	bsDesc.IndependentBlendEnable = false;
	bsDesc.RenderTargetsBlendState[0].BlendEnabled = true;
	bsDesc.RenderTargetsBlendState[0].BlendOp = BLEND_OP_ADD;
	bsDesc.RenderTargetsBlendState[0].BlendAlphaOp = BLEND_OP_ADD;
	bsDesc.RenderTargetsBlendState[0].DestinationBlend = BLEND_MODE_ONE;
	bsDesc.RenderTargetsBlendState[0].DestinationAlphaBlend = BLEND_MODE_ONE;
	bsDesc.RenderTargetsBlendState[0].SourceBlend = BLEND_MODE_ONE;
	bsDesc.RenderTargetsBlendState[0].SourceAlphaBlend = BLEND_MODE_ONE;
	bsDesc.RenderTargetsBlendState[0].RenderTargetWriteMask =
		RENDER_TARGET_WRITE_ENABLE_RED_BIT | RENDER_TARGET_WRITE_ENABLE_GREEN_BIT |
		RENDER_TARGET_WRITE_ENABLE_BLUE_BIT;
	assert(PipelineState_Blend_Additive.InitBlendState(device, &bsDesc, "PipelineState_Blend_Additive"));

	//Mark the stencil buffer with a value -> Used during geometry pass as a default 
	//setting - TODO: Engine can set the stencil to a value in the range 0-128. User can set the
	//stencil to 128-255
	dssDesc = DepthStencilPipelineStateDescription();
	dssDesc.StencilTestEnabled = true;
	dssDesc.StencilReadMask = 0;
	dssDesc.StencilWriteMask = 0xFF;
	dssDesc.FrontFaceOp.OnStencilFail = STENCIL_OP_KEEP;
	dssDesc.FrontFaceOp.OnStencilPassDepthFail = STENCIL_OP_KEEP;
	dssDesc.FrontFaceOp.OnStencilPassDepthPass = STENCIL_OP_REPLACE;			//Set value
	dssDesc.FrontFaceOp.StencilComparisonFunction = COMPARISON_FUNCTION_ALWAYS; //No stencil testing...
	assert(PipelineState_DepthStencil_DepthDefault_MarkStencilBuffer.InitDepthStencilState(device, &dssDesc, "PipelineState_DepthStencil_DefaultDepth_MarkStencil"));

	//Stencil test enabled - passes if the stencil value in the stencil buffer
	//is not equal to the reference - Eg: Lighting pass - only pass for
	//pixels that have some geometry data from the geometry pass
	dssDesc = DepthStencilPipelineStateDescription();
	dssDesc.StencilTestEnabled = true;
	dssDesc.StencilReadMask = 0xFF;
	dssDesc.StencilWriteMask = 0;
	dssDesc.FrontFaceOp.OnStencilFail = STENCIL_OP_KEEP;
	dssDesc.FrontFaceOp.OnStencilPassDepthFail = STENCIL_OP_KEEP;
	dssDesc.FrontFaceOp.OnStencilPassDepthPass = STENCIL_OP_KEEP;
	dssDesc.FrontFaceOp.StencilComparisonFunction = COMPARISON_FUNCTION_LESS; 
	dssDesc.BackFaceOp.OnStencilFail = STENCIL_OP_KEEP;
	dssDesc.BackFaceOp.OnStencilPassDepthFail = STENCIL_OP_KEEP;
	dssDesc.BackFaceOp.OnStencilPassDepthPass = STENCIL_OP_KEEP;
	dssDesc.BackFaceOp.StencilComparisonFunction = COMPARISON_FUNCTION_LESS;
	assert(PipelineState_DepthStencil_DepthDefault_StencilTestNotEqual.InitDepthStencilState(device, &dssDesc, "PipelineState_DepthStencil_DefaultDepth_StencilTestNotEqual"));
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
		"LightPass_DirectionalLight_SharedLightDataCB"));

	DepthStencilPipelineStateDescription dssDesc = {};
	dssDesc.DepthTestEnabled = false;
	dssDesc.DepthWriteMask = DEPTH_TEXTURE_WRITE_MASK_ZERO;
	dssDesc.StencilTestEnabled = true;
	dssDesc.StencilReadMask = 0xFF;
	dssDesc.StencilWriteMask = 0;
	dssDesc.FrontFaceOp.OnStencilFail = STENCIL_OP_KEEP;
	dssDesc.FrontFaceOp.OnStencilPassDepthFail = STENCIL_OP_KEEP;
	dssDesc.FrontFaceOp.OnStencilPassDepthPass = STENCIL_OP_KEEP;
	dssDesc.FrontFaceOp.StencilComparisonFunction = COMPARISON_FUNCTION_NOT_EQUAL;
	dssDesc.BackFaceOp.OnStencilFail = STENCIL_OP_KEEP;
	dssDesc.BackFaceOp.OnStencilPassDepthFail = STENCIL_OP_KEEP;
	dssDesc.BackFaceOp.OnStencilPassDepthPass = STENCIL_OP_KEEP;
	dssDesc.BackFaceOp.StencilComparisonFunction = COMPARISON_FUNCTION_NOT_EQUAL;
	assert(LightPass_DirectionalLight_DSS.InitDepthStencilState(device, &dssDesc, "LightPass_DirectionalLight_DepthStencilState"));

	//	Point light:
	assert(LightPass_PointLight_VS.InitCompiledVertexShaderFromFile(device,
		ENGINE_SHADER_COMPILED_ASSETS_FOLDER"L_PointLightVS.cso",
		nullptr, 0, "LightPass_PointLight_VS"));

	assert(LightPass_PointLight_HS.InitCompiledHullShaderFromFile(device,
		ENGINE_SHADER_COMPILED_ASSETS_FOLDER"L_PointLightHS.cso",
		"LightPass_PointLight_HS"));

	assert(LightPass_PointLight_DS.InitCompiledDomainShaderFromFile(device,
		ENGINE_SHADER_COMPILED_ASSETS_FOLDER"L_PointLightDS.cso",
		"LightPass_PointLight_DS"));

	assert(LightPass_PointLight_PS.InitCompiledPixelShaderFromFile(device,
		ENGINE_SHADER_COMPILED_ASSETS_FOLDER"L_PointLightPS.cso",
		"LightPass_PointLight_PS"));

	assert(LightPass_PointLight_LightDataCB.InitConstantBuffer(device,
		sizeof(PointLightGraphicsData), nullptr,
		RESOURCE_USAGE_DYNAMIC, RESOURCE_CPU_ACCESS_WRITE_BIT,
		RESOURCE_BIND_CONSTANT_BUFFER_BIT,
		"LightPass_PointLight_SharedLightDataCB"));

	dssDesc = DepthStencilPipelineStateDescription();
	dssDesc.DepthTestEnabled = true;
	dssDesc.DepthWriteMask = DEPTH_TEXTURE_WRITE_MASK_ZERO;
	dssDesc.DepthTestFunction = COMPARISON_FUNCTION_GREATER_EQUAL;
	dssDesc.StencilTestEnabled = true;
	dssDesc.StencilReadMask = 0xFF;
	dssDesc.StencilWriteMask = 0;
	dssDesc.FrontFaceOp.OnStencilFail = STENCIL_OP_KEEP;
	dssDesc.FrontFaceOp.OnStencilPassDepthFail = STENCIL_OP_KEEP;
	dssDesc.FrontFaceOp.OnStencilPassDepthPass = STENCIL_OP_KEEP;
	dssDesc.FrontFaceOp.StencilComparisonFunction = COMPARISON_FUNCTION_NOT_EQUAL;
	dssDesc.BackFaceOp.OnStencilFail = STENCIL_OP_KEEP;
	dssDesc.BackFaceOp.OnStencilPassDepthFail = STENCIL_OP_KEEP;
	dssDesc.BackFaceOp.OnStencilPassDepthPass = STENCIL_OP_KEEP;
	dssDesc.BackFaceOp.StencilComparisonFunction = COMPARISON_FUNCTION_NOT_EQUAL;
	assert(LightPass_PointLight_DSS.InitDepthStencilState(device, &dssDesc, "LightPass_PointLight_DepthStencilState"));

	RasterizerPipelineStateDescription rsDesc = {};
	rsDesc.FillMode = POLYGON_FILL_SOLID;
	rsDesc.FaceCullingMode = POLYGON_FACE_CULL_FRONT;
	assert(LightPass_PointLight_RZS.InitRasterizerState(device, &rsDesc, "LightPass_PointLight_RasterizerState"));

	//	Spot Light:
	assert(LightPass_SpotLight_VS.InitCompiledVertexShaderFromFile(device,
		ENGINE_SHADER_COMPILED_ASSETS_FOLDER"L_SpotLightVS.cso",
		nullptr, 0, "LightPass_SpotLight_VS"));

	assert(LightPass_SpotLight_HS.InitCompiledHullShaderFromFile(device,
		ENGINE_SHADER_COMPILED_ASSETS_FOLDER"L_SpotLightHS.cso",
		"LightPass_SpotLight_HS"));

	assert(LightPass_SpotLight_DS.InitCompiledDomainShaderFromFile(device,
		ENGINE_SHADER_COMPILED_ASSETS_FOLDER"L_SpotLightDS.cso",
		"LightPass_SpotLight_DS"));

	assert(LightPass_SpotLight_PS.InitCompiledPixelShaderFromFile(device,
		ENGINE_SHADER_COMPILED_ASSETS_FOLDER"L_SpotLightPS.cso",
		"LightPass_SpotLight_PS"));

	assert(LightPass_SpotLight_LightDataCB.InitConstantBuffer(device,
		sizeof(SpotLightGraphicsData), nullptr,
		RESOURCE_USAGE_DYNAMIC, RESOURCE_CPU_ACCESS_WRITE_BIT,
		RESOURCE_BIND_CONSTANT_BUFFER_BIT,
		"LightPass_SpotLight_SharedLightDataCB"));
}

void GraphicsStatics::InitGBufferVis(EngineAPI::Graphics::GraphicsDevice* device)
{
	//Shaders
	//
	//Shared VS
	assert(Debug_GBufferVis_VS.InitCompiledVertexShaderFromFile(device, ENGINE_SHADER_COMPILED_ASSETS_FOLDER"GBufferVis_VS.cso", nullptr, 0, "Debug_GBufferVis_VS"));
	
	//PS
	assert(Debug_GBufferVis_Depth_PS.InitCompiledPixelShaderFromFile(device, ENGINE_SHADER_COMPILED_ASSETS_FOLDER"GBufferVis_DepthPS.cso", "Debug_GBufferVis_DepthPS"));
	assert(Debug_GBufferVis_Colour_PS.InitCompiledPixelShaderFromFile(device, ENGINE_SHADER_COMPILED_ASSETS_FOLDER"GBufferVis_ColourPS.cso", "Debug_GBufferVis_ColourPS"));
	assert(Debug_GBufferVis_SpecIntensity_PS.InitCompiledPixelShaderFromFile(device, ENGINE_SHADER_COMPILED_ASSETS_FOLDER"GBufferVis_SpecIntPS.cso", "Debug_GBufferVis_SpecIntPS"));
	assert(Debug_GBufferVis_PackedNormal_PS.InitCompiledPixelShaderFromFile(device, ENGINE_SHADER_COMPILED_ASSETS_FOLDER"GBufferVis_PackedNormalPS.cso", "Debug_GBufferVis_PackedNormalPS"));
	assert(Debug_GBufferVis_UnpackedNormal_PS.InitCompiledPixelShaderFromFile(device, ENGINE_SHADER_COMPILED_ASSETS_FOLDER"GBufferVis_UnpackedNormalPS.cso", "Debug_GBufferVis_UnpackedNormalPS"));
	assert(Debug_GBufferVis_SpecPower_PS.InitCompiledPixelShaderFromFile(device, ENGINE_SHADER_COMPILED_ASSETS_FOLDER"GBufferVis_SpecPowerPS.cso", "Debug_GBufferVis_SpecPowerPS"));

	//Sampler state
	SamplerStateDescription ssDesc = {};
	ssDesc.AddressModeU = TEXTURE_ADDRESS_CLAMP;
	ssDesc.AddressModeV = TEXTURE_ADDRESS_CLAMP;
	ssDesc.AddressModeW = TEXTURE_ADDRESS_CLAMP;
	ssDesc.FilterMode = TEXTURE_FILTER_MIN_MAG_MIP_POINT;
	assert(Debug_GBufferVis_SamplerState.InitSamplerState(device, &ssDesc, "Debug_GBufferVis_SamplerState"));
}