#include "TestScene.h"

//Subsystems
#include <Engine/Graphics/GraphicsManager/GraphicsManager.h>
#include <Engine/Graphics/GraphicsSwapchain/GraphicsSwapchain.h>

struct Vertex
{
	Vertex() {};
	Vertex(float px, float py, float pz, float cr, float cg, float cb)
	{
		p[0] = px;
		p[1] = py;
		p[2] = pz;

		c[0] = cr;
		c[1] = cg;
		c[2] = cb;
	}

	float p[3];
	float c[3];
};

const UINT CUBE_VERT_COUNT = 8;
const UINT CUBE_INDS_COUNT = 36;

const Vertex CUBE_VERTS[CUBE_VERT_COUNT]
{
	{ -1.0f, 1.0f, -1.0f, 1, 0, 1, },    // vertex 0
	{ 1.0f, 1.0f, -1.0f, 0, 1, 0, },     // vertex ...
	{ -1.0f, -1.0f, -1.0f, 1, 0, 0, },  
	{ 1.0f, -1.0f, -1.0f, 0, 1, 1, },  
	{ -1.0f, 1.0f, 1.0f, 1, 1, 0, },    
	{ 1.0f, 1.0f, 1.0f, 1, 0, 0, },
	{ -1.0f, -1.0f, 1.0f, 0, 1, 0, },
	{ 1.0f, -1.0f, 1.0f, 0, 1, 1 },
};

const UINT16 CUBE_INDICES[CUBE_INDS_COUNT]
{
	0, 1, 2,    // side 1
	2, 1, 3,
	4, 0, 6,    // side 2
	6, 0, 2,
	7, 5, 6,    // side 3
	6, 5, 4,
	3, 1, 7,    // side 4
	7, 1, 5,
	4, 5, 0,    // side 5
	0, 5, 1,
	3, 7, 2,    // side 6
	2, 7, 6,
};

bool TestScene::OnAddToSceneManagerList()
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage("TestScene::OnAddToSceneManagerList()\n");

	//Done
	return true;
}

bool TestScene::OnSceneBecomeActive()
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage("TestScene::OnSceneBecomeActive()\n");
	
	//
	//Test resources here
	//
	TestVB();
	TestShaders();
	TestIB();
	TestStates();
	TestConstantBuffers();
	TestRenderTarget();
	TestTexturesFromFile();

	//Done
	return true;
}

void TestScene::TestVB()
{
	Vertex triVBData[3];
	triVBData[0] = Vertex(1, 1, 0, 1, 0, 0); //TR
	triVBData[1] = Vertex(1, -1, 0, 0, 1, 0); //BR
	triVBData[2] = Vertex(-1, -1, 0, 1, 1, 0); //BL

	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();

	assert(vb.InitVertexBuffer(device,
		sizeof(Vertex), 3, triVBData,
		RESOURCE_USAGE_IMMUTABLE,
		NULL,
		RESOURCE_BIND_VERTEX_BUFFER_BIT,
		std::string("DebugVB")));
}

void TestScene::TestShaders()
{
	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();

	//Vertex input layout 
	VertexInputSignatureElementDescription vbDesc[2];
	vbDesc[0] = VertexInputSignatureElementDescription::PerVertex("POSITION", 0, RESOURCE_FORMAT_R32G32B32_FLOAT, 0, 0);
	vbDesc[1] = VertexInputSignatureElementDescription::PerVertex("COLOUR", 0, RESOURCE_FORMAT_R32G32B32_FLOAT, 0, 12);
	
	assert(vs.InitCompiledVertexShaderFromFile(device, 
		SHADER_COMPILED_ASSETS_FOLDER"TestVS.cso", 
		&vbDesc[0], 2,
		std::string("TestVertexShader")));

	assert(ps.InitCompiledPixelShaderFromFile(device,
		SHADER_COMPILED_ASSETS_FOLDER"TestPS.cso",
		std::string("TestPixelShader")));
}

void TestScene::TestIB()
{
	Vertex quadVBData[4];
	quadVBData[0] = Vertex(1, 1, 0, 1, 0, 0); //TR
	quadVBData[1] = Vertex(1, -1, 0, 0, 1, 0); //BR
	quadVBData[2] = Vertex(-1, -1, 0, 1, 1, 0); //BL
	quadVBData[3] = Vertex(-1, 1, 0, 1, 1, 0); //TL

	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();

	assert(indexedVB.InitVertexBuffer(device,
		sizeof(Vertex), 4, quadVBData,
		RESOURCE_USAGE_IMMUTABLE, NULL, RESOURCE_BIND_VERTEX_BUFFER_BIT,
		std::string("DebugIndexedVB")));

	uint16_t inds[6];
	inds[0] = 0;
	inds[1] = 1;
	inds[2] = 2;

	inds[3] = 3;
	inds[4] = 0;
	inds[5] = 2;

	assert(ib.InitIndexBuffer(device, 
		INDEX_BUFFER_FORMAT_UINT16, 6, inds, 
		RESOURCE_USAGE_IMMUTABLE, NULL, RESOURCE_BIND_INDEX_BUFFER_BIT, 
		std::string("DebugIndexedIB")));
}

