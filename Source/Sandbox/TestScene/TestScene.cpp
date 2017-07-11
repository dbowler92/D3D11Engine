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
	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();

	VertexStreamDescription vbDesc[2];

	vbDesc[0] = VertexStreamDescription::GeneratePerVertexDataStreamDescription("POSITION", 0, RESOURCE_FORMAT_R32G32B32_FLOAT, 0, 0);
	vbDesc[1] = VertexStreamDescription::GeneratePerVertexDataStreamDescription("COLOUR", 0, RESOURCE_FORMAT_R32G32B32_FLOAT, 0, 12);
	
	float someData[1024 / 4];
	void* pData = (void*)someData;

	assert(vb.InitVertexBuffer(device,
		1024, pData,
		RESOURCE_USAGE_IMMUTABLE,
		NULL,
		RESOURCE_BIND_VERTEX_BUFFER_BIT,
		std::string("DebugVB")));
}

void TestScene::TestShaders()
{
	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();

	assert(vs.InitCompiledVertexShaderFromFile(device, 
		SHADER_COMPILED_ASSETS_FOLDER"TestVS.cso", 
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
	//Done
	return true;
}
