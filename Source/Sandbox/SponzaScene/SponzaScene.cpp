#include "SponzaScene.h"

//Subsystems
#include <Engine/Graphics/GraphicsManager/GraphicsManager.h>

struct TexVertex
{
	TexVertex() {};
	TexVertex(float px, float py, float pz, float tx, float ty)
	{
		p[0] = px;
		p[1] = py;
		p[2] = pz;

		t[0] = tx;
		t[1] = ty;
	}

	float p[3];
	float t[2];
};

const UINT CUBE_NON_INDEXED_VERTS_COUNT = 36;
const TexVertex CUBE_TEX_VERTS[36] =
{
	{ -1.0f,-1.0f,-1.0f, 1.0f, 1.0f },  // -X side
	{ -1.0f,-1.0f, 1.0f, 0.0f, 1.0f },
	{ -1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
	{ -1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
	{ -1.0f, 1.0f,-1.0f, 1.0f, 0.0f },
	{ -1.0f,-1.0f,-1.0f, 1.0f, 1.0f },

	{ -1.0f,-1.0f,-1.0f, 0.0f, 1.0f },  // -Z side
	{ 1.0f, 1.0f,-1.0f,  1.0f, 0.0f },
	{ 1.0f,-1.0f,-1.0f,  1.0f, 1.0f },
	{ -1.0f,-1.0f,-1.0f, 0.0f, 1.0f },
	{ -1.0f, 1.0f,-1.0f, 0.0f, 0.0f },
	{ 1.0f, 1.0f,-1.0f,  1.0f, 0.0f },

	{ -1.0f,-1.0f,-1.0f, 0.0f, 1.0f }, // -Y
	{ 1.0f,-1.0f,-1.0f,  0.0f, 0.0f },
	{ 1.0f,-1.0f, 1.0f,  1.0f, 0.0f },
	{ -1.0f,-1.0f,-1.0f, 0.0f, 1.0f },
	{ 1.0f,-1.0f, 1.0f,  1.0f, 0.0f },
	{ -1.0f,-1.0f, 1.0f, 1.0f, 1.0f },

	{ -1.0f, 1.0f,-1.0f, 0.0f, 1.0f },  // +Y side
	{ -1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
	{ 1.0f, 1.0f, 1.0f,  1.0f, 0.0f },
	{ -1.0f, 1.0f,-1.0f, 0.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f,  1.0f, 0.0f },
	{ 1.0f, 1.0f,-1.0f,  1.0f, 1.0f },

	{ 1.0f, 1.0f,-1.0f, 0.0f, 1.0f },  // +X side
	{ 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
	{ 1.0f,-1.0f, 1.0f, 1.0f, 0.0f },
	{ 1.0f,-1.0f, 1.0f, 1.0f, 0.0f },
	{ 1.0f,-1.0f,-1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f,-1.0f, 0.0f, 1.0f },

	{ -1.0f, 1.0f, 1.0f, 0.0f, 1.0f },  // +Z side
	{ -1.0f,-1.0f, 1.0f, 0.0f, 0.0f },
	{ 1.0f, 1.0f, 1.0f,  1.0f, 1.0f },
	{ -1.0f,-1.0f, 1.0f, 0.0f, 0.0f },
	{ 1.0f,-1.0f, 1.0f,  1.0f, 0.0f },
	{ 1.0f, 1.0f, 1.0f,  1.0f, 1.0f },
};

bool SponzaScene::OnAddToSceneManagerList()
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage("SponzaScene::OnAddToSceneManagerList()\n");

	//Done
	return true;
}

bool SponzaScene::OnSceneBecomeActive()
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage("SponzaScene::OnSceneBecomeActive()\n");
	
	//Init camera
	mainCamera.Init();

	//TEMP: Init cube
	InitCubeVB();
	InitShaders();
	InitTextureAndViewAndSampler();
	InitPipelineStates();
	InitCBuffer();

	//Done
	return true;
}

void SponzaScene::InitCubeVB()
{
	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();

	assert(texCubeVB.InitVertexBuffer(device,
		sizeof(TexVertex), CUBE_NON_INDEXED_VERTS_COUNT, (void*)CUBE_TEX_VERTS,
		RESOURCE_USAGE_IMMUTABLE, NULL, RESOURCE_BIND_VERTEX_BUFFER_BIT,
		std::string("Cube_Textured_VB")));
}

void SponzaScene::InitShaders()
{
	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();

	VertexInputSignatureElementDescription vbDesc[2];
	vbDesc[0] = VertexInputSignatureElementDescription::PerVertex("POSITION", 0, RESOURCE_FORMAT_R32G32B32_FLOAT, 0, 0);
	vbDesc[1] = VertexInputSignatureElementDescription::PerVertex("TEXCOORD", 0, RESOURCE_FORMAT_R32G32_FLOAT, 0, 12);

	assert(texVS.InitCompiledVertexShaderFromFile(device,
		SHADER_COMPILED_ASSETS_FOLDER"TestTexturedVS.cso",
		&vbDesc[0], 2,
		std::string("TexturedCubeVS")));

	assert(texPS.InitCompiledPixelShaderFromFile(device,
		SHADER_COMPILED_ASSETS_FOLDER"TestTexturedPS.cso",
		std::string("TexturedCubePS")));
}

void SponzaScene::InitTextureAndViewAndSampler()
{
	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();

	assert(textureFromFile.InitTexture2DFromPNGFile(device,
		std::string(ASSETS_FOLDER"Sponza/textures/sponza_curtain_blue_diff.png"), true,
		std::string("TestTexture2DLoadedFromFile_PNG")));

	assert(texSRV.InitShaderResourceViewToTexture2D(device,
		&textureFromFile, true, RESOURCE_FORMAT_R8G8B8A8_UNORM, false,
		std::string("DDSTextureFromFile_SRV")));

	SamplerStateDescription linearSamplerDesc = {};
	assert(defaultLinearSampler.InitSamplerState(device, &linearSamplerDesc,
		std::string("DefaultLinearSamplerState")));

	//Generate mips?
	if (textureFromFile.DoesSupportAutoMipmapsGeneration())
		assert(textureFromFile.AutoGenerateMipmaps(device, &texSRV));
}

void SponzaScene::InitPipelineStates()
{
	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();

	DepthStencilPipelineStateDescription dssState = {};
	assert(dss.InitDepthStencilState(device, &dssState, std::string("TestDSS")));

	RasterizerPipelineStateDescription rssState = {};
	assert(rss.InitRasterizerState(device, &rssState, std::string("TestRSS")));

	BlendPipelineStateDescription bsDesc = {};
	assert(bs.InitBlendState(device, &bsDesc, std::string("TestBS")));
}

void SponzaScene::InitCBuffer()
{
	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();

	cbWorld = XMMatrixTranslation(0.0f, 0.0f, -5.0f);
	
	XMFLOAT3 eye = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 look = XMFLOAT3(0.0f, 0.0f, -1.0f);
	XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);

	XMVECTOR eyeV = XMLoadFloat3(&eye);
	XMVECTOR lookV = XMLoadFloat3(&look);
	XMVECTOR upV = XMLoadFloat3(&up);

	cbView = XMMatrixLookAtLH(eyeV, lookV, upV);

	float screenW = (float)gm->GetWindowWidth();
	float screenH = (float)gm->GetWindowHeight();
	cbProj = XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45.0f), (screenW / screenH), 0.1f, 100.0f);

	XMMATRIX wvp = (cbWorld * cbView) * cbProj;
	wvp = XMMatrixTranspose(wvp);

	XMFLOAT4X4 wvp4x4;
	XMStoreFloat4x4(&wvp4x4, wvp);

	assert(constantBuffer.InitConstantBuffer(device,
		sizeof(XMFLOAT4X4), &wvp4x4,
		RESOURCE_USAGE_DYNAMIC, RESOURCE_CPU_ACCESS_WRITE_BIT,
		RESOURCE_BIND_CONSTANT_BUFFER_BIT,
		std::string("TestConstantBuffer_WVP")));
}

