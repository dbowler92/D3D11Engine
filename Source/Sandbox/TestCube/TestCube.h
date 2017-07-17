//TestCube.h
//Created 17/07/17
//Created By Daniel Bowler
//
//Represents a textured cube

#pragma once

//Parent
#include <Engine/Core/CoreObject/CoreObject.h>

//Includes
#include <Engine/Includes/MathIncludes.h>
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

class TestCube : public EngineAPI::Core::CoreObject
{
public:
	TestCube() {};
	virtual ~TestCube() {};

	virtual void Shutdown() override;

	//Init the cube
	void InitCube(EngineAPI::Graphics::GraphicsDevice* device, 
		CXMMATRIX& cubeWorld,
		std::string diffuseTex, bool isPNG,
		std::string debugName);

	void Update(float dt);
	void Render(EngineAPI::Graphics::GraphicsDevice* device);

public:
	//Gets world matrix
	XMMATRIX GetWorld() { return world; };

protected:
	//World matrix
	XMMATRIX world;

	//Graphics data
	EngineAPI::Graphics::VertexBuffer texCubeVB;

	EngineAPI::Graphics::VertexShader texVS;
	EngineAPI::Graphics::PixelShader texPS;

	EngineAPI::Graphics::Texture2D textureFromFile;
	EngineAPI::Graphics::ShaderResourceView texSRV;
	EngineAPI::Graphics::SamplerState defaultLinearSampler;

	EngineAPI::Graphics::BlendState bs;
	EngineAPI::Graphics::DepthStencilState dss;
	EngineAPI::Graphics::RasterizerState rss;
};