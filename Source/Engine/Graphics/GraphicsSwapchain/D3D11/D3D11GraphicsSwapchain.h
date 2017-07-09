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

namespace EngineAPI
{
	namespace Graphics
	{
		class GraphicsDevice;
	}
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

				//Inits && shutdown the D3D11 swapchain
				bool InitD3D11Swapchain(EngineAPI::Graphics::GraphicsDevice* device);
				void ShutdownD3D11Swapchain();

			protected:
				//Swapchain 
				IDXGISwapChain* swapChain = nullptr;
			};
		};
	};
};
