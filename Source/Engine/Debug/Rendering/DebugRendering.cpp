#include "DebugRendering.h"

//Uses some of the engine static data - eg: Shaders
#include <Statics/GraphicsStatics/GraphicsStatics.h>

//Needs access to the lights data in order to debug render
#include <Rendering/PointLight/PointLight.h>
#include <Rendering/SpotLight/SpotLight.h>

using namespace EngineAPI::Debug;

void DebugRendering::DebugRenderGBuffersToScreen(bool showPackedNormals)
{
	//Device and manager
	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();

	//Even size for each GBuffer vis
	uint32_t w = gm->GetWindowWidth() / 5;  //We have 5 buffers to vis
	uint32_t h = gm->GetWindowHeight() / 4;

	//Updated for each draw call
	Viewport vp = {};
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = (FLOAT)gm->GetWindowHeight() - h;
	vp.Width = (FLOAT)w;
	vp.Height = (FLOAT)h;

	//Pointer to the GBuffer SRV for rendering
	EngineAPI::Graphics::ShaderResourceView* gbufferSRV = nullptr;

	//Each draw generates its own quad in the VS
	device->VSSetShader(&EngineAPI::Statics::GraphicsStatics::Debug_GBufferVis_VS); //Shared VS
	device->IASetTopology(PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	device->IASetVertexBuffer(nullptr, 0, 0);
	device->IASetIndexBuffer(nullptr, 0);

	//Uses point sampler
	device->PSSetSamplerState(&EngineAPI::Statics::GraphicsStatics::Debug_GBufferVis_SamplerState, 0);

	//Null states
	device->OMSetBlendState(&EngineAPI::Statics::GraphicsStatics::DefaultPipelineState_Blend);
	device->OMSetDepthStencilState(&EngineAPI::Statics::GraphicsStatics::DefaultPipelineState_DepthStencil, 0);
	device->RSSetState(&EngineAPI::Statics::GraphicsStatics::DefaultPipelineState_Rasterizer);

	//1) Depth
	//
	gbufferSRV = gm->GetSwapchain()->GetSwapchainDepthTexture2DShaderResourceView();

	device->RSSetViewport(&vp);
	device->PSSetShader(&EngineAPI::Statics::GraphicsStatics::Debug_GBufferVis_Depth_PS);
	device->PSSetShaderResource(gbufferSRV, 0);
	device->Draw(4, 0);

	//2) Colour
	//
	vp.TopLeftX += w;
	gbufferSRV = gm->GetGBuffer()->GetRenderTargetAtIndex(0)->GetRenderTargetShaderResourceView();

	device->RSSetViewport(&vp);
	device->PSSetShader(&EngineAPI::Statics::GraphicsStatics::Debug_GBufferVis_Colour_PS);
	device->PSSetShaderResource(gbufferSRV, 0);
	device->Draw(4, 0);

	//3) Spec Intensity 
	//
	vp.TopLeftX += w;
	device->RSSetViewport(&vp);
	device->PSSetShader(&EngineAPI::Statics::GraphicsStatics::Debug_GBufferVis_SpecIntensity_PS);
	device->Draw(4, 0);

	//4.a) Packed normal [0,1] || 4.b) Unpacked normals [-1,1]
	//
	vp.TopLeftX += w;
	gbufferSRV = gm->GetGBuffer()->GetRenderTargetAtIndex(1)->GetRenderTargetShaderResourceView();
	device->PSSetShaderResource(gbufferSRV, 0);
	device->RSSetViewport(&vp);

	if (showPackedNormals)
		device->PSSetShader(&EngineAPI::Statics::GraphicsStatics::Debug_GBufferVis_PackedNormal_PS);
	else
		device->PSSetShader(&EngineAPI::Statics::GraphicsStatics::Debug_GBufferVis_UnpackedNormal_PS);

	device->Draw(4, 0);

	//5) SpecPower
	//
	vp.TopLeftX += w;
	gbufferSRV = gm->GetGBuffer()->GetRenderTargetAtIndex(2)->GetRenderTargetShaderResourceView();

	device->RSSetViewport(&vp);
	device->PSSetShader(&EngineAPI::Statics::GraphicsStatics::Debug_GBufferVis_SpecPower_PS);
	device->PSSetShaderResource(gbufferSRV, 0);
	device->Draw(4, 0);

	//Cleanup
	device->RSSetViewport(nullptr);
	device->PSSetShaderResource(nullptr, 0);
}

void DebugRendering::DebugRenderPointLight(EngineAPI::Rendering::PointLight* pointLight, 
	EngineAPI::Rendering::VirtualCamera* mainCamera)
{
	//Device and manager
	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();

	assert(pointLight);
	assert(mainCamera);
	assert(device);

	//Set state
	device->OMSetDepthStencilState(&EngineAPI::Statics::GraphicsStatics::LightVisualisation_DepthStencilState, 0);
	device->OMSetBlendState(&EngineAPI::Statics::GraphicsStatics::LightVisualisation_BlendState);
	device->RSSetState(&EngineAPI::Statics::GraphicsStatics::LightVisualisation_RasterizerState);

	//Shaders - not, can reuse the VS, HS, DS from the lighting pass - Simple 
	//output colour PS (the colour of the light)
	device->VSSetShader(&EngineAPI::Statics::GraphicsStatics::LightPass_PointLight_VS);
	device->HSSetShader(&EngineAPI::Statics::GraphicsStatics::LightPass_PointLight_HS);
	device->DSSetShader(&EngineAPI::Statics::GraphicsStatics::LightPass_PointLight_DS);
	device->PSSetShader(&EngineAPI::Statics::GraphicsStatics::LightVisualisation_PointLight_PS);

	//IA state
	device->IASetTopology(PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST);
	device->IASetVertexBuffer(nullptr, 0, 0);
	device->IASetIndexBuffer(nullptr, 0);

	//Render the spot light
	pointLight->Render(mainCamera);

	//Reset shaders
	device->VSSetShader(nullptr);
	device->HSSetShader(nullptr);
	device->DSSetShader(nullptr);
	device->PSSetShader(nullptr);
}

void DebugRendering::DebugRenderSpotLight(EngineAPI::Rendering::SpotLight* spotLight, 
	EngineAPI::Rendering::VirtualCamera* mainCamera)
{
	//Device and manager
	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();

	assert(spotLight);
	assert(mainCamera);
	assert(device);

	//Set state
	device->OMSetDepthStencilState(&EngineAPI::Statics::GraphicsStatics::LightVisualisation_DepthStencilState, 0);
	device->OMSetBlendState(&EngineAPI::Statics::GraphicsStatics::LightVisualisation_BlendState);
	device->RSSetState(&EngineAPI::Statics::GraphicsStatics::LightVisualisation_RasterizerState);
	
	//Shaders - not, can reuse the VS, HS, DS from the lighting pass - Simple 
	//output colour PS (the colour of the light)
	device->VSSetShader(&EngineAPI::Statics::GraphicsStatics::LightPass_SpotLight_VS);
	device->HSSetShader(&EngineAPI::Statics::GraphicsStatics::LightPass_SpotLight_HS);
	device->DSSetShader(&EngineAPI::Statics::GraphicsStatics::LightPass_SpotLight_DS);
	device->PSSetShader(&EngineAPI::Statics::GraphicsStatics::LightVisualisation_SpotLight_PS);

	//IA state
	device->IASetTopology(PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST);
	device->IASetVertexBuffer(nullptr, 0, 0);
	device->IASetIndexBuffer(nullptr, 0);

	//Render the spot light
	spotLight->Render(mainCamera);

	//Reset shaders
	device->VSSetShader(nullptr);
	device->HSSetShader(nullptr);
	device->DSSetShader(nullptr);
	device->PSSetShader(nullptr);
}
