//TestScene.h
//Created 30/06/17
//Created By Daniel Bowler
//
//Simple test scene used when creating the engine

#pragma once

//Parent class
#include <Engine/Gameplay/Scene/Scene.h>

//Test includes
#include <Engine/Graphics/VertexBuffer/VertexBuffer.h>
#include <Engine/Graphics/IndexBuffer/IndexBuffer.h>

#include <Engine/Graphics/VertexShader/VertexShader.h>
#include <Engine/Graphics/PixelShader/PixelShader.h>

#include <Engine/Graphics/BlendState/BlendState.h>
#include <Engine/Graphics/DepthStencilState/DepthStencilState.h>
#include <Engine/Graphics/RasterizerState/RasterizerState.h>

#include <Engine/Graphics/ConstantBuffer/ConstantBuffer.h>

#include <Engine/Graphics/RenderTexture2D/RenderTexture2D.h>
#include <Engine/Graphics/RenderTargetView/RenderTargetView.h>
#include <Engine/Graphics/DepthStencilTexture2D/DepthStencilTexture2D.h>
#include <Engine/Graphics/DepthStencilView/DepthStencilView.h>

#include <Engine/Graphics/Texture2D/Texture2D.h>
#include <Engine/Graphics/ShaderResourceView/ShaderResourceView.h>
#include <Engine/Graphics/SamplerState/SamplerState.h>

#include <Engine/Rendering/VirtualCamera/VirtualCamera.h>

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

	//Engine events
	bool OnResize(uint32_t newWidth, uint32_t newHeight) override;

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
	EngineAPI::Graphics::BlendState bs;
	EngineAPI::Graphics::DepthStencilState dss;
	EngineAPI::Graphics::RasterizerState rss;

	EngineAPI::Graphics::VertexShader cbVS;
	EngineAPI::Graphics::PixelShader cbPS;
	EngineAPI::Graphics::VertexBuffer cbVB;
	EngineAPI::Graphics::IndexBuffer cbIB;
	EngineAPI::Graphics::ConstantBuffer constantBuffer;
	XMMATRIX cbWorld;
	XMMATRIX cbView;
	XMMATRIX cbProj;

	EngineAPI::Graphics::RenderTexture2D renderTgt;
	EngineAPI::Graphics::RenderTargetView renderTgtView;
	EngineAPI::Graphics::DepthStencilTexture2D depthTexture;
	EngineAPI::Graphics::DepthStencilView depthTextureView;

	EngineAPI::Graphics::VertexBuffer texCubeVB;
	EngineAPI::Graphics::VertexShader texVS;
	EngineAPI::Graphics::PixelShader texPS;
	EngineAPI::Graphics::Texture2D textureFromFile;
	EngineAPI::Graphics::ShaderResourceView texSRV;
	EngineAPI::Graphics::SamplerState defaultLinearSampler;

private:
	void TestVB();
	void TestShaders();
	void TestIB();
	void TestStates();
	void TestConstantBuffers();
	void TestRenderTarget();
	void TestTexturesFromFile();
};
