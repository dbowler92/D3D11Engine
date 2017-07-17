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
	{ -1.0f,-1.0f,-1.0f, 1.0f, 1.0f },  
	{ -1.0f,-1.0f, 1.0f, 0.0f, 1.0f },
	{ -1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
	{ -1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
	{ -1.0f, 1.0f,-1.0f, 1.0f, 0.0f },
	{ -1.0f,-1.0f,-1.0f, 1.0f, 1.0f },

	{ -1.0f,-1.0f,-1.0f, 0.0f, 1.0f }, 
	{ 1.0f, 1.0f,-1.0f,  1.0f, 0.0f },
	{ 1.0f,-1.0f,-1.0f,  1.0f, 1.0f },
	{ -1.0f,-1.0f,-1.0f, 0.0f, 1.0f },
	{ -1.0f, 1.0f,-1.0f, 0.0f, 0.0f },
	{ 1.0f, 1.0f,-1.0f,  1.0f, 0.0f },

	{ -1.0f,-1.0f,-1.0f, 0.0f, 1.0f }, 
	{ 1.0f,-1.0f,-1.0f,  1.0f, 1.0f },
	{ 1.0f,-1.0f, 1.0f,  1.0f, 0.0f },
	{ -1.0f,-1.0f,-1.0f, 0.0f, 1.0f },
	{ 1.0f,-1.0f, 1.0f,  1.0f, 0.0f },
	{ -1.0f,-1.0f, 1.0f, 0.0f, 0.0f },

	{ -1.0f, 1.0f,-1.0f, 0.0f, 0.0f },  
	{ -1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f,  1.0f, 1.0f },
	{ -1.0f, 1.0f,-1.0f, 0.0f, 0.0f },
	{ 1.0f, 1.0f, 1.0f,  1.0f, 1.0f },
	{ 1.0f, 1.0f,-1.0f,  1.0f, 0.0f },

	{ 1.0f, 1.0f,-1.0f, 0.0f, 0.0f },  
	{ 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
	{ 1.0f,-1.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f,-1.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f,-1.0f,-1.0f, 0.0f, 1.0f },
	{ 1.0f, 1.0f,-1.0f, 0.0f, 0.0f },

	{ -1.0f, 1.0f, 1.0f, 0.0f, 0.0f },  
	{ -1.0f,-1.0f, 1.0f, 0.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f,  1.0f, 0.0f },
	{ -1.0f,-1.0f, 1.0f, 0.0f, 1.0f },
	{ 1.0f,-1.0f, 1.0f,  1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f,  1.0f, 0.0f },
};

bool SponzaScene::OnAddToSceneManagerList()
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage("SponzaScene::OnAddToSceneManagerList()\n");

	//Init last mouse position
	lastMPos.x = 0;
	lastMPos.y = 0;

	//Done
	return true;
}

bool SponzaScene::OnSceneBecomeActive()
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

	//TEMP: Init cube
	cbWorld = XMMatrixTranslation(0.0f, 0.0f, -5.0f);
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


	XMMATRIX wvp = (cbWorld * mainCamera.GetView()) * mainCamera.GetProj();
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

	//Resize camera
	mainCamera.InitCameraPerspectiveProjectionProperties(45.0f, screenW / screenH, 0.1f, 100.0f);

	//Done
	return true;
}

//input subsystem
void SponzaScene::OnMouseDown(WPARAM btnState, int x, int y)
{
	lastMPos.x = x;
	lastMPos.y = y;

	EngineAPI::Debug::DebugLog::PrintInfoMessage(__FUNCTION__);
}

void SponzaScene::OnMouseUp(WPARAM btnState, int x, int y)
{

}

void SponzaScene::OnMouseMove(WPARAM btnState, int x, int y)
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

bool SponzaScene::OnSceneUpdate(float dt)
{
	const float WALK = 7.5f;
	const float STRAFE = 7.5f;

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


	static float rot = 0.0f;
	static float rotX = 0.0f;
	rot += 0.25f * dt;
	rotX += 0.25f * dt;
	//XMMATRIX world = XMMatrixRotationY(rot) * XMMatrixRotationX(rotX);
	XMMATRIX world = XMMatrixIdentity();
	world *= XMMatrixTranslation(0.0f, 0.0f, -5.0f);
	XMMATRIX wvp = (world * mainCamera.GetView()) * mainCamera.GetProj();

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
