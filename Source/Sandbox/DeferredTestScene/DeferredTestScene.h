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
#include <Engine/Rendering/RenderTarget/RenderTarget.h>

//Cube we render
#include <Sandbox/TestCube/TestCube.h>

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
	bool OnSceneRender() override;

private:
	//Fly camera
	EngineAPI::Rendering::VirtualCamera mainCamera;
	POINT lastMPos; //Last mouse position - used when rotating the camera

	//Render target
	EngineAPI::Rendering::RenderTarget renderTarget;

	//CBuffer (WVP)
	EngineAPI::Graphics::ConstantBuffer constantBuffer;

	//Cube
	TestCube cube;

private:
	void InitCBuffer();
	void InitRenderTargets();
};
