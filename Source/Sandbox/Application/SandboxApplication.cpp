#include "SandboxApplication.h"

//App config
#include <Sandbox/AppConfig/AppConfig.h>

//Scenes
#include <Sandbox/SponzaScene/SponzaScene.h>
#include <Sandbox/DeferredTestScene/DeferredTestScene.h>

SandboxApplication::SandboxApplication()
{}

ECHAR* SandboxApplication::GetGameTitle()
{
	return "SandboxApplication";
};

bool SandboxApplication::InitApplicationPreEngineInit()
{
	//Debug
	EngineAPI::Debug::DebugLog::PrintInfoMessage("SandboxApplication::InitApplicationPreEngineInit()\n");

	return true;
}

bool SandboxApplication::InitApplicationPostEngineInit()
{
	//Debug
	EngineAPI::Debug::DebugLog::PrintInfoMessage("SandboxApplication::InitApplicationPostEngineInit()\n");

	//Inform scene manager of scenes
	EngineAPI::Gameplay::SceneManager::GetInstance()->AddNewSceneToManager<SponzaScene>("Sponza Scene");
	EngineAPI::Gameplay::SceneManager::GetInstance()->AddNewSceneToManager<DeferredTestScene>("Deferred Test Scene");

	//Set the first active scene
	assert(EngineAPI::Gameplay::SceneManager::GetInstance()->SetCurrentActiveSceneByTitle("Deferred Test Scene"));
	//assert(EngineAPI::Gameplay::SceneManager::GetInstance()->SetCurrentActiveSceneByTitle("Sponza Scene"));

	return true;
}

bool SandboxApplication::ShutdownApplication()
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage("SandboxApplication::ShutdownApplication()\n");
	return true;
}
