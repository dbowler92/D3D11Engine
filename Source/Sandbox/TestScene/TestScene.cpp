#include "TestScene.h"

//Subsystems
#include <Engine/Graphics/GraphicsManager/GraphicsManager.h>

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

	//RenderTexture2D
	assert(renderTgt.InitRenderTexture2D(device, 512, 512, 1, nullptr,
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
	
	//Done
	return true;
}

bool TestScene::OnSceneRender()
{
	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();

	//Bind render target with default depth buffer for now && clear (The render target - depth
	//buffer should have been cleared by the engine at frame start!)
	assert(gm->GetSwapchain()->DoesSwapchainManageDepthStencilBuffer());
	//device->OMBindRenderTarget( /* Get default depth buffer */ );
	//device->ClearRenderTarget();

	//Topology
	device->IASetTopology(PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Shaders
	device->VSSetShader(&cbVS);
	device->PSSetShader(&cbPS);

	//Vertex and index buffer
	device->IABindVertexBuffer(&cbVB, 0);
	device->IASetIndexBuffer(&cbIB, 0);

	//Shader resource / cbuffers
	device->VSBindConstantBuffer(&constantBuffer, 0);

	//Rendering state
	device->RSSetState(&rss);
	device->OMSetBlendState(&bs);
	device->OMSetDepthStencilState(&dss, 0);

	//Draw command
	device->DrawIndexed(cbIB.GetIndexCount(), 0, 0);

	//Done
	return true;
}
