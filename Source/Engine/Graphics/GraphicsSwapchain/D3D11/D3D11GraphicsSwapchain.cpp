#include "D3D11GraphicsSwapchain.h"

//Complete forward declarations 
#include <Graphics/GraphicsDevice/GraphicsDevice.h>
#include <OS/OSWindow/OSWindow.h>

using namespace EngineAPI::Graphics::Platform;

bool D3D11GraphicsSwapchain::InitD3D11Swapchain(EngineAPI::Graphics::GraphicsDevice* device, 
	EngineAPI::OS::OSWindow* osWindow, 
	uint32_t backBufferCount, uint32_t msaaSampleCount, 
	bool doesCreateDefaultDepthStencilBuffer)
{
	assert(device != nullptr);

	//Set debug name
	SetDebugName("Swapchain");

	//Cahce init info
	this->backBufferCount = backBufferCount;
	this->msaaSampleCount = msaaSampleCount;
	this->doesManageADepthBuffer = doesCreateDefaultDepthStencilBuffer;

	//Temp:
	assert(backBufferCount == 1);
	
	//Init the IXGISwapchain handle
	if (!InitD3D11SwapchainHandle(device, osWindow))
	{
		EngineAPI::Debug::DebugLog::PrintErrorMessage("D3D11GraphicsSwapchain::InitD3D11Swapchain() Error - Could not init IDXGISwapChain");
		return false;
	}

	//Create (back) buffers
	if (!InitD3D11SwapchainBuffers(device, osWindow))
	{
		EngineAPI::Debug::DebugLog::PrintErrorMessage("D3D11GraphicsSwapchain::InitD3D11Swapchain() Error - Could not size the swapchain");
		return false;
	}

	//Init depth buffer?
	if (doesManageADepthBuffer)
	{
		if (!InitD3D11SwapchainDepthBuffer(device, osWindow, msaaSampleCount))
		{
			EngineAPI::Debug::DebugLog::PrintErrorMessage("D3D11GraphicsSwapchain::InitD3D11SwapchainBuffers() Error - Failed to create depth buffer");
			return false;
		}
	}

	//Done
	return true;
}

void D3D11GraphicsSwapchain::Shutdown()
{
	if (doesManageADepthBuffer)
	{
		swapchainDepthStencilTextureSRV.Shutdown();
		swapchainDepthStencilViewReadWrite.Shutdown();
		swapchainDepthStencilViewReadOnly.Shutdown();
		swpachainDepthStencilTexture.Shutdown();
	}

	swapchainBackbufferRenderTargetView.Shutdown();
	ReleaseCOM(swapchain);

	//Super
	__super::Shutdown();
}

bool D3D11GraphicsSwapchain::OnResize(EngineAPI::Graphics::GraphicsDevice* device,
	EngineAPI::OS::OSWindow* osWindow)
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage(__FUNCTION__);

	//Reinit the swapchain + depth buffers
	if (!InitD3D11SwapchainBuffers(device, osWindow))
	{
		EngineAPI::Debug::DebugLog::PrintErrorMessage("D3D11GraphicsSwapchain::OnResize() Error - Failed to resize swapchain && buffers");
		return false;
	}

	//ReInit depth buffer
	if (doesManageADepthBuffer)
	{
		if (!InitD3D11SwapchainDepthBuffer(device, osWindow, msaaSampleCount))
		{
			EngineAPI::Debug::DebugLog::PrintErrorMessage("D3D11GraphicsSwapchain::OnResize() Error - Failed to resize depth buffer");
			return false;
		}
	}

	return true;
}

