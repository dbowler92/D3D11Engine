#include "LABuffer.h"

using namespace EngineAPI::Graphics::DeferredRendering;

void LABuffer::Shutdown()
{
	//Shutdown self
	laBufferRenderTarget.Shutdown();

	//And super
	__super::Shutdown();
}

void LABuffer::SetDebugName(std::string s)
{
	//Core obj name
	__super::SetDebugName(s);

	//Name of the render target set
	laBufferRenderTarget.SetDebugName(GetDebugName());
}

bool LABuffer::InitLABuffer(uint32_t width, uint32_t height,
	std::string debugName)
{
	//Core obj debug name
	__super::SetDebugName(debugName);

	//Init render target
	if (!laBufferRenderTarget.InitRenderTarget(width, height,
		LABUFFER_RENDER_TARGET_FORMAT, true,
		(debugName + "_RT")))
	{
		//Error
		std::string o = "LABuffer::InitLABuffer() Failed to init RenderTarget. Debug name: " + GetDebugName();
		EngineAPI::Debug::DebugLog::PrintErrorMessage(o.c_str());
		return false;
	}

	//Done
	return true;
}

bool LABuffer::OnResize(uint32_t newWidth, uint32_t newHeight)
{
	return laBufferRenderTarget.Resize(newWidth, newHeight);
}

void LABuffer::Clear()
{
	laBufferRenderTarget.ClearRenderTarget(Float32Colour(0.f, 0.f, 0.f, 0.f));
}

void LABuffer::BindLABufferForLightingPass(EngineAPI::Graphics::DepthStencilView* depthGBufferReadOnlyDSV)
{
	laBufferRenderTarget.BindRenderTargetWithExternalDepthStencilViewAsOutput(depthGBufferReadOnlyDSV);
}