void TestScene::TestStates()
{
	//VB and IB
	Vertex quadVBData[4];
	quadVBData[0] = Vertex(1, 1, 0, 1, 0, 0); //TR
	quadVBData[1] = Vertex(1, -1, 0, 0, 1, 0); //BR
	quadVBData[2] = Vertex(-1, -1, 0, 1, 1, 0); //BL
	quadVBData[3] = Vertex(-1, 1, 0, 1, 1, 0); //TL

	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();

	assert(stateVB.InitVertexBuffer(device,
		sizeof(Vertex), 4, quadVBData,
		RESOURCE_USAGE_IMMUTABLE, NULL, RESOURCE_BIND_VERTEX_BUFFER_BIT,
		std::string("StateVB")));

	uint16_t inds[6];
	inds[0] = 0;
	inds[1] = 1;
	inds[2] = 2;

	inds[3] = 3;
	inds[4] = 0;
	inds[5] = 2;

	assert(stateIB.InitIndexBuffer(device,
		INDEX_BUFFER_FORMAT_UINT16, 6, inds,
		RESOURCE_USAGE_IMMUTABLE, NULL, RESOURCE_BIND_INDEX_BUFFER_BIT,
		std::string("StateIB")));

	//
	//States
	//
	DepthStencilPipelineStateDescription dssState = {};
	dssState.StencilTestEnabled = false;
	dssState.StencilReadMask = 0;
	dssState.StencilWriteMask = 0;
	assert(dss.InitDepthStencilState(device, &dssState, std::string("TestDSS")));

	RasterizerPipelineStateDescription rssState = {};
	//rssState.FillMode = POLYGON_FILL_WIREFRAME;
	assert(rss.InitRasterizerState(device, &rssState, std::string("TestRSS")));

	BlendPipelineStateDescription bsDesc = {};
	assert(bs.InitBlendState(device, &bsDesc, std::string("TestBS")));
}

void TestScene::TestConstantBuffers()
{
	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();

	//
	//Shaders
	//

	//Vertex input layout 
	VertexInputSignatureElementDescription vbDesc[2];
	vbDesc[0] = VertexInputSignatureElementDescription::PerVertex("POSITION", 0, RESOURCE_FORMAT_R32G32B32_FLOAT, 0, 0);
	vbDesc[1] = VertexInputSignatureElementDescription::PerVertex("COLOUR", 0, RESOURCE_FORMAT_R32G32B32_FLOAT, 0, 12);

	assert(cbVS.InitCompiledVertexShaderFromFile(device,
		SHADER_COMPILED_ASSETS_FOLDER"TestCBufferVS.cso",
		&vbDesc[0], 2,
		std::string("TestConstantBufferVertexShader")));

	assert(cbPS.InitCompiledPixelShaderFromFile(device,
		SHADER_COMPILED_ASSETS_FOLDER"TestCBufferPS.cso",
		std::string("TestConstantBufferPixelShader")));

	//
	//Cube VB && IB
	//
	assert(cbVB.InitVertexBuffer(device,
		sizeof(Vertex), CUBE_VERT_COUNT, (void*)CUBE_VERTS,
		RESOURCE_USAGE_IMMUTABLE, NULL, RESOURCE_BIND_VERTEX_BUFFER_BIT,
		std::string("Cube_CB_VB")));

	assert(cbIB.InitIndexBuffer(device,
		INDEX_BUFFER_FORMAT_UINT16, CUBE_INDS_COUNT, (void*)CUBE_INDICES,
		RESOURCE_USAGE_IMMUTABLE, NULL, RESOURCE_BIND_INDEX_BUFFER_BIT,
		std::string("Cube_CB_IB")));

	//
	//CBuffer
	//
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
	
	cbWorld = XMMatrixIdentity();

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

void TestScene::TestRenderTarget()
{
	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();

	uint32_t w = (uint32_t)gm->GetWindowWidth();
	uint32_t h = (uint32_t)gm->GetWindowHeight();

	//RenderTexture2D
	assert(renderTgt.InitRenderTexture2D(device, w, h, GRAPHICS_CONFIG_MSAA_SAMPLE_COUNT, nullptr,
		RESOURCE_FORMAT_R8G8B8A8_UNORM,
		RESOURCE_USAGE_DEFAULT,
		(ResourceCPUAccessFlag)0,
		RESOURCE_BIND_RENDER_TARGET_BIT | RESOURCE_BIND_SHADER_RESOURCE_BIT,
		std::string("TestRenderTexture2D")));

	//View
	assert(renderTgtView.InitRenderTargetView(device, &renderTgt, 
		//true, RESOURCE_FORMAT_UNKNOWN,
		false, RESOURCE_FORMAT_R8G8B8A8_UNORM,
		std::string("TestRenderTexture2D_RTV")));

	//Depth
	assert(depthTexture.InitDepthStencilTexture2D(device, w, h, GRAPHICS_CONFIG_MSAA_SAMPLE_COUNT,
		DEPTH_STENCIL_FORMAT_D24_UNORM_S8_UINT, false, std::string("RenderTarget_DepthTexture")));

	//View
	assert(depthTextureView.InitDepthStencilView(device, &depthTexture, false, std::string("RenderTarget_DepthTextureView")));
}

void TestScene::TestTexturesFromFile()
{
	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();

	assert(textureFromFile.InitTexture2DFromDDSFile(device,
		std::string(TEXTURE_ASSETS_FOLDER"/TestTextures/floor.dds"),
		false, std::string("TestTexture2DLoadedFromFile")));
}

bool TestScene::OnSceneBecomeDeactive()
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage("TestScene::OnSceneBecomeDeactive()\n");

	vb.Shutdown();

	vs.Shutdown();
	ps.Shutdown();

	indexedVB.Shutdown();
	ib.Shutdown();

	stateVB.Shutdown();
	stateIB.Shutdown();
	bs.Shutdown();
	dss.Shutdown();
	rss.Shutdown();

	cbVS.Shutdown();
	cbPS.Shutdown();
	cbVB.Shutdown();
	cbIB.Shutdown();
	constantBuffer.Shutdown();

	renderTgt.Shutdown();
	renderTgtView.Shutdown();
	depthTexture.Shutdown();
	depthTextureView.Shutdown();

	textureFromFile.Shutdown();

	//Done
	return true;
}

