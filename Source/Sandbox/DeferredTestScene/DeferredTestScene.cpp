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
	mainCamera.InitCameraViewProperties(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(XMFLOAT3(0.0f, 0.0f, +1.0f)), XMFLOAT3(0.0f, 1.0f, 0.0f));
	mainCamera.InitCameraPerspectiveProjectionProperties(45.0f, (screenW / screenH), 0.1f, 100.0f);
	mainCamera.RebuildView();

	//Init cube
	XMMATRIX cubeWorld = XMMatrixTranslation(0.f, 0.f, +5.0f);

	cube.InitCube(device,
		cubeWorld,
		std::string(ASSETS_FOLDER"Sponza/textures/sponza_curtain_blue_diff.png"), true,
		64.0f, 0.75f,
		std::string("TestCube"));

	//Init camera CBuffer
	InitCameraCBuffer();

	//Init Render Targets
	InitRenderTargets();

	//Done
	return true;
}

void DeferredTestScene::InitCameraCBuffer()
{
	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();

	XMFLOAT4X4 viewAndProj[3];
	XMStoreFloat4x4(&viewAndProj[0], XMMatrixIdentity());
	XMStoreFloat4x4(&viewAndProj[1], XMMatrixIdentity());
	XMStoreFloat4x4(&viewAndProj[2], XMMatrixIdentity());
		
	assert(camerConstantBuffer.InitConstantBuffer(device,
		sizeof(XMFLOAT4X4) * 3, &viewAndProj,
		RESOURCE_USAGE_DYNAMIC, RESOURCE_CPU_ACCESS_WRITE_BIT,
		RESOURCE_BIND_CONSTANT_BUFFER_BIT,
		std::string("DeferredTestScene_CameraCB")));
}

void DeferredTestScene::InitRenderTargets()
{
	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();
	uint32_t screenW = (uint32_t)gm->GetWindowWidth();
	uint32_t screenH = (uint32_t)gm->GetWindowHeight();

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
	camerConstantBuffer.Shutdown();
	cube.Shutdown();

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

	//Resize render targets
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

	//Update camera CBuffer
	XMFLOAT4X4 viewAndProj[3];
	XMStoreFloat4x4(&viewAndProj[0], XMMatrixTranspose(mainCamera.GetView()));
	XMStoreFloat4x4(&viewAndProj[1], XMMatrixTranspose(mainCamera.GetProj()));
	XMStoreFloat4x4(&viewAndProj[2], XMMatrixTranspose(mainCamera.GetViewProj()));

	MappedResourceData mrd;
	bool didMap = camerConstantBuffer.MapResource(device, 0, RESOURCE_MAP_WRITE_DISCARD, &mrd);
	if (didMap)
		memcpy(mrd.MappedData, &viewAndProj[0], camerConstantBuffer.GetConstantBufferSizeBytes());
	camerConstantBuffer.UnmapResource(device, 0);

	//Update cube
	cube.Update(dt);

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

	//
	//Render target set
	//
	renderTargetSet.ClearAllRenderTargets(Float32Colour(0.0f, 0.0f, 0.0f, 0.0f));
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

	//Set camera CBuffer
	device->VSSetConstantBuffer(&camerConstantBuffer, 0);

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