#include "GBuffer.h"

//Graphics manager
#include <Graphics/GraphicsManager/GraphicsManager.h>

using namespace EngineAPI::Graphics::DeferredRendering;

void GBuffer::Shutdown()
{
	//Shutdown self
	gBufferRenderTargetSet.Shutdown();

	//And super
	__super::Shutdown();
}

void GBuffer::SetDebugName(std::string s)
{
	//Core obj name
	__super::SetDebugName(s);

	//Name of the render target set
	gBufferRenderTargetSet.SetDebugName(GetDebugName());
}

bool GBuffer::InitGBuffer(uint32_t width, uint32_t height,
	std::string debugName)
{
	//Core obj debug name
	__super::SetDebugName(debugName);

	if (!gBufferRenderTargetSet.InitRenderTargetSet(width, height,
		GBUFFER_RENDER_TARGETS_COUNT, (ResourceFormat*)&GBUFFER_RENDER_TARGETS_FORMATS[0],
		true, (debugName + "_RTS")))
	{
		//Error
		std::string o = "GBuffer::InitGBuffer() Failed to init RenderTargetSet. Debug name: " + GetDebugName();
		EngineAPI::Debug::DebugLog::PrintErrorMessage(o.c_str());
		return false;
	}

	//Done
	return true;
}

bool GBuffer::OnResize(uint32_t newWidth, uint32_t newHeight)
{
	return gBufferRenderTargetSet.Resize(newWidth, newHeight);
}

void GBuffer::Clear()
{
	gBufferRenderTargetSet.ClearAllRenderTargets(Float32Colour(0.f, 0.f, 0.f, 0.f));
}

void GBuffer::BindGBufferForGeometryPass(EngineAPI::Graphics::DepthStencilView* depthGBufferDSV)
{
	gBufferRenderTargetSet.BindAllRenderTargetsAndExternalDepthStencilViewAsOutput(depthGBufferDSV);
}

void GBuffer::UnbindGBufferAfterGeometryPass()
{
	EngineAPI::Graphics::GraphicsDevice* device =
		EngineAPI::Graphics::GraphicsManager::GetInstance()->GetDevice();

	//Null render targets for each slot + null depth
	device->OMSetRenderTargets(GBUFFER_RENDER_TARGETS_COUNT, nullptr, nullptr);
}

void GBuffer::BindGBufferForLightPass(EngineAPI::Graphics::ShaderResourceView* depthGBufferSRV)
{
	EngineAPI::Graphics::GraphicsDevice* device =
		EngineAPI::Graphics::GraphicsManager::GetInstance()->GetDevice();

	assert(depthGBufferSRV);

	//GBuffer format can be found in HLSLLightPassIncludes.hlsl
	//
	//Texture2D GBuffer_Depth                : register(t0);
	//Texture2D GBuffer_DiffuseSpecIntensity : register(t1);
	//Texture2D GBuffer_PackedNormal         : register(t2);
	//Texture2D GBuffer_SpecPower			 : register(t3);
	//
	static EngineAPI::Graphics::ShaderResourceView* srvs[GBUFFER_RENDER_TARGETS_COUNT + 1]; //Inc depth in [0]
	srvs[0] = depthGBufferSRV;
	for (int i = 0; i < GBUFFER_RENDER_TARGETS_COUNT; i++)
	{
		EngineAPI::Rendering::RenderTarget* rt = gBufferRenderTargetSet.GetRenderTargetAtIndex(i);
		EngineAPI::Graphics::ShaderResourceView* srv = rt->GetRenderTargetShaderResourceView();
		srvs[i + 1] = srv;
	}

	device->PSSetShaderResources(&srvs[0], GBUFFER_RENDER_TARGETS_COUNT + 1, 0);
}

void GBuffer::UnbindGBufferAfterLightPass()
{
	EngineAPI::Graphics::GraphicsDevice* device =
		EngineAPI::Graphics::GraphicsManager::GetInstance()->GetDevice();

	//Unbind SRVs from PS - inc the depth srv
	device->PSSetShaderResources(nullptr, GBUFFER_RENDER_TARGETS_COUNT + 1, 0);
}