#include "D3D11BufferResource.h"

using namespace EngineAPI::Graphics::Platform;

void D3D11BufferResource::Shutdown()
{
	ReleaseCOM(buffer);

	//Shutdown core object
	__super::Shutdown();
}

bool D3D11BufferResource::InitBuffer(EngineAPI::Graphics::GraphicsDevice* device, 
	bool doInitWitInitialData, 
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
	HR(device->GetD3D11Device()->CreateBuffer(&bufferDesc, initialData, &buffer));
	if (buffer == nullptr)
		return false;

	//Debug name
	SetDebugName(debugName);

	//Init the common resource - stores api agnostic data on the buffer usage
	CommonResource::InitCommonResourceUsageData(resourceUsage, cpuAccess, resourceBindingFlag);

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

void D3D11BufferResource::MapResource()
{
	EngineAPI::Debug::DebugLog::PrintInfoMessage(__FUNCTION__);
}

void D3D11BufferResource::UnmapResource()
{

}