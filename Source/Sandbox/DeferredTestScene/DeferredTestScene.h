//DeferredTestScene.h
//Created 17/07/17
//Created By Daniel Bowler
//
//Deferred rendering test scene

#pragma once

//Parent class
#include <Engine/Gameplay/Scene/Scene.h>

//Rendering && graphics
#include <Engine/Rendering/VirtualCamera/VirtualCamera.h>
#include <Engine/Rendering/DirectionalLight/DirectionalLight.h>
#include <Engine/Rendering/PointLight/PointLight.h>
#include <Engine/Rendering/SpotLight/SpotLight.h>

//Cube + plane we render
#include <Sandbox/TestCube/TestCube.h>
#include <Sandbox/TestPlane/TestPlane.h>

class DeferredTestScene : public EngineAPI::Gameplay::Scene
{
public:
	DeferredTestScene() {};
	~DeferredTestScene() {};

	//Load and unload
	bool OnAddToSceneManagerList() override;
	bool OnSceneBecomeActive() override;
	bool OnSceneBecomeDeactive() override;
	bool OnRemoveFromSceneManagerList() override;
	bool OnEngineShutdown() override;

	//Engine events
	bool OnResize(uint32_t newWidth, uint32_t newHeight) override;

	//TEMP: Manage the camera rotation this way until I have a proper
	//input subsystem
	void OnMouseDown(WPARAM btnState, int x, int y) override;
	void OnMouseUp(WPARAM btnState, int x, int y) override;
	void OnMouseMove(WPARAM btnState, int x, int y) override;

	//Rendering and updating the scene - NOTE: I will add
	//an update/render loop thats used internally. 
	bool OnSceneUpdate(float dt) override;

	bool OnScenePreRender() override;
	bool OnSceneRenderGeometryPass() override;
	bool OnSceneRenderLightPass(LightPassMode mode) override;
	bool OnSceneRenderPostProcessPass() override;
	bool OnSceneRenderDebugPass()  override;
	bool OnSceneRenderUIPass() override;
	bool OnSceneRenderDebugUIPass() override;

private:
	//Fly camera
	EngineAPI::Rendering::VirtualCamera mainCamera;
	POINT lastMPos; //Last mouse position - used when rotating the camera

	//CBuffer (WVP)
	EngineAPI::Graphics::ConstantBuffer camerConstantBuffer;

	//Lights
	EngineAPI::Rendering::DirectionalLight dLight;
	EngineAPI::Rendering::PointLight pLights[5];
	EngineAPI::Rendering::SpotLight sLight;

	//Cube
	TestCube cube;
	TestPlane plane;

	//Debug flags
	bool shouldDebugDrawGBuffer = false;
	bool shouldDebugDrawGBufferPackedNormals = true;
	bool shouldDebugDrawLights = false;

	float atbBarsToggleCooldownTimer = 0.3f;
	float shouldDebugDrawGBufferCooldownTimer = 0.3f;
	float shouldDebugDrawGBufferPackedNormalsCooldownTimer = 0.3f;
	float shouldDebugDrawLightsCooldownTimer = 0.3f;

private:
	void InitCameraCBuffer();
};
