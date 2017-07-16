//SponzaScene.h
//Created 30/06/17
//Created By Daniel Bowler
//
//Sponza scene to demo the engine features

#pragma once

//Parent class
#include <Engine/Gameplay/Scene/Scene.h>

class SponzaScene : public EngineAPI::Gameplay::Scene
{
public:
	SponzaScene() {};
	~SponzaScene() {};

	//Load and unload
	bool OnAddToSceneManagerList() override;
	bool OnSceneBecomeActive() override;
	bool OnSceneBecomeDeactive() override;
	bool OnRemoveFromSceneManagerList() override;
	bool OnEngineShutdown() override;

	//Engine events
	bool OnResize(uint32_t newWidth, uint32_t newHeight) override;

	//Rendering and updating the scene - NOTE: I will add
	//an update/render loop thats used internally. 
	bool OnSceneUpdate(float dt) override;
	bool OnSceneRender() override;

private:
	
};
