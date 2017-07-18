#include "RenderTargetSet.h"

using namespace EngineAPI::Rendering;

void RenderTargetSet::Shutdown()
{
	//Shutdown self
	for (unsigned i = 0; i < renderTargetsCount; i++)
		renderTargets[i].Shutdown();

	renderTargetsCount = 0;
	isInited = false;

	//And super
	__super::Shutdown();
}

void RenderTargetSet::SetDebugName(std::string s)
{
	//Name object
	__super::SetDebugName(s);

	//Name rendering resources
	for (unsigned i = 0; i < renderTargetsCount; i++)
	{
		std::string name = GetDebugName() + "_RT[" + std::to_string(i) + "]";
		renderTargets[i].SetDebugName(name);
	}
}

bool RenderTargetSet::InitRenderTargetSet(uint32_t width, uint32_t height,
	uint32_t renderTargetCount,
	ResourceFormat* textureFormatsArray, bool isUsedAsShaderResource,
	std::string debugName)
{
	assert(renderTargetCount > 1);
	assert(renderTargetCount <= MAX_RENDER_TARGETS_BOUND);
	assert(textureFormatsArray != nullptr);
	assert(isInited == false);
	
	//
	//TODO: Make sure formats are compatable with each other
	//

	//Set core object debug name
	__super::SetDebugName(debugName);

	//Store set count
	this->renderTargetsCount = renderTargetCount;

	//No depth buffer will be created
	this->doesManageDepthBuffer = false;

	//Init each render target
	for (unsigned i = 0; i < renderTargetsCount; i++)
	{
		//Unique name per render target in set
		std::string dbgName = GetDebugName() + "_RT[" + std::to_string(i) + "]";

		//Init - non msaa, no depth texture
		if (!renderTargets[i].InitRenderTarget(width, height, textureFormatsArray[i],
			isUsedAsShaderResource, dbgName))
		{
			//Error
			std::string o = "RenderTargetSet::InitRenderTargetSet() Error: Failed to init the RenderTarget[" + std::to_string(i) + "]  DebugName: " + debugName;
			EngineAPI::Debug::DebugLog::PrintErrorMessage(o.c_str());
			return false;
		}
	}

	//Done
	isInited = true;
	return true;
}

bool RenderTargetSet::InitRenderTargetSetWithADepthStencilTexture(uint32_t width, uint32_t height,
	uint32_t renderTargetCount,
	ResourceFormat* renderTextureFormatsArray, DepthStencilTextureFormat depthStencilFormat,
	bool isUsedAsShaderResource,
	std::string debugName)
{
	assert(renderTargetCount > 1);
	assert(renderTargetCount <= MAX_RENDER_TARGETS_BOUND);
	assert(renderTextureFormatsArray != nullptr);
	assert(isInited == false);

	//Set core object debug name
	__super::SetDebugName(debugName);

	//Store set count
	this->renderTargetsCount = renderTargetCount;

	//A depth buffer will be created
	this->doesManageDepthBuffer = true;

	//Init first render target with a depth stencil buffer
	if (!renderTargets[0].InitRenderTargetWithDepthStencilTexture(width, height,
		renderTextureFormatsArray[0], depthStencilFormat, isUsedAsShaderResource,
		std::string(GetDebugName() + "_RT_WithDepth[" + std::to_string(0) + "]")))
	{
		//Error
		std::string o = "RenderTargetSet::InitRenderTargetSetWithADepthStencilTexture() Error: Failed to init the RenderTarget[0] (With DepthStencil) DebugName: " + debugName;
		EngineAPI::Debug::DebugLog::PrintErrorMessage(o.c_str());
		return false;
	}

	//Init the rest of the render targets
	for (unsigned i = 1; i < renderTargetsCount; i++)
	{
		//Unique name per render target in set
		std::string dbgName = GetDebugName() + "_RT[" + std::to_string(i) + "]";

		//Init - non msaa, no depth texture
		if (!renderTargets[i].InitRenderTarget(width, height, renderTextureFormatsArray[i],
			isUsedAsShaderResource, dbgName))
		{
			//Error
			std::string o = "RenderTargetSet::InitRenderTargetSetWithADepthStencilTexture() Error: Failed to init the RenderTarget[" + std::to_string(i) + "]  DebugName: " + debugName;
			EngineAPI::Debug::DebugLog::PrintErrorMessage(o.c_str());
			return false;
		}
	}

	//Done
	isInited = true;
	return true;
}

bool RenderTargetSet::Resize(uint32_t newWidth, uint32_t newHeight)
{
	//Resize all active render targets
	for (unsigned i = 0; i < renderTargetsCount; i++)
	{
		if (!renderTargets[i].Resize(newWidth, newHeight)) 
		{
			//Error
			std::string o = "RenderTargetSet::Resize() Error: Failed to resize the RenderTarget[" + std::to_string(i) + "]  DebugName: " + GetDebugName();
			EngineAPI::Debug::DebugLog::PrintErrorMessage(o.c_str());
			return false;
		}
	}

	//Done
	return true;
}