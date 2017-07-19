#include "SceneManager.h"

//Assert 
#include <assert.h>

using namespace EngineAPI::Gameplay;

SceneManager* SceneManager::instance = nullptr;

SceneManager::SceneManager()
{}

SceneManager::~SceneManager()
{}

SceneManager* SceneManager::GetInstance()
{
	if (!instance)
		instance = new SceneManager();
	return instance;
}

void SceneManager::DestroyInstance()
{
	if (instance)
	{
		delete instance; 
		instance = nullptr;
	}
}

void SceneManager::ShutdownSubsystem()
{
	SetDebugName("SceneManager");

	EngineAPI::Debug::DebugLog::PrintInfoMessage(__FUNCTION__);

	//Deactivate current scene
	if (currentScene)
		assert(currentScene->OnSceneBecomeDeactive());

	//Cleanup scenes
	std::list<Scene*>::iterator it = scenes.begin();
	while (it != scenes.end())
	{
		assert((*it)->OnEngineShutdown());
		delete *it;
		it++;
	}
	scenes.clear();

	//Super
	__super::ShutdownSubsystem();
}

bool SceneManager::InitSubsystem()
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage(__FUNCTION__);

	//Done
	return true;
}

bool SceneManager::OnResize(uint32_t newWidth, uint32_t newHeight)
{
	if (currentScene)
		return currentScene->OnResize(newWidth, newHeight);

	//Done
	return true;
}

void SceneManager::OnMouseDown(WPARAM btnState, int x, int y)
{
	if (currentScene)
		currentScene->OnMouseDown(btnState, x, y);
}

void SceneManager::OnMouseUp(WPARAM btnState, int x, int y)
{
	if (currentScene)
		currentScene->OnMouseUp(btnState, x, y);
}

void SceneManager::OnMouseMove(WPARAM btnState, int x, int y)
{
	if (currentScene)
		currentScene->OnMouseMove(btnState, x, y);
}

bool SceneManager::OnUpdate(float dt)
{
	//Update current scene
	if (currentScene)
		return currentScene->OnSceneUpdate(dt);

	//Done
	return true;
}

bool SceneManager::OnPreRender()
{
	if (currentScene)
		return currentScene->OnScenePreRender();

	return false;

}

bool SceneManager::OnRenderGeometryPass()
{
	if (currentScene)
		return currentScene->OnSceneRenderGeometryPass();

	return false;

}

bool SceneManager::OnRenderLightPass()
{
	if (currentScene)
		return currentScene->OnSceneRenderLightPass();

	return false;

}

bool SceneManager::OnRenderPostProcessPass()
{
	if (currentScene)
		return currentScene->OnSceneRenderPostProcessPass();

	return false;

}

bool SceneManager::OnRenderDebugPass()
{
	if (currentScene)
		return currentScene->OnSceneRenderDebugPass();

	return false;
}

bool SceneManager::OnRenderUIPass()
{
	if (currentScene)
		return currentScene->OnSceneRenderUIPass();

	return false;
}

bool SceneManager::OnRenderDebugUIPass()
{
	if (currentScene)
		return currentScene->OnSceneRenderDebugUIPass();

	return false;
}

bool SceneManager::SetCurrentActiveScene(EngineAPI::Gameplay::Scene* scene)
{
	if (scene == nullptr)
		return false;

	if (currentScene)
		assert(currentScene->OnSceneBecomeDeactive());

	currentScene = scene;
	assert(currentScene->OnSceneBecomeActive());

	//Done
	return true;
}

bool SceneManager::SetCurrentActiveSceneByTitle(std::string title)
{
	//Find scene in list
	std::list<Scene*>::iterator it = scenes.begin();
	while (it != scenes.end())
	{
		std::string sceneTitle = (*it)->GetSceneTitle();
		if (sceneTitle == title)
		{
			if (currentScene)
				assert(currentScene->OnSceneBecomeDeactive());

			currentScene = *it;
			assert(currentScene->OnSceneBecomeActive());

			//Done
			return true;
		}

		//next scene in list
		++it;
	}

	//Failed
	EngineAPI::Debug::DebugLog::PrintErrorMessage("SceneManager::SetCurrentActiveSceneByTitle() Error - Could not find scene");
	return false;
}

std::string SceneManager::GetCurrentSceneTitle()
{
	if (currentScene == nullptr)
		return "";
	
	return currentScene->GetSceneTitle();
}