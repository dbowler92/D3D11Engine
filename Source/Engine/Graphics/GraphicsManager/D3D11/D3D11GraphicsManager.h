//D3D11GraphicsManager.h
//Created 09/07/17
//Created By Daniel Bowler
//
//Graphics subsystem - D3D11 implementation

#pragma once

//Parent
#include "../../Core/CoreObject/CoreObject.h"

//Graphics includes - inc D3D11
#include "../../Includes/GraphicsIncludes.h"

//Debug
#include "../../../Debug/Log/DebugLog.h"

//OS window info - so the graphics API can talk to the OS windowing system.
#include "../../../OS/OSWindow/OSWindow.h"

//classes that this manager manages
#include "../../GraphicsDevice/GraphicsDevice.h"
#include "../../GraphicsSwapchain/GraphicsSwapchain.h"

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
	namespace Graphics
	{
		namespace Platform
		{
			class D3D11GraphicsManager : EngineAPI::Core::CoreObject
			{
				//Only application should be able to create/destroy this
				//subsystem + pass events. 
				friend class EngineAPI::OS::Platform::OS_PLATFORM_IMPLEMENTATION_CLASS_NAME(Application);
				
			public:
				//
				//Public interface
				//
				EngineAPI::Graphics::GraphicsDevice* GetDevice() { return &device; };

			protected:
				D3D11GraphicsManager() {};
				virtual ~D3D11GraphicsManager() = 0 {};

				//Inits && shutdown the subsystem
				bool InitSubsystem(EngineAPI::OS::OSWindow* osWindow); //Contains the initial window size
				void ShutdownSubsystem();

				//Called when the window is resized - Will
				//need to resize render targets. 
				bool OnResize(EngineAPI::OS::OSWindow* osWindow);

				//Called when we are begining rendering & finishing rendering by the 
				//main game loop
				bool OnBeginRender();
				bool OnEndRender();

			protected:
				//D3D11 resources
				EngineAPI::Graphics::GraphicsDevice device;
				EngineAPI::Graphics::GraphicsSwapchain swapchain;

			};
		};
	};
};
