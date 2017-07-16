#include "SponzaScene.h"

//Subsystems
#include <Engine/Graphics/GraphicsManager/GraphicsManager.h>

bool SponzaScene::OnAddToSceneManagerList()
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage("SponzaScene::OnAddToSceneManagerList()\n");

	//Done
	return true;
}

bool SponzaScene::OnSceneBecomeActive()
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage("SponzaScene::OnSceneBecomeActive()\n");
	
	//Done
	return true;
}

bool SponzaScene::OnSceneBecomeDeactive()
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage("SponzaScene::OnSceneBecomeDeactive()\n");
	
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
	//Done
	return true;
}

bool SponzaScene::OnSceneUpdate(float dt)
{
	//Done
	return true;
}

bool SponzaScene::OnSceneRender()
{
	EngineAPI::Graphics::GraphicsManager* gm = EngineAPI::Graphics::GraphicsManager::GetInstance();
	EngineAPI::Graphics::GraphicsDevice* device = gm->GetDevice();
	
	//Done
	return true;
}
