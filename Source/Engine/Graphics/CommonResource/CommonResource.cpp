#include "CommonResource.h"

using namespace EngineAPI::Graphics;

void CommonResource::Shutdown()
{
	//Shutdown implementation specific common resource
	__super::Shutdown();
}

void CommonResource::InitCommonResourceUsageData(ResourceUsage resourceUsage, ResourceCPUAccessFlag cpuAccess, ResourceBindFlag resourceBindingFlag)
{
	//Cache data
	this->resourceUsage = resourceUsage;
	this->resourceCPUAccessFlag = resourceCPUAccessFlag;
	this->resourceBindingFlag = resourceBindingFlag;
}