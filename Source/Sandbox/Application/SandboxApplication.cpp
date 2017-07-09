#include "SandboxApplication.h"

//App config
#include "../App Config/AppConfig.h"

//Scenes
#include "../TestScene/TestScene.h"

SandboxApplication::SandboxApplication()
{}

ECHAR* SandboxApplication::GetGameTitle()
{
	return "SandboxApplication";
};

bool SandboxApplication::InitApplication()
{
	//Debug
	EngineAPI::Debug::DebugLog::PrintInfoMessage("SandboxApplication::InitApplication()\n");

	//Inform scene manager of scenes && set first scene
	EngineAPI::Gameplay::SceneManager::GetInstance()->AddNewSceneToManager<TestScene>("Test Scene");
	assert(EngineAPI::Gameplay::SceneManager::GetInstance()->SetCurrentActiveSceneByTitle("Test Scene"));

	return true;
}

bool SandboxApplication::ShutdownApplication()
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage("SandboxApplication::ShutdownApplication()\n");
	return true;
}
