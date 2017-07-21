#include "DebugRendering.h"

//Uses some of the engine static data - eg: Shaders
#include <Statics/GraphicsStatics/GraphicsStatics.h>

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