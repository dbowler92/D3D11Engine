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
	
	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();

	//
	//Test resources here
	//
	float someData[1024 / 4];
	void* pData = (void*)someData;

	assert(vb.InitVertexBuffer(device, 
		1024, pData,
		RESOURCE_USAGE_IMMUTABLE,
		NULL,
		RESOURCE_BIND_VERTEX_BUFFER_BIT,
		std::string("DebugVB")));

	//Done
	return true;
}

bool TestScene::OnSceneBecomeDeactive()
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage("TestScene::OnSceneBecomeDeactive()\n");

	vb.Shutdown();

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
