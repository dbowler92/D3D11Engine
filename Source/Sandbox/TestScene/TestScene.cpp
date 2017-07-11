#include "TestScene.h"

//Subsystems
#include "../../../Engine/Graphics/GraphicsManager/GraphicsManager.h"

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

	//Done
	return true;
}

void TestScene::TestVB()
{
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

	Vertex triVBData[3];
	triVBData[0] = Vertex(1, 1, 0, 1, 0, 0); //TR
	triVBData[1] = Vertex(1, -1, 0, 0, 1, 0); //BR
	triVBData[2] = Vertex(-1, -1, 0, 1, 1, 0); //BL

	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();

	assert(vb.InitVertexBuffer(device,
		sizeof(Vertex) * 3, triVBData,
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

bool TestScene::OnSceneBecomeDeactive()
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage("TestScene::OnSceneBecomeDeactive()\n");

	vb.Shutdown();

	vs.Shutdown();
	ps.Shutdown();

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

bool TestScene::OnSceneUpdate(float dt)
{
	//Done
	return true;
}

bool TestScene::OnSceneRender()
{
	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();

	device->VSBindShader(&vs);
	device->PSBindShader(&ps);

	vb.BindVertexBufferToPipeline(device, 24, 0);
	
	device->IASetTopology(PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	device->Draw(3, 0);
	
	//Done
	return true;
}