bool D3D11GraphicsSwapchain::InitD3D11SwapchainHandle(EngineAPI::Graphics::GraphicsDevice* device,
	EngineAPI::OS::OSWindow* osWindow)
{
	//Set swapchain buffers format
	swapchainBuffersFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	//Cache the initial dimensions from the osWindow.
	swapchainBuffersWidth = osWindow->GetWindowWidth();
	swapchainBuffersHeight = osWindow->GetWindowHeight();

	//Fill out the DXGI_SWAP_CHAIN struct to describe out swap chain
	DXGI_SWAP_CHAIN_DESC sc = {};
	sc.BufferDesc.Width = swapchainBuffersWidth;
	sc.BufferDesc.Height = swapchainBuffersHeight;
	sc.BufferDesc.RefreshRate.Numerator = 0; //DXGI will work out refresh rate for us
	sc.BufferDesc.RefreshRate.Denominator = 0;
	sc.BufferDesc.Format = swapchainBuffersFormat;
	sc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if (msaaSampleCount == 1)
	{
		//No MSAA
		sc.SampleDesc.Count = 1;		//No MSAA
		sc.SampleDesc.Quality = 0;		//No MSAA
	}
	else
	{
		uint32_t maxMSAAQuality = 0;
		device->GetD3D11Device()->CheckMultisampleQualityLevels(swapchainBuffersFormat, msaaSampleCount, &maxMSAAQuality);
		assert(maxMSAAQuality > 0);

		//Enable MSAA
		msaaQualityLevel = maxMSAAQuality - 1;
		sc.SampleDesc.Count = msaaSampleCount;
		sc.SampleDesc.Quality = msaaQualityLevel;
	}

	sc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sc.BufferCount = backBufferCount; //1 == number of backbuffers in SC - this is double buffering. 2 would be tripple buffering. 
	sc.OutputWindow = osWindow->GetAppMainWindowHandle();
	sc.Windowed = true; //Initially
	sc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sc.Flags = 0;

	//In order to correctly create the swap chain, we need to obtain the 
	//IDXGIFactory that was used to create the device. Failing to do so results
	//in an error. 
	IDXGIDevice* dxgiDevice = NULL;
	IDXGIAdapter* dgxiAdapter = NULL;
	IDXGIFactory* dxgiFactory = NULL;

	HR_CHECK_ERROR(device->GetD3D11Device()->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));
	HR_CHECK_ERROR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dgxiAdapter));
	HR_CHECK_ERROR(dgxiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	//Create swapchain
	HR_CHECK_ERROR(dxgiFactory->CreateSwapChain(device->GetD3D11Device(), &sc, &swapchain));

	//Debug name
	if (swapchain)
		EngineAPI::Statics::D3D11ResourceStatics::SetIDXGIResourceDebugName(swapchain, std::string("SwapchainObject"));

	//Clean up afterourselves - ReleaseCOM is a macro found in D3dUtil.
	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dgxiAdapter);
	ReleaseCOM(dxgiFactory);

	//Done
	return true;
}

