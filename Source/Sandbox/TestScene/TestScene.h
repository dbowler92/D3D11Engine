//TestScene.h
//Created 30/06/17
//Created By Daniel Bowler
//
//Simple test scene used when creating the engine

#pragma once

//Parent class
#include "../../../Engine/Gameplay/Scene/Scene.h"

//Test includes
#include "../../../Engine/Graphics/VertexBuffer/VertexBuffer.h"
#include "../../../Engine/Graphics/IndexBuffer/IndexBuffer.h"

#include "../../../Engine/Graphics/VertexShader/VertexShader.h"
#include "../../../Engine/Graphics/PixelShader/PixelShader.h"

#include "../../../Engine/Graphics/DepthStencilState/DepthStencilState.h"
#include "../../../Engine/Graphics/RasterizerState/RasterizerState.h"

class TestScene : public EngineAPI::Gameplay::Scene
{
public:
	TestScene() {};
	~TestScene() {};

	//Load and unload
	bool OnAddToSceneManagerList() override;
	bool OnSceneBecomeActive() override;
	bool OnSceneBecomeDeactive() override;
	bool OnRemoveFromSceneManagerList() override;
	bool OnEngineShutdown() override;

	//Rendering and updating the scene - NOTE: I will add
	//an update/render loop thats used internally. 
	bool OnSceneUpdate(float dt) override;
	bool OnSceneRender() override;

private:
	//Test resources
	EngineAPI::Graphics::VertexBuffer vb;

	EngineAPI::Graphics::VertexBuffer indexedVB;
	EngineAPI::Graphics::IndexBuffer ib;

	EngineAPI::Graphics::VertexShader vs;
	EngineAPI::Graphics::PixelShader ps;

	EngineAPI::Graphics::VertexBuffer stateVB;
	EngineAPI::Graphics::IndexBuffer stateIB;
	EngineAPI::Graphics::DepthStencilState dss;
	EngineAPI::Graphics::RasterizerState rss;

private:
	void TestVB();
	void TestShaders();
	void TestIB();
	void TestStates();
};
