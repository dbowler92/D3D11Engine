#include "D3D11BufferResource.h"

using namespace EngineAPI::Graphics::Platform;

void D3D11BufferResource::Shutdown()
{
	ReleaseCOM(buffer);

	//Shutdown core object
	__super::Shutdown();
}

bool D3D11BufferResource::InitBuffer(EngineAPI::Graphics::GraphicsDevice* device, 
	bool doInitWitInitialData, ResourceType resourceType,
	ResourceUsage resourceUsage, ResourceCPUAccessFlag cpuAccess, ResourceBindFlag resourceBindingFlag,
	std::string debugName)
{
	//If old buffer, destroy it before recreating
	if (buffer)
	{
		std::string o = std::string(__FUNCTION__) + ": " + "Releasing old buffer: " + GetDebugName();
		EngineAPI::Debug::DebugLog::PrintWarningMessage(o.c_str());
		ReleaseCOM(buffer);
	}

	//Print message saying we are creating a buffer
	std::string o = std::string(__FUNCTION__) + ": " + "Creating Buffer: " + debugName;
	EngineAPI::Debug::DebugLog::PrintInfoMessage(o.c_str());

	//Initial data?
	D3D11_SUBRESOURCE_DATA* initialData = nullptr;
	if (doInitWitInitialData)
		initialData = &bufferInitialData;

	//Create the buffer
	HR_CHECK_ERROR(device->GetD3D11Device()->CreateBuffer(&bufferDesc, initialData, &buffer));
	if (buffer == nullptr)
		return false;

	//Debug name
	SetDebugName(debugName);

	//Init the common resource - stores api agnostic data on the buffer usage
	BaseResource::InitBaseResourceUsageData(resourceType, resourceUsage, cpuAccess, resourceBindingFlag);

	//Done
	return true;
}

void D3D11BufferResource::SetDebugName(std::string s)
{
	//Set engine debug name
	__super::SetDebugName(s);

	//Set resource debug name
	if (buffer)
		buffer->SetPrivateData(WKPDID_D3DDebugObjectName, s.size(), s.c_str());
}

bool D3D11BufferResource::MapResource(EngineAPI::Graphics::GraphicsDevice* device,
	UINT subresourceIndex, ResourceMappingMode mapMode, MappedResourceData* mappedResourceOut)
{
	//Verify if we can map this resource?
	//if (!CanPerformMapOperation(mapMode))
	//	return false;

	ID3D11DeviceContext* context = device->GetD3D11ImmediateContext();

	//Init output
	*mappedResourceOut = {};

	//From api agnostic to D3D11:;
	D3D11_MAP mode = (D3D11_MAP)mapMode;
	D3D11_MAP_FLAG mapFlag = (D3D11_MAP_FLAG)0; //TODO

	//Map the buffer
	D3D11_MAPPED_SUBRESOURCE mappedRes = {};
	HR_CHECK_WARNING(context->Map(buffer, subresourceIndex, mode, mapFlag, &mappedRes));
	if (mappedRes.pData == nullptr)
		return false;

	//Return data
	mappedResourceOut->MappedData = mappedRes.pData;
	mappedResourceOut->RowPitch = mappedRes.RowPitch;
	mappedResourceOut->DepthPitch = mappedRes.DepthPitch;

	//Buffer is mapped
	isResourceCurrentlyMapped = true;

	//Done
	return true;
}

void D3D11BufferResource::UnmapResource(EngineAPI::Graphics::GraphicsDevice* device, 
	UINT subresourceIndex)
{
	//Verify if we can unmap
	if (isResourceCurrentlyMapped)
	{
		//Unmap resource
		device->GetD3D11ImmediateContext()->Unmap(buffer, subresourceIndex);

		//Update internal flag. 
		isResourceCurrentlyMapped = false;
	}
}