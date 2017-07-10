//D3D11GraphicsSwapchain.h
//Created 09/07/17
//Created By Daniel Bowler
//
//Interface to IDXGISwapChain and, eventually, to ID3D11*View
//to the depth stencil view. 

#pragma once

//Graphics includes - inc D3D11
#include "../../Includes/GraphicsIncludes.h"

//Debug
#include "../../Debug/Log/DebugLog.h"

//Manages a depth texture
#include "../../Rendering/DepthTexture/DepthTexture.h"

namespace EngineAPI
{
	namespace Graphics
	{
		class GraphicsDevice;
	};

	namespace OS
	{
		class OSWindow;
	};
};

namespace EngineAPI
{
	namespace Graphics
	{
		namespace Platform
		{
			class D3D11GraphicsSwapchain
			{
			public:
				D3D11GraphicsSwapchain() {};
				virtual ~D3D11GraphicsSwapchain() = 0 {};

				//Inits the swapchain and optional default depth stencil buffer
				bool InitD3D11Swapchain(EngineAPI::Graphics::GraphicsDevice* device, 
					EngineAPI::OS::OSWindow* osWindow, 
					uint32_t backBufferCount = 1, 
					uint32_t msaaSampleCount = 1, 
					bool doesCreateDefaultDepthStencilBuffer = true);

				//Shutsdown the swapchain and resources we create inc the depth
				//buffer if we created one.
				void ShutdownD3D11Swapchain();

				//Called by graphics manager during resize event
				bool OnResize(EngineAPI::Graphics::GraphicsDevice* device,
					EngineAPI::OS::OSWindow* osWindow);

			protected:
				//Swapchain 
				IDXGISwapChain* swapchain = nullptr;

				//Swapchain (front + back) buffer data
				DXGI_FORMAT swapchainBuffersFormat;
				unsigned swapchainBuffersWidth;
				unsigned swapchainBuffersHeight;
				float swapchainBuffersAspect;  //Width / Height

				uint32_t backBufferCount; //Doesn't include the front buffer => 1 == double buffering
				bool doesManageADepthBuffer;

				uint32_t msaaSampleCount;
				uint32_t msaaQualityLevel;

				//D3D11 viewport struct for full screen
				D3D11_VIEWPORT swapchainFullViewport;

				//Render target view to the backbuffer - used to bind the backbuffer
				//for rendering in to
				ID3D11RenderTargetView* backBufferRenderTargetView = nullptr; //TODO: Abstract resource views!

				//Depth texture created alongside the swapchain. TODO: Add way of not creating one
				//of these automatically (not 100% necessary in deferred rendering - you can create one and manage
				//it yourself when initing a GBuffer)
				EngineAPI::Rendering::DepthTexture swpachainDepthTexture;

			protected:
				//Internal init functions
				//
				//Inits the IDXGISwapChain
				bool InitD3D11SwapchainHandle(EngineAPI::Graphics::GraphicsDevice* device,
					EngineAPI::OS::OSWindow* osWindow);

				//Resizes the swapchain - also called at init time to init the
				//swpachain buffers to the correct size.
				//
				//Also handles creating && resizing the depth(stencil) buffer by calling ResizeDepthBuffer() within
				//this function
				bool InitD3D11SwapchainBuffers(EngineAPI::Graphics::GraphicsDevice* device,
					EngineAPI::OS::OSWindow* osWindow);

				//Manages creation and resizing of the depth (stencil) buffer
				bool InitD3D11SwapchainDepthBuffer(EngineAPI::Graphics::GraphicsDevice* device,
					EngineAPI::OS::OSWindow* osWindow);
			};
		};
	};
};
