#include "D3D11Texture2D.h"

#include <math.h>
//#include <string>

//File loaders
#include <3rdParty/DDSTextureLoader/DDSTextureLoader.h>
#include <3rdParty/LodePNG/lodepng.h>

//SRV for auto mip map generation
#include <Graphics/ShaderResourceView/ShaderResourceView.h>

using namespace EngineAPI::Graphics::Platform;

void D3D11Texture2D::Shutdown()
{
	//Release texture
	ReleaseCOM(texture2DHandle);

	//Shutdown up the chain
	__super::Shutdown();
}

bool D3D11Texture2D::InitTexture2D(EngineAPI::Graphics::GraphicsDevice* device,
	uint32_t textureWidth, uint32_t textureHeight, uint32_t msaaSampleCount,
	uint32_t mipLevels, uint32_t arraySize,
	ResourceMiscFlag miscFlags,
	SubResourceData* resourceInitDataPerSubresource,
	ResourceFormat textureFormat,
	ResourceUsage textureUsage,
	ResourceCPUAccessFlag textureCpuAccess,
	ResourceBindFlag textureBindFlag,
	std::string debugName)
{
	//Destroy old texture
	if (texture2DHandle)
	{
		std::string o = std::string(__FUNCTION__) + ": " + "Releasing old Texture2D: " + GetDebugName();
		EngineAPI::Debug::DebugLog::PrintWarningMessage(o.c_str());
		ReleaseCOM(texture2DHandle);
	}

	//If we want to auto generate mips, we must set the bind flag to include
	//render target + shader resource
	if (miscFlags & RESOURCE_MISC_GENERATE_MIPS)
	{
		textureBindFlag |= RESOURCE_BIND_SHADER_RESOURCE_BIT;
		textureBindFlag |= RESOURCE_BIND_RENDER_TARGET_BIT;
	}

	//Reset D3D11 desc struct
	textureDesc = {};

	//Fill D3D11 Tex desc struct
	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = mipLevels;
	textureDesc.ArraySize = arraySize;
	textureDesc.Format = (DXGI_FORMAT)textureFormat;
	textureDesc.Usage = (D3D11_USAGE)textureUsage;
	textureDesc.CPUAccessFlags = (UINT)textureCpuAccess;
	textureDesc.BindFlags = (UINT)textureBindFlag;
	textureDesc.MiscFlags = (UINT)miscFlags;

	//MSAA
	if (msaaSampleCount == 1)
	{
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
	}
	else
	{
		uint32_t maxMSAAQuality = 0;
		device->GetD3D11Device()->CheckMultisampleQualityLevels((DXGI_FORMAT)textureFormat, msaaSampleCount, &maxMSAAQuality);
		assert(maxMSAAQuality > 0);

		//Do enable msaa (for render targets...)
		uint32_t msaaQualityLevel = maxMSAAQuality - 1;
		textureDesc.SampleDesc.Count = msaaSampleCount;
		textureDesc.SampleDesc.Quality = msaaQualityLevel;
	}

	//Print message saying we are creating a texture
	std::string o = std::string(__FUNCTION__) + ": " + "Creating Texture2D: " + debugName;
	EngineAPI::Debug::DebugLog::PrintInfoMessage(o.c_str());

	//Init texture with initial data?
	uint32_t subresourceStructsCount = mipLevels; //TODO
	std::vector<D3D11_SUBRESOURCE_DATA>textureInitialData(subresourceStructsCount);
	D3D11_SUBRESOURCE_DATA* initialDataDescPtr = nullptr;
	if (resourceInitDataPerSubresource)
	{
		//Fill D3D11 init data struct
		for (int i = 0; i < (int)subresourceStructsCount; i++)
		{
			textureInitialData[i].pSysMem = resourceInitDataPerSubresource[i].pData;
			textureInitialData[i].SysMemPitch = resourceInitDataPerSubresource[i].MemoryRowPitch;
			textureInitialData[i].SysMemSlicePitch = resourceInitDataPerSubresource[i].MemorySlicePitch;
		}
		initialDataDescPtr = textureInitialData.data();
	}

	HR_CHECK_ERROR(device->GetD3D11Device()->CreateTexture2D(&textureDesc, initialDataDescPtr, &texture2DHandle));
	if (texture2DHandle == nullptr)
		return false;

	//Debug name
	SetDebugName(debugName);

	//Init base resource with API agnostic usage data
	BaseResource::InitBaseResourceUsageData(RESOURCE_TYPE_TEXTURE_2D, textureUsage, textureCpuAccess, textureBindFlag);
	
	//Done
	return true;
}

