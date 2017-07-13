#include "D3D11ViewResource.h"

using namespace EngineAPI::Graphics::Platform;

void D3D11ViewResource::Shutdown()
{
	//No longer need the reference to the baseViewInterface
	//ReleaseCOM(baseViewInterface);
	baseViewInterface = nullptr;

	//Shutdown core object
	__super::Shutdown();
}

void D3D11ViewResource::SetDebugName(std::string s)
{
	//CoreObject debug name
	__super::SetDebugName(s);

	//D3D11 Resource
	if (baseViewInterface)
		baseViewInterface->SetPrivateData(WKPDID_D3DDebugObjectName, s.size(), s.c_str());
}