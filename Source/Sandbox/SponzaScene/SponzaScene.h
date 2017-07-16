//SponzaScene.h
//Created 30/06/17
//Created By Daniel Bowler
//
//Sponza scene to demo the engine features

#pragma once

//Parent class
#include <Engine/Gameplay/Scene/Scene.h>

//Rendering && graphics
#include <Engine/Rendering/VirtualCamera/VirtualCamera.h>

#include <Engine/Graphics/VertexBuffer/VertexBuffer.h>
#include <Engine/Graphics/ConstantBuffer/ConstantBuffer.h>
#include <Engine/Graphics/VertexShader/VertexShader.h>
#include <Engine/Graphics/PixelShader/PixelShader.h>
#include <Engine/Graphics/BlendState/BlendState.h>
#include <Engine/Graphics/DepthStencilState/DepthStencilState.h>
#include <Engine/Graphics/RasterizerState/RasterizerState.h>
#include <Engine/Graphics/Texture2D/Texture2D.h>
#include <Engine/Graphics/ShaderResourceView/ShaderResourceView.h>
#include <Engine/Graphics/SamplerState/SamplerState.h>

class SponzaScene : public EngineAPI::Gameplay::Scene
{
public:
	SponzaScene() {};
	~SponzaScene() {};

	//Load and unload
	bool OnAddToSceneManagerList() override;
	bool OnSceneBecomeActive() override;
	bool OnSceneBecomeDeactive() override;
	bool OnRemoveFromSceneManagerList() override;
	bool OnEngineShutdown() override;

	//Engine events
	bool OnResize(uint32_t newWidth, uint32_t newHeight) override;

	//Rendering and updating the scene - NOTE: I will add
	//an update/render loop thats used internally. 
	bool OnSceneUpdate(float dt) override;
	bool OnSceneRender() override;

private:
	//Fly camera
	EngineAPI::Rendering::VirtualCamera mainCamera;

	//TEMP: Textured Cube rendering
	XMMATRIX cbWorld;
	XMMATRIX cbView;
	XMMATRIX cbProj;

	EngineAPI::Graphics::VertexBuffer texCubeVB;

	EngineAPI::Graphics::VertexShader texVS;
	EngineAPI::Graphics::PixelShader texPS;

	EngineAPI::Graphics::Texture2D textureFromFile;
	EngineAPI::Graphics::ShaderResourceView texSRV;
	EngineAPI::Graphics::SamplerState defaultLinearSampler;

	EngineAPI::Graphics::BlendState bs;
	EngineAPI::Graphics::DepthStencilState dss;
	EngineAPI::Graphics::RasterizerState rss;

	EngineAPI::Graphics::ConstantBuffer constantBuffer;

private:
	void InitCubeVB();
	void InitShaders();
	void InitTextureAndViewAndSampler();
	void InitPipelineStates();
	void InitCBuffer();
};