bool D3D11Texture2D::InitTexture2DFromDDSFile(EngineAPI::Graphics::GraphicsDevice* device,
	std::string ddsFilePath, std::string debugName)
{
	assert(!ddsFilePath.empty());

	//Release old texture + warning message
	if (texture2DHandle)
	{
		std::string o = std::string(__FUNCTION__) + ": " + "Releasing old Texture2D: " + GetDebugName();
		EngineAPI::Debug::DebugLog::PrintWarningMessage(o.c_str());
		ReleaseCOM(texture2DHandle);
	}

	//Usage
	ResourceUsage usg = RESOURCE_USAGE_DEFAULT;
	ResourceCPUAccessFlag cpuAccess = 0;
	ResourceBindFlag bindFlag = RESOURCE_BIND_SHADER_RESOURCE_BIT;
	ResourceMiscFlag misc = 0;

	//Message telling us we are loading a Texture2D from file
	std::string o = std::string(__FUNCTION__) + ": " + "Creating Texture2D: " + debugName + ". Loaded from file: " + ddsFilePath;
	EngineAPI::Debug::DebugLog::PrintInfoMessage(o.c_str());
	 
	//Load the texture
	std::wstring ddsFileWS(ddsFilePath.begin(), ddsFilePath.end());
	ID3D11Resource* resource = nullptr;
	DirectX::DDS_ALPHA_MODE alphaMode;

	//HR_CHECK_ERROR(DirectX::CreateDDSTextureFromFile(device->GetD3D11Device(),
	//	ddsFileWS.c_str(), &resource, nullptr, 0, &alphaMode));

	HR_CHECK_ERROR(DirectX::CreateDDSTextureFromFileEx(device->GetD3D11Device(), 
		ddsFileWS.c_str(), 0, (D3D11_USAGE)usg, (UINT)bindFlag, cpuAccess, misc, 
		false, &resource, nullptr, &alphaMode));

	//Check 
	if (resource == nullptr)
		return false;

	//Ensure we actually loaded a Texture2D object
	D3D11_RESOURCE_DIMENSION d3dResourceDimention = {};
	resource->GetType(&d3dResourceDimention);
	if (d3dResourceDimention != D3D11_RESOURCE_DIMENSION_TEXTURE2D)
		return false;

	//Cache texture pointer
	texture2DHandle = (ID3D11Texture2D*)resource;

	//Cache the D3D11_TEXTURE2D_DESC struct
	textureDesc = {};
	texture2DHandle->GetDesc(&textureDesc);

	//Set debug name
	SetDebugName(debugName);
	//__super::SetDebugName(debugName);

	//Init the BaseResource data
	ResourceUsage textureUsage = (ResourceUsage)textureDesc.Usage;
	ResourceCPUAccessFlag textureCPUAccess = (ResourceCPUAccessFlag)textureDesc.CPUAccessFlags;
	ResourceBindFlag textureBinding = (ResourceBindFlag)textureDesc.BindFlags;
	BaseResource::InitBaseResourceUsageData(RESOURCE_TYPE_TEXTURE_2D, 
		textureUsage, textureCPUAccess, textureBinding);
	
	//Done
	return true;
}

bool D3D11Texture2D::InitTexture2DFromPNGFile(EngineAPI::Graphics::GraphicsDevice* device,
	std::string pngFilePath, bool doEnableAutoMipGeneration,
	std::string debugName)
{
	assert(!pngFilePath.empty());

	//Print file path
	std::string o = std::string(__FUNCTION__) + ": " + "LodePNG parsing file: " + pngFilePath + " For Object: " + debugName;
	EngineAPI::Debug::DebugLog::PrintInfoMessage(o.c_str());

	//Use LodePNG to load the file
	std::vector<unsigned char>pngData;
	unsigned int pngFileWidth;
	unsigned int pngFileHeight;
	unsigned error = lodepng::decode(pngData, pngFileWidth, pngFileHeight, pngFilePath, LCT_RGBA, 8); //RGBA8
	if (error != 0)
	{
		std::string o = std::string(__FUNCTION__) + ": " + "Failed to load PNG: " + pngFilePath + " For Object: " + debugName;
		EngineAPI::Debug::DebugLog::PrintErrorMessage(o.c_str());
		pngData.clear();
		return false;
	}

	assert(pngFileWidth > 0);
	assert(pngFileHeight > 0);
	
	//Calculate size in bytes from the beginning of one line of the texture
	//to to the next line
	uint32_t memoryPitch = (pngFileWidth) * 4; //PNG is converted to RGBA8 within pngData

	//Flags && init data:
	uint32_t mipLevels = 1;
	ResourceUsage usg = RESOURCE_USAGE_IMMUTABLE;
	ResourceBindFlag binding = RESOURCE_BIND_SHADER_RESOURCE_BIT;
	ResourceMiscFlag miscFlag = NULL;

	//If we want to enable auto mips generation...
	if (doEnableAutoMipGeneration)
	{
		mipLevels = CalculateFullMipmapChainCount(pngFileWidth, pngFileHeight);
		miscFlag |= RESOURCE_MISC_GENERATE_MIPS;
		binding |= RESOURCE_BIND_RENDER_TARGET_BIT;
		usg = RESOURCE_USAGE_DEFAULT;
	}

	//Initial data - The main texture. NOTE: If the mip levels of the texture
	//we are creating is 1 (Ie: We don't want to support auto mip generation), 
	//then we can pass the parsed texture as the initial data to load in to 
	//GPU memory. If we do support auto generation of mips (ie: mipLevels > 1)
	//then we will pass null here and call UpdateSubresource() after the texture
	//has been inited to load the most detailed mip level with some data - we can
	//then call D3D11ImediateContext->GenerateMips() later.
	SubResourceData texInitData = {};
	texInitData.pData = (void*)pngData.data();
	texInitData.MemoryRowPitch = memoryPitch;
	texInitData.MemorySlicePitch = 0;

	SubResourceData* initialDataDescPtr = nullptr;
	if (mipLevels == 1)
		initialDataDescPtr = &texInitData;

	//Init the texture
	bool ret = InitTexture2D(device,
		pngFileWidth, pngFileHeight, 1,
		mipLevels, 1,
		miscFlag,
		initialDataDescPtr,
		RESOURCE_FORMAT_R8G8B8A8_UNORM, usg, NULL,
		binding,
		debugName);

	assert(ret == true);

	//If we want auto mips generation, the texture data has not been 
	//loaded as of now - go ahead and copy the texture data loaded from the
	//PNG in to the most detailed mip now
	if (mipLevels > 1)
		UpdateSubresourceFull(device, 0, &texInitData);

	//Done
	return ret;
}

