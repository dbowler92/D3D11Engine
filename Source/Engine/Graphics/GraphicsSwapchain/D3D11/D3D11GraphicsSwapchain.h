//D3D11GraphicsSwapchain.h
//Created 09/07/17
//Created By Daniel Bowler
//
//Interface to IDXGISwapChain and, eventually, to ID3D11*View
//to the depth stencil view. 

#pragma once

//Parent
#include "../../Core/CoreObject/CoreObject.h"

//Graphics includes - inc D3D11
#include "../../Includes/GraphicsIncludes.h"

//Debug
#include "../../Debug/Log/DebugLog.h"

//Creates an RTV to the backbuffer / texture
#include "../../RenderTargetView/RenderTargetView.h"

//Manages a depth texture
#include "../../DepthTexture2D/DepthTexture2D.h"
#include "../../DepthStencilView/DepthStencilView.h"

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
			class D3D11GraphicsSwapchain : public EngineAPI::Core::CoreObject
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
				virtual void Shutdown() override;

				//Called by graphics manager during resize event
				bool OnResize(EngineAPI::Graphics::GraphicsDevice* device,
					EngineAPI::OS::OSWindow* osWindow);

			public:
				//Clears the backbuffer RTV
				void ClearSwapchainBackbufferRenderTarget(EngineAPI::Graphics::GraphicsDevice* device,
					const float* col);

				//Clears the depth stencil buffer
				void ClearDepthStencilBuffer(EngineAPI::Graphics::GraphicsDevice* device,
					bool doClearDepth = true, bool doClearStencil = true,
					float depthClearValue = 1.0f, uint8_t stencilClearValue = 0);

				//Binds the swapchain back buffer (and depth buffer if we manage it - pass false for shouldBindReadWriteDSV to
				//bind the read only DSV - this allows a SRV to the depth buffer to be bound for pixel shader reads. Eg: Deferred rendering)
				//to the pipeline for rendering
				void BindSwapchainBackbufferAsRenderTarget(EngineAPI::Graphics::GraphicsDevice* device, 
					bool shouldBindReadWriteDSV = true);

				//Binds the swapchain backbuffer to the pipeline with supplied DSV. Pass null to bind
				//no depth buffer. 
				void BindSwpachainBackbufferAsRenderTarget(EngineAPI::Graphics::GraphicsDevice* device,
					EngineAPI::Graphics::DepthStencilView* depthStencilView);

				//Sets the viewport for full screen rendering - this viewport matches the size of the
				//swapchain render target + depth buffer
				void SetFullResolutionViewport(EngineAPI::Graphics::GraphicsDevice* device);

				//Presents the contents rendered in to the swapchain backbuffer to the monitor && user
				void PresentSwapchainBackbuffer();

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
				EngineAPI::Graphics::RenderTargetView swapchainBackbufferRenderTargetView;

				//Depth texture created alongside the swapchain. TODO: Add way of not creating one
				//of these automatically (not 100% necessary in deferred rendering - you can create one and manage
				//it yourself when initing a GBuffer)
				EngineAPI::Graphics::DepthTexture2D swpachainDepthTexture;
				EngineAPI::Graphics::DepthStencilView swapchainDepthStencilViewReadWrite; //Normal use
				EngineAPI::Graphics::DepthStencilView swapchainDepthStencilViewReadOnly;  //Want to sample from depth buffer but use depth test. 

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
					EngineAPI::OS::OSWindow* osWindow, 
					uint32_t msaaCount);
			};
		};
	};
};
