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

bool DeferredTestScene::OnSceneBecomeDeactive()
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage("SponzaScene::OnSceneBecomeDeactive()\n");

	//Shutdown
	mainCamera.Shutdown();
	constantBuffer.Shutdown();
	cube.Shutdown();

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

bool DeferredTestScene::OnSceneRender()
{
	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();

	//Set CBUffer
	device->VSSetConstantBuffer(&constantBuffer, 0);

	//Draw cube
	cube.Render(device);

	//Done
	return true;
}