void D3D11Texture2D::SetDebugName(std::string s)
{
	//Set engine debug name
	__super::SetDebugName(s);

	//Set resource debug name
	if (texture2DHandle)
		texture2DHandle->SetPrivateData(WKPDID_D3DDebugObjectName, s.size(), s.c_str());
}

bool D3D11Texture2D::MapResource(EngineAPI::Graphics::GraphicsDevice* device,
	UINT subresourceIndex, ResourceMappingMode mapMode, MappedResourceData* mappedResourceOut)
{
	//Verify if we can map this resource?
	//if (!CanPerformMapOperation(mapMode))
	//	return false;

	ID3D11DeviceContext* context = device->GetD3D11ImmediateContext();

	//Init output
	*mappedResourceOut = {};

	//From api agnostic to D3D11:
	D3D11_MAP mode = (D3D11_MAP)mapMode;
	D3D11_MAP_FLAG mapFlag = (D3D11_MAP_FLAG)0; //TODO

	//Map the buffer
	D3D11_MAPPED_SUBRESOURCE mappedRes = {};
	HR_CHECK_WARNING(context->Map(texture2DHandle, subresourceIndex, mode, mapFlag, &mappedRes));
	if (mappedRes.pData == nullptr)
		return false;

	//Return data
	mappedResourceOut->MappedData = mappedRes.pData;
	mappedResourceOut->RowPitch = mappedRes.RowPitch;
	mappedResourceOut->DepthPitch = mappedRes.DepthPitch;

	//Texture is now mapped
	isResourceCurrentlyMapped = true;

	//Done
	return true;
}

void D3D11Texture2D::UnmapResource(EngineAPI::Graphics::GraphicsDevice* device, 
	UINT subresourceIndex)
{
	//Verify if we can unmap
	if (isResourceCurrentlyMapped)
	{
		//Unmap texture
		device->GetD3D11ImmediateContext()->Unmap(texture2DHandle, subresourceIndex);

		//Update internal is mapped flag 
		isResourceCurrentlyMapped = false;
	}
}

void D3D11Texture2D::UpdateSubresourceFull(EngineAPI::Graphics::GraphicsDevice* device,
	UINT subresourceIndex, SubResourceData* subresourceData)
{
	assert(subresourceData);

	//Must not be immutable or dynamic resource
	assert(GetResourceUsage() != RESOURCE_USAGE_IMMUTABLE);
	assert(GetResourceUsage() != RESOURCE_USAGE_DYNAMIC);

	device->GetD3D11ImmediateContext()->UpdateSubresource(texture2DHandle, subresourceIndex, nullptr,
		subresourceData->pData, subresourceData->MemoryRowPitch, subresourceData->MemorySlicePitch);
}

bool D3D11Texture2D::AutoGenerateMipmaps(EngineAPI::Graphics::GraphicsDevice* device,
	EngineAPI::Graphics::ShaderResourceView* srv)
{
	if (srv)
	{
		//TODO: Verify that the SRV references us

		if (DoesSupportAutoMipmapsGeneration())
			return (srv->AutoGenerateMipmaps(device));
		else
			return false;
	}

	return false;
}

//
//Private
//

uint32_t D3D11Texture2D::CalculateFullMipmapChainCount(uint32_t w, uint32_t h)
{
	assert(w >= 1);
	assert(h >= 1);

	return (uint32_t)(1 + floor(log2(max(w, h))));
}