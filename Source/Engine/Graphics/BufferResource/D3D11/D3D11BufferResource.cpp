#include "D3D11BufferResource.h"

using namespace EngineAPI::Graphics::Platform;

bool D3D11BufferResource::InitBuffer(EngineAPI::Graphics::GraphicsDevice* device,
	const D3D11_SUBRESOURCE_DATA* initialData,
	std::string debugName)
{
	//If old buffer, destroy it before recreating
	if (buffer)
	{
		std::string o = std::string(__FUNCTION__) + ": " + "Releasing old buffer: " + GetDebugName();
		EngineAPI::Debug::DebugLog::PrintWarningMessage(o.c_str());
		ReleaseCOM(buffer);
	}

	//Store debug name
	SetDebugName(debugName);

	//Print message saying we are creating a buffer
	std::string o = std::string(__FUNCTION__) + ": " + "Creating Buffer: " + GetDebugName();
	EngineAPI::Debug::DebugLog::PrintInfoMessage(o.c_str());

	//Create the buffer
	HR(device->GetD3D11Device()->CreateBuffer(&bufferDesc, initialData, &buffer));

	//Debug name
	buffer->SetPrivateData(WKPDID_D3DDebugObjectName, GetDebugName().size(), GetDebugName().c_str());
	
	//Done
	return true;
}

void D3D11BufferResource::Shutdown()
{
	ReleaseCOM(buffer);
}