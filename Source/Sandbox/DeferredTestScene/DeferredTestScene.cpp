#include "DeferredTestScene.h"

//Subsystems
#include <Engine/Graphics/GraphicsManager/GraphicsManager.h>

bool DeferredTestScene::OnAddToSceneManagerList()
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage("SponzaScene::OnAddToSceneManagerList()\n");

	//Init last mouse position
	lastMPos.x = 0;
	lastMPos.y = 0;

	//Done
	return true;
}

bool DeferredTestScene::OnSceneBecomeActive()
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage("SponzaScene::OnSceneBecomeActive()\n");

	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();
	float screenW = (float)gm->GetWindowWidth();
	float screenH = (float)gm->GetWindowHeight();

	//Init camera
	mainCamera.SetDebugName("SponzaScene_MainCamera");
	mainCamera.InitCameraViewProperties(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(XMFLOAT3(0.0f, 0.0f, -1.0f)), XMFLOAT3(0.0f, 1.0f, 0.0f));
	mainCamera.InitCameraPerspectiveProjectionProperties(45.0f, (screenW / screenH), 0.1f, 100.0f);

	//Init cube
	XMMATRIX cubeWorld = XMMatrixTranslation(0.f, 0.f, -5.0f);

	cube.InitCube(device,
		cubeWorld,
		std::string(ASSETS_FOLDER"Sponza/textures/sponza_curtain_blue_diff.png"), true,
		std::string("TestCube"));

	//Init CBuffer
	InitCBuffer();

	//Init Render Targets
	InitRenderTargets();

	//Done
	return true;
}

void DeferredTestScene::InitCBuffer()
{
	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();

	XMMATRIX wvp = (cube.GetWorld() * mainCamera.GetView()) * mainCamera.GetProj();
	wvp = XMMatrixTranspose(wvp);
	XMFLOAT4X4 wvp4x4;
	XMStoreFloat4x4(&wvp4x4, wvp);

	assert(constantBuffer.InitConstantBuffer(device,
		sizeof(XMFLOAT4X4), &wvp4x4,
		RESOURCE_USAGE_DYNAMIC, RESOURCE_CPU_ACCESS_WRITE_BIT,
		RESOURCE_BIND_CONSTANT_BUFFER_BIT,
		std::string("DeferredTestScene_CB")));
}

void DeferredTestScene::InitRenderTargets()
{
	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();
	uint32_t screenW = (uint32_t)gm->GetWindowWidth();
	uint32_t screenH = (uint32_t)gm->GetWindowHeight();

	//assert(renderTarget.InitRenderTarget(screenW, screenH, RESOURCE_FORMAT_R8G8B8A8_UNORM, true, std::string("DeferredScene_TestRenderTarget")));
	
	//assert(renderTarget.InitMSAARenderTarget(screenW, screenH, GRAPHICS_CONFIG_MSAA_SAMPLE_COUNT,
	//	RESOURCE_FORMAT_R8G8B8A8_UNORM, true, std::string("DeferredScene_TestRenderTarget")));
	
	assert(renderTarget.InitRenderTargetWithDepthStencilTexture(screenW, screenH, RESOURCE_FORMAT_R8G8B8A8_UNORM,
		DEPTH_STENCIL_FORMAT_D24_UNORM_S8_UINT, true, std::string("DeferredScene_TestRenderTarget")));

	//assert(renderTarget.InitMSAARenderTargetWithDepthStencilTexture(screenW, screenH, GRAPHICS_CONFIG_MSAA_SAMPLE_COUNT,
	//	RESOURCE_FORMAT_R8G8B8A8_UNORM,
	//	DEPTH_STENCIL_FORMAT_D24_UNORM_S8_UINT, true, std::string("DeferredScene_TestRenderTarget")));


	//
	//
	//
	ResourceFormat fmts[3];
	fmts[0] = RESOURCE_FORMAT_R8G8B8A8_UNORM;
	fmts[1] = RESOURCE_FORMAT_R11G11B10_FLOAT;
	fmts[2] = RESOURCE_FORMAT_R8G8B8A8_UNORM;

	assert(renderTargetSet.InitRenderTargetSet(screenW, screenH, 3,
		&fmts[0], true, std::string("RenderTargetSet")));

	//assert(renderTargetSet.InitRenderTargetSetWithADepthStencilTexture(screenW, screenH, 3,
	//	&fmts[0], DEPTH_STENCIL_FORMAT_D24_UNORM_S8_UINT,
	//	true, std::string("RenderTargetSet")));
}

bool DeferredTestScene::OnSceneBecomeDeactive()
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage("SponzaScene::OnSceneBecomeDeactive()\n");

	//Shutdown
	mainCamera.Shutdown();
	constantBuffer.Shutdown();
	cube.Shutdown();

	renderTarget.Shutdown();
	renderTargetSet.Shutdown();

	//Done
	return true;
}

bool DeferredTestScene::OnRemoveFromSceneManagerList()
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage("SponzaScene::OnRemoveFromSceneManagerList()\n");

	//Done
	return true;
}

bool DeferredTestScene::OnEngineShutdown()
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage("SponzaScene::OnFinalShutdown()\n");

	//Done
	return true;
}

