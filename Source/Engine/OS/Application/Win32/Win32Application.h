//Win32Application.h
//Created 15/14/17
//Created By Daniel Bowler
//
//Win32 implementation of the base application class

#pragma once

//Parent
#include "../../Core/CoreObject/CoreObject.h"
#include "../../Core/CoreSubsystem/CoreSubsystem.h"

#include <Windows.h>

#include "../../Config/EngineConfig.h" //Engine settings
#include "../../Main/EngineStd.h"
#include "../../Debug/Log/DebugLog.h" //Debug logging

#include "../../Utils/GameTimer/GameTimer.h"
#include "../../OSWindow/OSWindow.h"

//Subsystems
#include "../../Graphics/GraphicsManager/GraphicsManager.h"
#include "../../Gameplay/SceneManager/SceneManager.h"

namespace EngineAPI
{
	namespace OS
	{
		namespace Platform
		{
			class Win32Application : public EngineAPI::Core::CoreObject
			{
				HIDE_COPY_ASSIGNMENT(Win32Application)
			public:
				Win32Application();
				virtual ~Win32Application() = 0 {};

				//
				//Virtual functions. Game specific instance of this class should implement these
				//
				virtual ECHAR *GetGameTitle() = 0;

				//Inits your game - called after engine and subsystems have been 
				//inited. Here, you would create your game specific subsystems and
				//assets etc. 
				virtual bool InitApplicationPreEngineInit() = 0;
				virtual bool InitApplicationPostEngineInit() = 0;

				//Called when the engine is shutdown before closing the actual engine. 
				virtual bool ShutdownApplication() = 0;

			public:
				//Engine
				//
				//The global messaging function will forward messages to this function
				LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

				//Init engine & engine subsystems. Will bring up a W32 window. Called before initing the 
				//game 
				bool InitEngine(HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd = NULL,
					int appVersionMajor = 1, int appVersionMinor = 0, int appVersionPatch = 0,
					int screenWidth = 960, int screenHeight = 540);

				//Shutsdown the engine - called after shutting down the 
				bool ShutdownEngine();

				//Game loop
				void EnterGameLoop();

			private:
				//Inits Win32 application
				bool InitWin32App();

				//Inits the engine subsystems - eg: Graphics, physics, etc
				bool InitEngineSubsystems();

			private:
				//Engine events
				//
				//Called when the engine has loaded the systems correctly
				//
				//Can load some static engine assets now. This is called before
				//we initialise the user specific application
				bool EngineDidFinishInitialisation();

				//Called when the window is resized
				bool OnResize();

			private:
				//Outputs FPS
				void CalculateFrameRateStats();

			protected:		
				bool appPaused, minimized, maximized, resizing; //State of app
				Utils::GameTimer mainGameLoopTimer;			    //Calculates frame delta

			protected:
				//Subsystems / managers
				EngineAPI::Graphics::GraphicsManager* graphicsSubsystem;
				EngineAPI::Gameplay::SceneManager* sceneManagerSubsystem;

			protected:
				//Shared data
				EngineAPI::OS::OSWindow osWindow;
				int appVersionMajor, appVersionMinor, appVersionPatch; //Application version

			private:
				uint32_t windowWidth;  //Window width && height -> Updated during WM_SIZE event
				uint32_t windowHeight; //When user stops dragging, we can then resize the render targets
			};
		};
	};
};

//Global application pointer. 
extern EngineAPI::OS::Platform::Win32Application* g_App;