bool D3D11GraphicsSwapchain::InitD3D11SwapchainBuffers(EngineAPI::Graphics::GraphicsDevice* device,
	EngineAPI::OS::OSWindow* osWindow)
{
	//Destroy old RTV to backbuffer - don't call Shutdown() as that calls
	//the parent shutdown which eventually shutsdown the core object - not an issue
	//now, but maybe later!
	swapchainBackbufferRenderTargetView.ReleaseD3D11RenderTargetView();
	
	//New client size?
	swapchainBuffersWidth = osWindow->GetWindowWidth();
	swapchainBuffersHeight = osWindow->GetWindowHeight();

	//Aspect ratio
	swapchainBuffersAspect = (float)swapchainBuffersWidth / (float)swapchainBuffersHeight;

	//Resize buffers
	HR_CHECK_ERROR(swapchain->ResizeBuffers(1, swapchainBuffersWidth, swapchainBuffersHeight, swapchainBuffersFormat, 0)); //Also resizes the swapchain width/height
	
	//Render target view (RTV) to the back buffer - we will be drawing directly in to the 
	//back buffer. 
	ID3D11Texture2D* backBuffer;
	HR_CHECK_ERROR(swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	
	//Debug name of the backbuffer texture2D object
	if (backBuffer)
		EngineAPI::Statics::D3D11ResourceStatics::SetD3D11ResourceDebugName(backBuffer, std::string("SwapchainBackbufferTexture2D"));

	//Create the RTV via protected method in D3D11RenderTargetView class
	assert(swapchainBackbufferRenderTargetView.InitRenderTargetViewDirectFromD3D11Texture2D(device, backBuffer, std::string("SwapchainBackbufferRenderTargetView")));
	
	//Don't need a reference to the texture - just the view.
	ReleaseCOM(backBuffer); //Don't need a reference to the texture - just the view.

	//Full viewport struct
	swapchainFullViewport.TopLeftX = 0;
	swapchainFullViewport.TopLeftY = 0;
	swapchainFullViewport.Width = static_cast<float>(swapchainBuffersWidth);
	swapchainFullViewport.Height = static_cast<float>(swapchainBuffersHeight);
	swapchainFullViewport.MinDepth = 0.0f; //TODO: Setting!
	swapchainFullViewport.MaxDepth = 1.0f;

	//Done
	return true;
}

bool D3D11GraphicsSwapchain::InitD3D11SwapchainDepthBuffer(EngineAPI::Graphics::GraphicsDevice* device,
	EngineAPI::OS::OSWindow* osWindow, uint32_t msaaCount)
{
	if (doesManageADepthBuffer)
	{
		if (!swpachainDepthStencilTexture.InitDepthStencilTexture2D(device, 
			swapchainBuffersWidth, swapchainBuffersHeight, msaaCount,
			DEPTH_STENCIL_FORMAT_D24_UNORM_S8_UINT, true,
			std::string("SwapchainDepthStencilTexture2D")))
		{
			EngineAPI::Debug::DebugLog::PrintErrorMessage("D3D11GraphicsSwapchain::InitD3D11SwapchainDepthBuffer() Error - Could not create depth texture");
			return false;
		}

		//DSVs
		assert(swapchainDepthStencilViewReadWrite.InitDepthStencilView(device,&swpachainDepthStencilTexture, false, std::string("SwapchainDepthStencilViewReadWrite")));
		assert(swapchainDepthStencilViewReadOnly.InitDepthStencilView(device, &swpachainDepthStencilTexture, true, std::string("SwapchainDepthStencilViewReadOnly")));

		//SRV to the depth buffer
		assert(swapchainDepthStencilTextureSRV.InitShaderResourceViewToDepthStencilTexture2D(device, &swpachainDepthStencilTexture, false, std::string("SwapchainDepthTextureShaderResourceView")));
	}
	else
	{
		//Creating depth texture when we shouldn't be!
		EngineAPI::Debug::DebugLog::PrintErrorMessage("D3D11GraphicsSwapchain::InitD3D11SwapchainDepthBuffer() Error - Trying to create a depth texture when we shouldn't be!");
		return false;
	}

	//Done
	return true;
}

void D3D11GraphicsSwapchain::ClearSwapchainBackbufferRenderTarget(EngineAPI::Graphics::GraphicsDevice* device,
	const float* col)
{
	swapchainBackbufferRenderTargetView.ClearRenderTargetView(device, col);
}

void D3D11GraphicsSwapchain::ClearDepthStencilBuffer(EngineAPI::Graphics::GraphicsDevice* device,
	DepthStencilClearFlag depthStencilBufferClearFlag,
	float depthClearValue, uint8_t stencilClearValue)
{
	//Only clear if we manage the depth stencil buffer. 
	if (doesManageADepthBuffer)
		swapchainDepthStencilViewReadWrite.ClearDepthStencilView(device, depthStencilBufferClearFlag, depthClearValue, stencilClearValue);
}

void D3D11GraphicsSwapchain::BindSwapchainBackbufferAsRenderTarget(EngineAPI::Graphics::GraphicsDevice* device,
	bool shouldBindReadWriteDSV)
{
	//Bind backbuffer and our managed DSV
	if (doesManageADepthBuffer)
	{
		if (shouldBindReadWriteDSV)
			swapchainBackbufferRenderTargetView.BindAsRenderTarget(device, &swapchainDepthStencilViewReadWrite);
		else
			swapchainBackbufferRenderTargetView.BindAsRenderTarget(device, &swapchainDepthStencilViewReadOnly);
	}
	else
		swapchainBackbufferRenderTargetView.BindAsRenderTarget(device, nullptr);
}

void D3D11GraphicsSwapchain::BindSwapchainBackbufferAsRenderTarget(EngineAPI::Graphics::GraphicsDevice* device,
	EngineAPI::Graphics::DepthStencilView* depthStencilView)
{
	//Bind backbuffer && supplied DSV
	swapchainBackbufferRenderTargetView.BindAsRenderTarget(device, depthStencilView);
}

void D3D11GraphicsSwapchain::SetFullResolutionViewport(EngineAPI::Graphics::GraphicsDevice* device)
{
	device->GetD3D11ImmediateContext()->RSSetViewports(1, &swapchainFullViewport);
}

void D3D11GraphicsSwapchain::PresentSwapchainBackbuffer()
{
	swapchain->Present(0, 0);
}