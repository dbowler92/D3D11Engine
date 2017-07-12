#include "D3D11Statics.h"

using namespace EngineAPI::Statics;

bool D3D11ResourceStatics::CalculateD3D11UsageFlags(ResourceUsage usage, ResourceCPUAccessFlag cpuAccess,
	D3D11_USAGE& d3d11UsageOut, UINT& d3d11CPUAccessFlagOut)
{
	//Both are simple casts
	d3d11UsageOut = (D3D11_USAGE)usage;
	d3d11CPUAccessFlagOut = cpuAccess;

	//Error?
	if (d3d11CPUAccessFlagOut & D3D11_CPU_ACCESS_WRITE)
	{
		//If we want to write, usage must be dynamic or staging
		if ( (d3d11UsageOut != D3D11_USAGE_DYNAMIC) && (d3d11UsageOut != D3D11_USAGE_IMMUTABLE) )
			return false;
	}

	if (d3d11CPUAccessFlagOut & D3D11_CPU_ACCESS_READ)
	{
		if (d3d11UsageOut != D3D11_USAGE_STAGING)
			return false;
	}

	return true;
}

void D3D11ResourceStatics::SetD3D11ResourceDebugName(ID3D11DeviceChild* child, std::string name)
{
	if (child)
		child->SetPrivateData(WKPDID_D3DDebugObjectName, name.size(), name.c_str());
}