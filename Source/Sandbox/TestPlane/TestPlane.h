//TestPlane.h
//Created 24/07/17
//Created By Daniel Bowler
//
//Represents a textured & normal mapped 
//plane

#pragma once

//Parent
#include <Engine/Core/CoreObject/CoreObject.h>

//Includes
#include <Engine/Includes/MathIncludes.h>
#include <Engine/Graphics/VertexBuffer/VertexBuffer.h>
#include <Engine/Graphics/IndexBuffer/IndexBuffer.h>
#include <Engine/Graphics/ConstantBuffer/ConstantBuffer.h>
#include <Engine/Graphics/VertexShader/VertexShader.h>
#include <Engine/Graphics/PixelShader/PixelShader.h>
#include <Engine/Graphics/BlendState/BlendState.h>
#include <Engine/Graphics/DepthStencilState/DepthStencilState.h>
#include <Engine/Graphics/RasterizerState/RasterizerState.h>
#include <Engine/Graphics/Texture2D/Texture2D.h>
#include <Engine/Graphics/ShaderResourceView/ShaderResourceView.h>
#include <Engine/Graphics/SamplerState/SamplerState.h>

class TestPlane : public EngineAPI::Core::CoreObject
{
public:
	TestPlane() {};
	virtual ~TestPlane() {};

	virtual void Shutdown() override;

	//Init the plane
	void InitPlane(EngineAPI::Graphics::GraphicsDevice* device, 
		CXMMATRIX& wd,
		float width, float depth,
		std::string diffuseTexFile,
		std::string normalTexFile,
		float specPower, float specIntensity,
		std::string debugName);

	void Update(float dt);
	void RenderToGBuffer(EngineAPI::Graphics::GraphicsDevice* device);

public:
	//Gets world matrix
	XMMATRIX GetWorld() { return world; };

protected:
	//World matrix
	XMMATRIX world;
	
	//Graphics data
	EngineAPI::Graphics::VertexBuffer planeVB;
	EngineAPI::Graphics::IndexBuffer planeIB;


	EngineAPI::Graphics::VertexShader texNormalMappedVS;
	EngineAPI::Graphics::PixelShader texNormalMappedPS;

	EngineAPI::Graphics::Texture2D diffuseTex;
	EngineAPI::Graphics::ShaderResourceView diffuseTexSRV;

	EngineAPI::Graphics::Texture2D normalTex;
	EngineAPI::Graphics::ShaderResourceView normalTexSRV;

	EngineAPI::Graphics::SamplerState defaultLinearSampler;

	//CBuffers
	EngineAPI::Graphics::ConstantBuffer vsCBPerObject; //World matrix
	EngineAPI::Graphics::ConstantBuffer psCBPerObject; //Material data
};