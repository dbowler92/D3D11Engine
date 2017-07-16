#include "D3D11Texture2D.h"

//#include <string>

//File loaders
#include <3rdParty/DDSTextureLoader/DDSTextureLoader.h>
#include <3rdParty/LodePNG/lodepng.h>

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
	void* initialData, uint32_t initialDataMemoryPitch,
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

	//Reset D3D11 structs
	textureDesc = {};
	textureInitialData = {};

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

	//Fill D3D11 init data struct
	textureInitialData.pSysMem = initialData;
	textureInitialData.SysMemPitch = initialDataMemoryPitch;
	textureInitialData.SysMemSlicePitch = 0;

	//Print message saying we are creating a texture
	std::string o = std::string(__FUNCTION__) + ": " + "Creating Texture2D: " + debugName;
	EngineAPI::Debug::DebugLog::PrintInfoMessage(o.c_str());

	//Init texture with initial data?
	D3D11_SUBRESOURCE_DATA* initialDataDesc = nullptr;
	if (initialData)
		initialDataDesc = &textureInitialData;

	HR_CHECK_ERROR(device->GetD3D11Device()->CreateTexture2D(&textureDesc, initialDataDesc, &texture2DHandle));
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

	//Message telling us we are loading a Texture2D from file
	std::string o = std::string(__FUNCTION__) + ": " + "Creating Texture2D: " + debugName + ". Loaded from file: " + ddsFilePath;
	EngineAPI::Debug::DebugLog::PrintInfoMessage(o.c_str());
	 
	//Load the texture
	std::wstring ddsFileWS(ddsFilePath.begin(), ddsFilePath.end());
	ID3D11Resource* resource = nullptr;
	DirectX::DDS_ALPHA_MODE alphaMode;
	HR_CHECK_ERROR(DirectX::CreateDDSTextureFromFile(device->GetD3D11Device(),
		ddsFileWS.c_str(), &resource, nullptr, 0, &alphaMode));
	
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
	textureInitialData = {};
	texture2DHandle->GetDesc(&textureDesc);

	//Set debug name - Just the CoreObject name mind - the DDSLoader 
	//sets the debug name of the resource to its asset path which is
	//rather handy!
	//SetDebugName(debugName);
	__super::SetDebugName(debugName);

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
	std::string pngFilePath, std::string debugName)
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

	//Init the texture
	return InitTexture2D(device,
		pngFileWidth, pngFileHeight, 1,
		1, 1,
		NULL,
		(void*)pngData.data(), memoryPitch,
		RESOURCE_FORMAT_R8G8B8A8_UNORM, RESOURCE_USAGE_DEFAULT, NULL,
		RESOURCE_BIND_SHADER_RESOURCE_BIT, 
		debugName);
}

bool D3D11Texture2D::Internal_InitTexture2D(EngineAPI::Graphics::GraphicsDevice* device,
	bool doInitWitInitialData, ResourceType resourceType,
	ResourceUsage resourceUsage, ResourceCPUAccessFlag cpuAccess, ResourceBindFlag resourceBindingFlag,
	std::string debugName)
{
	//Destroy old texture
	if (texture2DHandle)
	{
		std::string o = std::string(__FUNCTION__) + ": " + "Releasing old Texture2D: " + GetDebugName();
		EngineAPI::Debug::DebugLog::PrintWarningMessage(o.c_str());
		ReleaseCOM(texture2DHandle);
	}

	//Print message saying we are creating a texture
	std::string o = std::string(__FUNCTION__) + ": " + "Creating Texture2D: " + debugName;
	EngineAPI::Debug::DebugLog::PrintInfoMessage(o.c_str());

	//Init texture with initial data?
	D3D11_SUBRESOURCE_DATA* initialData = nullptr;
	if (doInitWitInitialData)
		initialData = &textureInitialData;

	HR_CHECK_ERROR(device->GetD3D11Device()->CreateTexture2D(&textureDesc, initialData, &texture2DHandle));
	if (texture2DHandle == nullptr)
		return false;

	//Debug name
	SetDebugName(debugName);

	//Init base resource with API agnostic usage data
	BaseResource::InitBaseResourceUsageData(resourceType, resourceUsage, cpuAccess, resourceBindingFlag);

	//Done
	return true;
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