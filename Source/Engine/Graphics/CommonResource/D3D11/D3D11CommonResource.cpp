#include "D3D11CommonResource.h"

using namespace EngineAPI::Graphics::Platform;

void D3D11CommonResource::SetD3D11ResourceDebugName(ID3D11DeviceChild* resource)
{
	if (resource)
		resource->SetPrivateData(WKPDID_D3DDebugObjectName, GetDebugName().size(), GetDebugName().c_str());
}