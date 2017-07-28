//ATBManager.h
//Created 28/07/17
//Created By Daniel Bowler
//
//Manages the AntTweakBar library - inc setting it up and
//shutting it down. Later, this may turn in to a deeper
//wrapper

#pragma once

#include <Main/EngineStd.h>
#include <Config/EngineConfig.h>

//ATW header
#include <3rdParty/AntTweakBar/Include/AntTweakBar.h>

//Forward declarations
namespace EngineAPI
{
	namespace Graphics
	{
		class GraphicsDevice;
	};
};

namespace EngineAPI
{
	namespace Debug
	{
		class ATBManager
		{
			MAKE_STATICS_CLASS_ONLY(ATBManager);
		public:
			//Called once the grpahics subsystem is up and running to init
			//the ATW library
			static bool InitATB(EngineAPI::Graphics::GraphicsDevice* device, 
				uint32_t windowWidth, uint32_t windowHeight);

			//Called before shutting down the graphics subsystem, this cleansup
			//the ATW library
			static bool ShutdownATB();

			//Draws the tweak bar(s) - should be called during the debugUI pass / UI pass
			//Likely after scene has done its own rendering
			//
			//This is called by the engine automatically
			static bool RenderATB();

			//Called during a resize event by the engine. Reinits the ATB data required
			static bool OnResizeATB(uint32_t newWidth, uint32_t newHeight);

			//Called by application to handle events
#ifdef ENGINE_CONFIG_PLATFORM_WIN32
			static bool UpdateATB_Win32(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

		public:
			//Do we render + update the ATB bars?
			static void SetATBActiveFlag(bool state) { ATBManager::isATBActive = state; };
			static bool GetATBActiveFlag() { return ATBManager::isATBActive; };

		private:
			//Flag which controlls if the ATB bars should be rendered + updated
			static bool isATBActive;
		};
	};
};
