//SceneManager.h
//Created 30/06/17
//Created By Daniel Bowler
//
//Subsystem responsible for scene management. 

#pragma once

//Parent
#include "../../Core/CoreObject/CoreObject.h"

//Container
#include <list>

//Scene
#include "../Scene/Scene.h"

//Debug
#include "../../Debug/Log/DebugLog.h"

//Forward declarations
namespace EngineAPI
{
	namespace OS
	{
		namespace Platform
		{
			class OS_PLATFORM_IMPLEMENTATION_CLASS_NAME(Application);
		};
	};
};

namespace EngineAPI
{
	namespace Gameplay
	{
		class SceneManager : public EngineAPI::Core::CoreObject
		{
			//Application should be the only one able to 
			//create and destroy the scene manager
			friend EngineAPI::OS::Platform::OS_PLATFORM_IMPLEMENTATION_CLASS_NAME(Application);

		public:
			//Singleton class
			static SceneManager* GetInstance();

			//Public interface
			//
			//Adds a new scene to the scene manager
			template<class T>
			void AddNewSceneToManager(std::string sceneTitle)
			{
				T* newScene = new T();
				newScene->SetSceneTitle(sceneTitle);
				scenes.push_back(newScene);
				newScene->OnAddToSceneManagerList();
			}

			//Sets the current active scene + calls the specific load and unload
			//functions
			bool SetCurrentActiveScene(EngineAPI::Gameplay::Scene* scene);
			bool SetCurrentActiveSceneByTitle(std::string title);

			//Getters
			//
			//Returns the current scene title. Empty string if current
			//scene is null
			std::string GetCurrentSceneTitle();

		protected:
			//Called after ShutdownSubsystem -> Cleans up memory
			static void DestroyInstance();
		
			//Init and shutdown
			bool InitSubsystem();
			void ShutdownSubsystem();

			//Update and render loops
			bool OnUpdate(float dt);
			bool OnRender();

		private:
			//Singleton
			SceneManager();
			~SceneManager();
			SceneManager(SceneManager& other);
			SceneManager& operator= (SceneManager& other);

		private:
			static SceneManager* instance;

		private:
			//List of scenes
			std::list<Scene*> scenes;

			//Current scene
			Scene* currentScene = nullptr;
		};
	}
};