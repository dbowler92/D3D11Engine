//SceneManager.h
//Created 30/06/17
//Created By Daniel Bowler
//
//Subsystem responsible for scene management. 

#pragma once

//Parent
#include <Core/CoreSubsystem/CoreSubsystem.h>

//Container
#include <list>

//Scene
#include <Gameplay/Scene/Scene.h>

//Debug
#include <Debug/Log/DebugLog.h>

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

	namespace Graphics
	{
		namespace Platform
		{
			class RENDERING_PLATFORM_IMPLEMENTATION_CLASS_NAME(GraphicsManager);
		};
	};
};

namespace EngineAPI
{
	namespace Gameplay
	{
		class SceneManager : public EngineAPI::Core::CoreSubsystem
		{
			//Application should be the only one able to 
			//create and destroy the scene manager
			friend EngineAPI::OS::Platform::OS_PLATFORM_IMPLEMENTATION_CLASS_NAME(Application);

			//Graphics manager will call rendering functions at apprpriate times
			friend EngineAPI::Graphics::Platform::RENDERING_PLATFORM_IMPLEMENTATION_CLASS_NAME(GraphicsManager);

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
			//Init and shutdown
			bool InitSubsystem();
			void ShutdownSubsystem() override;

			//Events
			bool OnResize(uint32_t newWidth, uint32_t newHeight);

			//TEMP: Mouse input should be forwarded to the current scene - 
			//this is until I have a proper input subsytem implemented
			void OnMouseDown(WPARAM btnState, int x, int y);
			void OnMouseUp(WPARAM btnState, int x, int y);
			void OnMouseMove(WPARAM btnState, int x, int y);

			//Update and render loops
			bool OnUpdate(float dt);

			bool OnPreRender();
			bool OnRenderGeometryPass();
			bool OnRenderLightPass();
			bool OnRenderPostProcessPass();
			bool OnRenderDebugPass();
			bool OnRenderUIPass();
			bool OnRenderDebugUIPass();

		private:
			//Singleton
			SceneManager();
			~SceneManager();
			SceneManager(SceneManager& other);
			SceneManager& operator= (SceneManager& other);

			//Called after ShutdownSubsystem -> Cleans up memory
			static void DestroyInstance();
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