bool TestScene::OnRemoveFromSceneManagerList()
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage("TestScene::OnRemoveFromSceneManagerList()\n");

	//Done
	return true;
}

bool TestScene::OnEngineShutdown()
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage("TestScene::OnFinalShutdown()\n");

	//Done
	return true;
}

bool TestScene::OnResize(uint32_t newWidth, uint32_t newHeight)
{
	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	float screenW = (float)gm->GetWindowWidth();
	float screenH = (float)gm->GetWindowHeight();

	//Recreate proj matrix
	cbProj = XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45.0f), (screenW / screenH), 0.1f, 100.0f);

	//Resize render to texture
	TestRenderTarget();

	//Done
	return true;
}

bool TestScene::OnSceneUpdate(float dt)
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
	
	//
	//My Math library
	//
	/*
	Vector3D v(2.0f, 2.0f, 2.0f);
	Vector3D v2(8.0f, 8.0f, 8.0f);

	Vector3D v3 = v2 * 2;
	Vector3D v4 = -v3;

	Vector3D v5(2.0f, 3.0f, 5.0f);
	Vector3D v6 = Normalize(v5);
	float v5Len = Length(v5);
	float v5Mag = Magnitude(v5);
	float v6Len = Length(v6);
	Vector3D v7 = -v5;

	Vector3D v8 = v5 * 2;
	
	int x = 100;   
	*/

	//Done
	return true;
}

bool TestScene::OnSceneRender()
{
	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();
	
	//Clear render target & depth
	device->ClearRenderTarget(&renderTgtView, Float32Colour(0.0f, 0.0f, 1.0f, 1.0f));
	device->ClearDepthStencilBuffer(&depthTextureView,
		DEPTH_STENCIL_BUFFER_CLEAR_DEPTH_BIT | DEPTH_STENCIL_BUFFER_CLEAR_STENCIL_BIT, 1.0f, 0);

	//Error check
	assert(renderTgt.GetTexture2D()->GetTextureWidth() == depthTexture.GetTexture2D()->GetTextureWidth());
	assert(renderTgt.GetTexture2D()->GetTextureHeight() == depthTexture.GetTexture2D()->GetTextureHeight());
	assert(renderTgt.GetTexture2D()->GetTextureMSAASampleCount() == depthTexture.GetTexture2D()->GetTextureMSAASampleCount());
	assert(renderTgt.GetTexture2D()->GetTextureMSAAQuality() == depthTexture.GetTexture2D()->GetTextureMSAAQuality());

	//Bind render target + depth.
	device->OMSetRenderTarget(&renderTgtView, &depthTextureView);

	//Topology
	device->IASetTopology(PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Shaders
	device->VSSetShader(&cbVS);
	device->PSSetShader(&cbPS);

	//Vertex and index buffer
	device->IASetVertexBuffer(&cbVB, 0);
	device->IASetIndexBuffer(&cbIB, 0);

	//Shader resource / cbuffers
	device->VSSetConstantBuffer(&constantBuffer, 0);

	//Rendering state
	device->RSSetState(&rss);
	device->OMSetBlendState(&bs);
	device->OMSetDepthStencilState(&dss, 0);

	//Draw command
	device->DrawIndexed(cbIB.GetIndexCount(), 0, 0);

	//Done
	return true;
}