bool SponzaScene::OnSceneBecomeDeactive()
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage("SponzaScene::OnSceneBecomeDeactive()\n");
	
	//Shutdown
	mainCamera.Shutdown();

	//TEMP: Shutdown cube
	texCubeVB.Shutdown();
	texVS.Shutdown();
	texPS.Shutdown();
	textureFromFile.Shutdown();
	texSRV.Shutdown();
	defaultLinearSampler.Shutdown();
	bs.Shutdown();
	dss.Shutdown();
	rss.Shutdown();
	constantBuffer.Shutdown();

	//Done
	return true;
}

bool SponzaScene::OnRemoveFromSceneManagerList()
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage("SponzaScene::OnRemoveFromSceneManagerList()\n");

	//Done
	return true;
}

bool SponzaScene::OnEngineShutdown()
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage("SponzaScene::OnFinalShutdown()\n");

	//Done
	return true;
}

bool SponzaScene::OnResize(uint32_t newWidth, uint32_t newHeight)
{
	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	float screenW = (float)gm->GetWindowWidth();
	float screenH = (float)gm->GetWindowHeight();

	//Recreate proj matrix
	cbProj = XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45.0f), (screenW / screenH), 0.1f, 100.0f);

	//Resize camera

	//Done
	return true;
}

bool SponzaScene::OnSceneUpdate(float dt)
{
	static float rot = 0.0f;
	static float rotX = 0.0f;
	rot += 0.25f * dt;
	rotX += 0.25f * dt;
	XMMATRIX world = XMMatrixRotationY(rot) * XMMatrixRotationX(rotX);
	//world = XMMatrixIdentity();
	world *= XMMatrixTranslation(0.0f, 0.0f, -5.0f);
	XMMATRIX wvp = (world * cbView) * cbProj;

	wvp = XMMatrixTranspose(wvp);

	XMFLOAT4X4 wvp4x4;
	XMStoreFloat4x4(&wvp4x4, wvp);

	//Update CBuffer
	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();

	MappedResourceData mrd;
	//if (device->MapBufferResource(&constantBuffer, 0, RESOURCE_MAP_WRITE_DISCARD, &mrd))
	//	memcpy(mrd.MappedData, (void*)&wvp4x4, constantBuffer.GetConstantBufferSizeBytes());
	//device->UnmapBufferResource(&constantBuffer);

	bool didMap = constantBuffer.MapResource(device, 0, RESOURCE_MAP_WRITE_DISCARD, &mrd);
	if (didMap)
		memcpy(mrd.MappedData, (void*)&wvp4x4, constantBuffer.GetConstantBufferSizeBytes());
	constantBuffer.UnmapResource(device, 0);

	//Update camera

	//Update CBuffer

	//Done
	return true;
}

bool SponzaScene::OnSceneRender()
{
	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();
	
	//Topology
	device->IASetTopology(PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Shaders
	device->VSSetShader(&texVS);
	device->PSSetShader(&texPS);

	//Vertex (and index) buffer
	device->IASetVertexBuffer(&texCubeVB, 0);

	//Shader resource / cbuffers
	device->VSSetConstantBuffer(&constantBuffer, 0);
	device->PSSetShaderResource(&texSRV, 0);

	//Rendering state
	device->RSSetState(&rss);
	device->OMSetBlendState(&bs);
	device->OMSetDepthStencilState(&dss, 0);

	//Sampler state object
	device->PSSetSamplerState(&defaultLinearSampler, 0);

	//Draw command
	device->Draw(texCubeVB.GetElementsCount(), 0);

	//Done
	return true;
}
