#include "RenderTargetSet.h"

//Depth stencil view
#include <Graphics/DepthStencilView/DepthStencilView.h>

//Graphics manager & the device for binding 
#include <Graphics/GraphicsManager/GraphicsManager.h>
#include <Graphics/GraphicsDevice/GraphicsDevice.h>

using namespace EngineAPI::Rendering;

void RenderTargetSet::Shutdown()
{
	//Shutdown self
	for (unsigned i = 0; i < renderTargetsCount; i++)
		renderTargets[i].Shutdown();

	renderTargetsCount = 0;
	isInited = false;
	doesManageDepthBuffer = false;

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
	//TODO: Make sure formats are compatible with each other
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

	//
	//TODO: Make sure formats are compatable with each other
	//

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
	assert(isInited);
	isInited = false;

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
	isInited = true;
	return true;
}

//
//Clearing
//

void RenderTargetSet::ClearRenderTarget(uint32_t targetIndex, Float32Colour clearColour)
{
	assert(isInited);
	assert(targetIndex < renderTargetsCount);

	renderTargets[targetIndex].ClearRenderTarget(clearColour);
}

void RenderTargetSet::ClearAllRenderTargets(Float32Colour clearColour)
{
	assert(isInited);
	assert(renderTargetsCount >= 1);

	for (unsigned i = 0; i < renderTargetsCount; i++)
		renderTargets[i].ClearRenderTarget(clearColour);
}

void RenderTargetSet::ClearAllRenderTargets(Float32Colour* clearColoursArray)
{
	assert(isInited);
	assert(clearColoursArray != nullptr);
	assert(renderTargetsCount >= 1);

	for (unsigned i = 0; i < renderTargetsCount; i++)
		renderTargets[i].ClearRenderTarget((clearColoursArray[i]));
}

void RenderTargetSet::ClearDepthStencilTexture(DepthStencilClearFlag depthStencilClearFlag,
	float depthClear, UINT8 stencilClear)
{
	assert(isInited);
	assert(doesManageDepthBuffer);
	assert(renderTargetsCount >= 1);
	assert(renderTargets[0].DoesManageADepthStencilTexture());

	//renderTargets[0] will be the one managing the depth buffer
	renderTargets[0].ClearDepthStencilTexture(depthStencilClearFlag, depthClear, stencilClear);
}

//
//Binding
//

void RenderTargetSet::BindRenderTargetAtIndexAsOutput(uint32_t targetIndex)
{
	//Binds a single render target (targetIndex) - null depth
	//
	assert(targetIndex < renderTargetsCount);
	assert(isInited);

	renderTargets[targetIndex].BindRenderTargetOnlyAsOutput();
}

void RenderTargetSet::BindRenderTargetAtIndexAndDepthStencilTextureAsOutput(uint32_t targetIndex, 
	bool doUseReadWriteDepth)
{
	//Binds a single render target (targetIndex) with the managed
	//depth/stencil texture
	//
	assert(targetIndex < renderTargetsCount);
	assert(isInited);
	assert(doesManageDepthBuffer);

	//This render target manages the depth/stencil view
	if (targetIndex == 0)
		renderTargets[0].BindRenderTargetAndDepthStencilTextureAsOutput(doUseReadWriteDepth);
	else
	{
		//Bind another target with this targets set managed DSV
		//
		EngineAPI::Graphics::DepthStencilView* pDepthStencilView = nullptr;
		if (doUseReadWriteDepth)
			pDepthStencilView = renderTargets[0].GetDepthStencilViewReadWrite();
		else
			pDepthStencilView = renderTargets[0].GetDepthStencilViewReadOnly();

		renderTargets[targetIndex].BindRenderTargetWithExternalDepthStencilViewAsOutput(pDepthStencilView);
	}
}

void RenderTargetSet::BindRenderTargetAtIndexWithExternalDepthStencilViewAsOutput(uint32_t targetIndex,
	EngineAPI::Graphics::DepthStencilView* externalDSV)
{
	//Binds a single render target (targetIndex) with an externally
	//managed depth/stencil texture
	//
	assert(targetIndex < renderTargetsCount);
	assert(isInited);
	assert(externalDSV);

	renderTargets[targetIndex].BindRenderTargetWithExternalDepthStencilViewAsOutput(externalDSV);
}

void RenderTargetSet::BindAllRenderTargetsAsOutput()
{
	//Binds all render targets as output - without the depth/stencil
	//texture - null depth
	//
	assert(isInited);

	EngineAPI::Graphics::GraphicsDevice* device = nullptr;
	device = EngineAPI::Graphics::GraphicsManager::GetInstance()->GetDevice();

	static EngineAPI::Rendering::RenderTarget* rtPtrs[MAX_RENDER_TARGETS_BOUND];
	for (unsigned i = 0; i < renderTargetsCount; i++)
		rtPtrs[i] = &renderTargets[i];

	device->OMSetRenderTargets(renderTargetsCount, &rtPtrs[0], nullptr);
}

void RenderTargetSet::BindAllRenderTargetsAndDepthStencilTextureAsOutput(bool doUseReadWriteDepth)
{
	//Binds all render textures and the managed depth/stencil texture
	//
	assert(isInited);
	assert(doesManageDepthBuffer);
	assert(renderTargets[0].DoesManageADepthStencilTexture());

	EngineAPI::Graphics::GraphicsDevice* device = nullptr;
	device = EngineAPI::Graphics::GraphicsManager::GetInstance()->GetDevice();

	EngineAPI::Graphics::DepthStencilView* dsv = nullptr;
	if (doUseReadWriteDepth)
		dsv = renderTargets[0].GetDepthStencilViewReadWrite();
	else
		dsv = renderTargets[0].GetDepthStencilViewReadOnly();

	static EngineAPI::Rendering::RenderTarget* rtPtrs[MAX_RENDER_TARGETS_BOUND];
	for (unsigned i = 0; i < renderTargetsCount; i++)
		rtPtrs[i] = &renderTargets[i];

	device->OMSetRenderTargets(renderTargetsCount, &rtPtrs[0], dsv);

}

void RenderTargetSet::BindAllRenderTargetsAndExternalDepthStencilViewAsOutput(
	EngineAPI::Graphics::DepthStencilView* externalDSV)
{
	//Binds all render textures and an externally managed depth/stencil
	//texture
	assert(isInited);
	assert(externalDSV);

	EngineAPI::Graphics::GraphicsDevice* device = nullptr;
	device = EngineAPI::Graphics::GraphicsManager::GetInstance()->GetDevice();

	static EngineAPI::Rendering::RenderTarget* rtPtrs[MAX_RENDER_TARGETS_BOUND];
	for (unsigned i = 0; i < renderTargetsCount; i++)
		rtPtrs[i] = &renderTargets[i];

	device->OMSetRenderTargets(renderTargetsCount, &rtPtrs[0], externalDSV);
}