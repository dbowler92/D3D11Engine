#include "D3D11GraphicsManager.h"

//Will bind engine static resources as required
#include <Statics/GraphicsStatics/GraphicsStatics.h>

//Talks to the scene manager for rendering events
#include <Gameplay/SceneManager/SceneManager.h>

using namespace EngineAPI::Graphics::Platform;

bool D3D11GraphicsManager::InitSubsystem(EngineAPI::OS::OSWindow* osWindow)
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage(__FUNCTION__);

	SetDebugName("D3D11GraphicsManager");

	//Cache window size
	windowWidth = osWindow->GetWindowWidth();
	windowHeight = osWindow->GetWindowHeight();

	//Create the device && context(TODO: s -. Deferred context)
	assert(device.InitD3D11DeviceAndImmediateContext(osWindow));

	//Create swapchain + depth buffer
	assert(swapchain.InitD3D11Swapchain(&device, osWindow, 
		GRAPHICS_CONFIG_BACKBUFFER_COUNT, 1, true));

	//Init deferred rendering
	assert(deferredGBuffer.InitGBuffer(windowWidth, windowHeight, "GBuffer"));
	assert(deferredLABuffer.InitLABuffer(windowWidth, windowHeight, "LABuffer"));

	return true;
}

void D3D11GraphicsManager::ShutdownSubsystem()
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage(__FUNCTION__);

	//Destroy deferred rendering
	deferredLABuffer.Shutdown();
	deferredGBuffer.Shutdown();

	//Destroy system swapchain
	swapchain.Shutdown();

	//Lastly, shutdown device and context
	device.Shutdown();

	//Super
	__super::ShutdownSubsystem();
}

bool D3D11GraphicsManager::OnResize(EngineAPI::OS::OSWindow* osWindow)
{
	//Cache new window size
	windowWidth = osWindow->GetWindowWidth();
	windowHeight = osWindow->GetWindowHeight();

	//Resize GBuffer and LABuffer
	deferredLABuffer.OnResize(windowWidth, windowHeight);
	deferredGBuffer.OnResize(windowWidth, windowHeight);

	//Resize the swapchain
	if (!swapchain.OnResize(&device, osWindow))
		return false;

	//Done
	return true;
}

bool D3D11GraphicsManager::OnRender()
{
	//Get the scene manager
	EngineAPI::Gameplay::SceneManager* sm = EngineAPI::Gameplay::SceneManager::GetInstance();
	assert(sm);

	//Pre render - eg: Shadows and the like
	assert(sm->OnPreRender());

	//Clear everything ready for new frame
	assert(OnBeginRender());

	//Rendering passes. TODO: GraphicsManager will need functions to
	//set itself up for these passes before informing the scene
	//to render
	assert(OnBeginGeometryPass());
	assert(sm->OnRenderGeometryPass());

	assert(OnBeginLightPass());
	assert(sm->OnRenderLightPass());

	assert(OnBeginPostProcessPass());
	//assert(sm->OnRenderPostProcessPass());

	assert(OnBeginDebugPass());
	//assert(sm->OnRenderDebugPass());

	assert(OnBeginUIPass());
	//assert(sm->OnRenderUIPass());

	assert(OnBeginDebugUIPass());
	assert(sm->OnRenderDebugUIPass());

	//Present the backbuffer to the user
	assert(OnEndRender());

	//Done
	return true;
}

bool D3D11GraphicsManager::OnBeginRender()
{
	//Clear swapchain buffer (+ depth) ready for rendering
	const Float32Colour clearColour = { 0.0f, 0.0f, 0.0f, 0.0f };
	swapchain.ClearSwapchainBackbufferRenderTarget(&device, (float*)&clearColour);
	swapchain.ClearDepthStencilBuffer(&device, DEPTH_STENCIL_BUFFER_CLEAR_DEPTH_BIT | DEPTH_STENCIL_BUFFER_CLEAR_STENCIL_BIT, 1.0f, 0);

	//Clear GBuffer
	deferredGBuffer.Clear();

	//Clear LABuffer
	deferredLABuffer.Clear();

	//Done
	return true;
}

bool D3D11GraphicsManager::OnBeginGeometryPass()
{
	//Bind the GBuffer as output with swapchain depth stencil
	//texture - ReadWrite enabled
	deferredGBuffer.BindGBufferForGeometryPass(swapchain.GetSwapchainDepthTexture2DReadWriteView());

	//Null states
	device.OMSetBlendState(&EngineAPI::Statics::GraphicsStatics::DefaultPipelineState_Blend);
	device.OMSetDepthStencilState(&EngineAPI::Statics::GraphicsStatics::DefaultPipelineState_DepthStencil, 0);
	device.RSSetState(&EngineAPI::Statics::GraphicsStatics::DefaultPipelineState_Rasterizer);

	//Fullscreen render
	swapchain.SetFullResolutionViewport(&device);



	//Done
	return true;
}