bool DeferredTestScene::OnResize(uint32_t newWidth, uint32_t newHeight)
{
	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	float screenW = (float)gm->GetWindowWidth();
	float screenH = (float)gm->GetWindowHeight();

	//Resize camera
	mainCamera.InitCameraPerspectiveProjectionProperties(45.0f, screenW / screenH, 0.1f, 100.0f);

	//Resize render target
	assert(renderTarget.Resize(newWidth, newHeight));
	assert(renderTargetSet.Resize(newWidth, newHeight));

	//Done
	return true;
}

//input subsystem
void DeferredTestScene::OnMouseDown(WPARAM btnState, int x, int y)
{
	lastMPos.x = x;
	lastMPos.y = y;
}

void DeferredTestScene::OnMouseUp(WPARAM btnState, int x, int y)
{

}

void DeferredTestScene::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		const float CAM_ROT_RATE_DX = 0.15f;
		const float CAM_ROT_RATE_DY = 0.20f;

		// Make each pixel correspond to a quarter of a degree.
		float dx = CAM_ROT_RATE_DX*static_cast<float>(x - lastMPos.x);
		float dy = CAM_ROT_RATE_DY*static_cast<float>(y - lastMPos.y);

		mainCamera.AddPitch(dy);
		mainCamera.AddYaw(dx);
	}

	lastMPos.x = x;
	lastMPos.y = y;
}

bool DeferredTestScene::OnSceneUpdate(float dt)
{
	const float WALK = 7.5f;
	const float STRAFE = 7.5f;

	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();

	//Update camera
	if (GetAsyncKeyState('W') & 0x8000)
		mainCamera.Walk(WALK*dt);
	else if (GetAsyncKeyState('S') & 0x8000)
		mainCamera.Walk(-WALK*dt);

	if (GetAsyncKeyState('A') & 0x8000)
		mainCamera.Strafe(-STRAFE*dt);
	else if (GetAsyncKeyState('D') & 0x8000)
		mainCamera.Strafe(STRAFE*dt);

	//Rebuild view matrix
	mainCamera.RebuildView();

	//Update CBuffer
	//
	//New WVP
	XMMATRIX wvp = (cube.GetWorld() * mainCamera.GetView()) * mainCamera.GetProj();
	wvp = XMMatrixTranspose(wvp);
	XMFLOAT4X4 wvp4x4;
	XMStoreFloat4x4(&wvp4x4, wvp);

	MappedResourceData mrd;
	bool didMap = constantBuffer.MapResource(device, 0, RESOURCE_MAP_WRITE_DISCARD, &mrd);
	if (didMap)
		memcpy(mrd.MappedData, (void*)&wvp4x4, constantBuffer.GetConstantBufferSizeBytes());
	constantBuffer.UnmapResource(device, 0);

	//Done
	return true;
}

bool DeferredTestScene::OnScenePreRender()
{
	//Done
	return true;
}

bool DeferredTestScene::OnSceneRenderGeometryPass()
{
	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();

	/*
	//Clear render target
	renderTarget.ClearRenderTarget(Float32Colour(0.f, 0.f, 1.f, 0.f));
	if (renderTarget.DoesManageADepthStencilTexture())
	renderTarget.ClearDepthStencilTexture(DEPTH_STENCIL_BUFFER_CLEAR_DEPTH_BIT | DEPTH_STENCIL_BUFFER_CLEAR_STENCIL_BIT, 1.0f, 0);

	//Bind render target
	if (renderTarget.DoesManageADepthStencilTexture())
	renderTarget.BindRenderTargetAndDepthStencilTextureAsOutput(true);
	else
	{
	//renderTarget.BindRenderTargetOnlyAsOutput();
	renderTarget.BindRenderTargetWithExternalDepthStencilViewAsOutput(
	gm->GetSwapchain()->GetSwapchainDepthTexture2DReadWriteView());
	}
	*/

	//
	//Render target set
	//
	renderTargetSet.ClearAllRenderTargets(Float32Colour(0.0f, 0.0f, 1.0f, 1.0f));
	if (renderTargetSet.DoesManageDepthStencilTexture())
		renderTargetSet.ClearDepthStencilTexture(DEPTH_STENCIL_BUFFER_CLEAR_DEPTH_BIT | DEPTH_STENCIL_BUFFER_CLEAR_STENCIL_BIT, 1.0f, 0);

	if (renderTargetSet.DoesManageDepthStencilTexture())
		renderTargetSet.BindAllRenderTargetsAndDepthStencilTextureAsOutput(true);
	else
	{
		renderTargetSet.BindAllRenderTargetsAndExternalDepthStencilViewAsOutput(
			gm->GetSwapchain()->GetSwapchainDepthTexture2DReadWriteView());

		//renderTargetSet.BindAllRenderTargetsAsOutput();
	}

	//Set CBuffer
	device->VSSetConstantBuffer(&constantBuffer, 0);

	//Draw cube
	cube.RenderToGBuffer(device);

	//Done
	return true;
}

bool DeferredTestScene::OnSceneRenderLightPass()
{
	//Done
	return true;
}

bool DeferredTestScene::OnSceneRenderPostProcessPass()
{
	//Done
	return true;
}

bool DeferredTestScene::OnSceneRenderDebugPass()
{
	//Done
	return true;
}

bool DeferredTestScene::OnSceneRenderUIPass()
{
	//Done
	return true;
}

bool DeferredTestScene::OnSceneRenderDebugUIPass()
{
	//Done
	return true;
}