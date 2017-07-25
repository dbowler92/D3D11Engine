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
	mainCamera.InitCameraViewProperties(XMFLOAT3(0.0f, 0.0f, -5.0f), XMFLOAT3(XMFLOAT3(0.0f, 0.0f, +1.0f)), XMFLOAT3(0.0f, 1.0f, 0.0f));
	mainCamera.InitCameraPerspectiveProjectionProperties(45.0f, (screenW / screenH), 0.1f, 100.0f);
	mainCamera.RebuildView();

	//Init cube
	XMMATRIX cubeWorld = XMMatrixTranslation(0.f, 0.f, 0.0f);
	cube.InitCube(device,
		cubeWorld,
		std::string(ASSETS_FOLDER"Sponza/textures/sponza_curtain_blue_diff.png"), true,
		128.0f, 0.75f,
		std::string("TestCube"));
	
	//Init plane
	XMMATRIX planeWorld = XMMatrixTranslation(0.0f, -1.f, 0.0f);
	plane.InitPlane(device,
		planeWorld,
		5.f, 5.f, 
		std::string(ASSETS_FOLDER"Textures/TestTextures/bricks.dds"),
		std::string(ASSETS_FOLDER"Textures/TestTextures/bricks_nmap.dds"),
		128.0f, 0.5f,
		std::string("TestPlane"));

	//Init camera CBuffer
	InitCameraCBuffer();

	//Init d light
	dLight.InitDirectionalLightSource(XMFLOAT3(1.f, 0.f, 1.f), 
		XMFLOAT3(0.4f, 0.4f, 0.4f), 1.0f, "DLight");
	dLight.SetActiveState(true);

	//Init p lights
	XMFLOAT3 pLightPositions[3] =
	{
		XMFLOAT3(-2.0f, 0.0f, 0.0f),
		XMFLOAT3(+2.0f, 0.0f, 0.0f),
		XMFLOAT3(0.0f, +2.0f, 0.0f)
	};

	XMFLOAT3 pLightColours[3] =
	{
		XMFLOAT3(0.6f, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 0.6f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, 0.6f),
	};

	for (int i = 0; i < 3; i++)
	{
		pLights[i].InitPointLightSource(pLightPositions[i], 3.f, 
			pLightColours[i], 1.f,
			std::string("PLight_") + std::to_string(i));
		pLights[i].SetActiveState(true);
	}

	//Done
	return true;
}

void DeferredTestScene::InitCameraCBuffer()
{
	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();

	VirtualCameraGraphicsData camInitData;
	XMStoreFloat4x4(&camInitData.View, XMMatrixIdentity());
	XMStoreFloat4x4(&camInitData.Proj, XMMatrixIdentity());
	XMStoreFloat4x4(&camInitData.ViewProj, XMMatrixIdentity());
	XMStoreFloat4x4(&camInitData.InverseView, XMMatrixIdentity());
	camInitData.CameraWorldPosition = XMFLOAT3(0.f, 0.f, 0.f);
	camInitData._Pad0 = 0.f;
		
	assert(camerConstantBuffer.InitConstantBuffer(device,
		sizeof(VirtualCameraGraphicsData), &camInitData,
		RESOURCE_USAGE_DYNAMIC, RESOURCE_CPU_ACCESS_WRITE_BIT,
		RESOURCE_BIND_CONSTANT_BUFFER_BIT,
		std::string("DeferredTestScene_CameraCB")));
}

bool DeferredTestScene::OnSceneBecomeDeactive()
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage("SponzaScene::OnSceneBecomeDeactive()\n");

	//Shutdown
	mainCamera.Shutdown();
	camerConstantBuffer.Shutdown();
	cube.Shutdown();
	plane.Shutdown();
	dLight.Shutdown();

	for (int i = 0; i < 3; i++)
		pLights[i].Shutdown();

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

	//Update camera CBuffer
	XMFLOAT4X4 viewAndProj[3];
	XMStoreFloat4x4(&viewAndProj[0], XMMatrixTranspose(mainCamera.GetView()));
	XMStoreFloat4x4(&viewAndProj[1], XMMatrixTranspose(mainCamera.GetProj()));
	XMStoreFloat4x4(&viewAndProj[2], XMMatrixTranspose(mainCamera.GetViewProj()));

	VirtualCameraGraphicsData cameraUpdatedData;
	cameraUpdatedData.View = viewAndProj[0];
	cameraUpdatedData.Proj = viewAndProj[1];
	cameraUpdatedData.ViewProj = viewAndProj[2];

	//Inverse view matrix
	XMMATRIX invView = mainCamera.CalculateInverseViewMatrix();
	XMFLOAT4X4 invView4x4;
	XMStoreFloat4x4(&invView4x4, XMMatrixTranspose(invView));
	cameraUpdatedData.InverseView = invView4x4;

	//Position
	cameraUpdatedData.CameraWorldPosition = mainCamera.GetPosition();
	cameraUpdatedData._Pad0 = 0.0f;

	MappedResourceData mrd;
	bool didMap = camerConstantBuffer.MapResource(device, 0, RESOURCE_MAP_WRITE_DISCARD, &mrd);
	if (didMap)
		memcpy(mrd.MappedData, &cameraUpdatedData, camerConstantBuffer.GetConstantBufferSizeBytes());
	camerConstantBuffer.UnmapResource(device, 0);

	//Update cube
	cube.Update(dt);

	//Update plane
	plane.Update(dt);

	//Debug draw?
	shouldDebugDrawGBufferCooldownTimer += dt;
	shouldDebugDrawGBufferPackedNormalsCooldownTimer += dt;
	if (GetAsyncKeyState('1') & 0x8000)
	{
		if (shouldDebugDrawGBufferCooldownTimer >= 0.30f)
		{
			shouldDebugDrawGBuffer = !shouldDebugDrawGBuffer;
			shouldDebugDrawGBufferCooldownTimer = 0.0f;
		}
	}
	if (GetAsyncKeyState('2') & 0x8000)
	{
		if (shouldDebugDrawGBufferPackedNormalsCooldownTimer >= 0.3f)
		{
			shouldDebugDrawGBufferPackedNormals = !shouldDebugDrawGBufferPackedNormals;
			shouldDebugDrawGBufferPackedNormalsCooldownTimer = 0.0f;
		}
	}

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

	//TEMP: Move this over to engine when I have components, etc
	//
	//Set camera CBuffer
	device->VSSetConstantBuffer(&camerConstantBuffer, GRAPHICS_CONFIG_CAMERA_CBUFFER_BINDING_SLOT);

	//Draw cube
	cube.RenderToGBuffer(device);

	//And plane
	plane.RenderToGBuffer(device);

	//Done
	return true;
}

bool DeferredTestScene::OnSceneRenderLightPass()
{
	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();

	//TEMP: This needs moving engine level -> Will do once I have
	//camera components and the concept of a "main camera"
	//
	//Bind the camera data to the PS
	device->PSSetConstantBuffer(&camerConstantBuffer, GRAPHICS_CONFIG_CAMERA_CBUFFER_BINDING_SLOT);

	//DLights
	dLight.Render();

	//PLights
	for (int i = 0; i < 3; i++)
		pLights[i].Render(&mainCamera); 

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
	//GBuffer vis
	if (shouldDebugDrawGBuffer)
		EngineAPI::Debug::DebugRendering::DebugRenderGBuffersToScreen(shouldDebugDrawGBufferPackedNormals);

	//Done
	return true;
}