bool D3D11GraphicsManager::OnBeginLightPass()
{
	//Unbind GBuffer
	deferredGBuffer.UnbindGBufferAfterGeometryPass();

	//Bind LA Buffer with swapchain read-only depth - we want depth testing (read)
	//but also reading from within the lighting PS
	//
	//TODO: HDR
	deferredLABuffer.BindLABufferForLightingPass(swapchain.GetSwapchainDepthTexture2DReadOnlyView());

	//Bind the GBuffer SRVs to the pipeline:
	//
	//Texture2D GBuffer_Depth                : register(t0);
	//Texture2D GBuffer_DiffuseSpecIntensity : register(t1);
	//Texture2D GBuffer_PackedNormal         : register(t2);
	//Texture2D GBuffer_SpecPower			 : register(t3);
	deferredGBuffer.BindGBufferForLightPass(swapchain.GetSwapchainDepthTexture2DShaderResourceView());

	//Null states
	device.OMSetBlendState(&EngineAPI::Statics::GraphicsStatics::DefaultPipelineState_Blend);
	device.OMSetDepthStencilState(&EngineAPI::Statics::GraphicsStatics::DefaultPipelineState_DepthStencil, 0);
	device.RSSetState(&EngineAPI::Statics::GraphicsStatics::DefaultPipelineState_Rasterizer);

	//Fullscreen render
	swapchain.SetFullResolutionViewport(&device);

	//Done
	return true;
}

bool D3D11GraphicsManager::OnBeginPostProcessPass()
{
	//Unbind GBuffer from (lighting pass) pixel shader
	deferredGBuffer.UnbindGBufferAfterLightPass();

	//Bind the swapchain (for now - may need ping-pong buffers)
	//
	//No depth buffer needs to be bound for testing
	swapchain.BindSwapchainBackbufferAsRenderTarget(&device, nullptr);

	//Null states
	device.OMSetBlendState(&EngineAPI::Statics::GraphicsStatics::DefaultPipelineState_Blend);
	device.OMSetDepthStencilState(&EngineAPI::Statics::GraphicsStatics::DefaultPipelineState_DepthStencil, 0);
	device.RSSetState(&EngineAPI::Statics::GraphicsStatics::DefaultPipelineState_Rasterizer);

	//Fullscreen render
	swapchain.SetFullResolutionViewport(&device);

	//TEMP: For now, blit the results of the LA buffer in to the swapchain backbuffer
	//
	//
	//Shaders:
	device.VSSetShader(&EngineAPI::Statics::GraphicsStatics::Blit_VS);
	device.PSSetShader(&EngineAPI::Statics::GraphicsStatics::Blit_PS);

	//NULL VB and IB -> We generate the quad in the shader
	device.IASetTopology(PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	device.IASetVertexBuffer(nullptr, 0, 0);
	device.IASetIndexBuffer(nullptr, 0);

	//Texture to blit in slot[0]
	assert(deferredLABuffer.GetLABufferSRV());
	device.PSSetShaderResource(deferredLABuffer.GetLABufferSRV(), 0);

	//Draw 
	device.Draw(4, 0);

	//Done
	return true;
}

bool D3D11GraphicsManager::OnBeginDebugPass()
{
	//Render directly in to the backbuffer

	//Done
	return true;
}

bool D3D11GraphicsManager::OnBeginUIPass()
{
	//Render directly in to the backbuffer over the top of
	//scene elements (depth test off)

	//Done
	return true;
}

bool D3D11GraphicsManager::OnBeginDebugUIPass()
{
	//As above, render directly in to the backbuffer
	//& over the top of other elements 2D/3D
	//
	//Bind the swapchain (for now - may need ping-pong buffers)
	//
	//No depth buffer needs to be bound for testing
	swapchain.BindSwapchainBackbufferAsRenderTarget(&device, nullptr);

	//Null states
	device.OMSetBlendState(&EngineAPI::Statics::GraphicsStatics::DefaultPipelineState_Blend);
	device.OMSetDepthStencilState(&EngineAPI::Statics::GraphicsStatics::DefaultPipelineState_DepthStencil, 0);
	device.RSSetState(&EngineAPI::Statics::GraphicsStatics::DefaultPipelineState_Rasterizer);

	//Default to fullscreen render
	swapchain.SetFullResolutionViewport(&device);

	//Done
	return true;
}

bool D3D11GraphicsManager::OnEndRender()
{
	//Present backbuffer
	swapchain.PresentSwapchainBackbuffer();

	//Done
	return true;
}