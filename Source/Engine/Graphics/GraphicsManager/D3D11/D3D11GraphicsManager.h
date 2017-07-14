//D3D11GraphicsManager.h
//Created 09/07/17
//Created By Daniel Bowler
//
//Graphics subsystem - D3D11 implementation

#pragma once

//Parent
#include <Core/CoreSubsystem/CoreSubsystem.h>

//Graphics includes - inc D3D11
#include <Includes/GraphicsIncludes.h>

//Debug
#include <Debug/Log/DebugLog.h>

//OS window info - so the graphics API can talk to the OS windowing system.
#include <OS/OSWindow/OSWindow.h>

//classes that this manager manages
#include <Graphics/GraphicsDevice/GraphicsDevice.h>
#include <Graphics/GraphicsSwapchain/GraphicsSwapchain.h>

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
			class D3D11GraphicsManager : EngineAPI::Core::CoreSubsystem
			{
				//Only application should be able to create/destroy this
				//subsystem + pass events. 
				friend class EngineAPI::OS::Platform::OS_PLATFORM_IMPLEMENTATION_CLASS_NAME(Application);
				
			public:
				//
				//Public interface
				//
				EngineAPI::Graphics::GraphicsDevice* GetDevice() { return &device; };
				EngineAPI::Graphics::GraphicsSwapchain* GetSwapchain() { return &swapchain; };

				//Get the size of the swapchain buffers
				uint32_t GetSwapchainBuffersWidth() { return swapchain.GetSwapchainBuffersWidth(); };
				uint32_t GetSwapchainBuffersHeight() { return swapchain.GetSwapchainBuffersHeight(); };

				//Get the size of the window - this may not be the same size as the swapchain buffers
				//if we are using some form of resolution scale (IE: Render to the backbuffer that is smaller
				//than the OSWindow or if the backbuffer is larger than the window (Super Sampling))
				uint32_t GetWindowWidth() { return windowWidth; };
				uint32_t GetWindowHeight() { return windowHeight; };

			protected:
				D3D11GraphicsManager() {};
				virtual ~D3D11GraphicsManager() = 0 {};

				//Inits && shutdown the subsystem
				bool InitSubsystem(EngineAPI::OS::OSWindow* osWindow); //Contains the initial window size
				void ShutdownSubsystem() override;

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

			protected:
				//Cache the OS window width
				uint32_t windowWidth = 0;
				uint32_t windowHeight = 0;
			};
		};
	};
};
