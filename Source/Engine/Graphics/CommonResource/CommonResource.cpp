#include "CommonResource.h"

using namespace EngineAPI::Graphics;

void CommonResource::InitCommonResource(ResourceUsage resourceUsage, ResourceCPUAccessFlag cpuAccess, ResourceBindFlag resourceBindingFlag, std::string debugName)
{
	//Cache data
	this->resourceUsage = resourceUsage;
	this->resourceCPUAccessFlag = resourceCPUAccessFlag;
	this->resourceBindingFlag = resourceBindingFlag;
	
	//TODO: Do something with name
	SetDebugName(debugName);
}