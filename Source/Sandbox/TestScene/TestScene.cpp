#include "TestScene.h"

//Subsystems
#include "../../../Engine/Graphics/GraphicsManager/GraphicsManager.h"

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

	//
	//CBuffer
	//
	XMVECTOR eyeV = XMLoadFloat3(&(XMFLOAT3(0.0f, 0.0f, -5.0f)));;
	XMVECTOR lookV = XMLoadFloat3(&(XMFLOAT3(0.0f, 0.0f, 0.0f)));;
	XMVECTOR upV = XMLoadFloat3(&(XMFLOAT3(0.0f, 1.0f, 0.0f)));
	cbView = XMMatrixLookAtLH(eyeV, lookV, upV);

	float screenW = 960.0f; //TODO: Get actual window dimensions
	float screenH = 540.0f; //TODO: Get actual window dimensions
	cbProj = XMMatrixPerspectiveFovLH(0.25f, (screenH / screenW), 0.1f, 100.0f);
	
	cbWorld = XMMatrixIdentity();

	XMMATRIX wvp = cbWorld * (cbView * cbProj);
	XMFLOAT4X4 wvp4x4;
	XMStoreFloat4x4(&wvp4x4, wvp);

	assert(constantBuffer.InitConstantBuffer(device,
		sizeof(XMFLOAT4X4), &wvp4x4,
		RESOURCE_USAGE_DYNAMIC, RESOURCE_CPU_ACCESS_WRITE_BIT, 
		RESOURCE_BIND_CONSTANT_BUFFER_BIT,
		std::string("TestConstantBuffer_WVP")));
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
	dss.Shutdown();
	rss.Shutdown();

	cbVS.Shutdown();
	cbPS.Shutdown();
	cbVB.Shutdown();
	cbIB.Shutdown();
	constantBuffer.Shutdown();

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
	//Recreate proj matrix
	cbProj = XMMatrixPerspectiveFovLH(0.25f, ((float)newHeight / (float)newWidth), 0.1f, 100.0f);

	//Done
	return true;
}

bool TestScene::OnSceneUpdate(float dt)
{
	static float rot = 0.0f;
	rot += 0.25f * dt;
	XMMATRIX world = XMMatrixRotationY(rot);
	XMMATRIX wvp = world * (cbView * cbProj);

	//Update CBuffer


	//Done
	return true;
}

bool TestScene::OnSceneRender()
{
	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();

	device->VSBindShader(&vs);
	device->PSBindShader(&ps);
	
	device->IASetTopology(PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	device->IASetVertexBuffer(&stateVB, 0);
	device->IASetIndexBuffer(&stateIB, 0);
	device->RSSetState(&rss);
	device->OMSetDepthStencilState(&dss, 0);
	device->DrawIndexed(ib.GetIndexCount(), 0, 0);
	
	//device->IASetVertexBuffer(&indexedVB, 0);
	//device->IASetIndexBuffer(&ib, 0);
	//device->DrawIndexed(ib.GetIndexCount(), 0, 0);

	//device->IASetVertexBuffer(&vb, 0);
	//device->Draw(3, 0);
	
	//Done
	return true;
}
