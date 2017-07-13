#include "CommonResource.h"

using namespace EngineAPI::Graphics;

void CommonResource::Shutdown()
{
	//Shutdown implementation specific common resource
	__super::Shutdown();
}

void CommonResource::InitCommonResourceUsageData(ResourceType type,
	ResourceUsage resourceUsage, ResourceCPUAccessFlag cpuAccess, ResourceBindFlag resourceBindingFlag)
{
	//Cache data
	this->resourceType = type;
	this->resourceUsage = resourceUsage;
	this->resourceCPUAccessFlag = cpuAccess;
	this->resourceBindingFlag = resourceBindingFlag;
}

bool CommonResource::CanPerformMapOperation(ResourceMappingMode mapMode)
{
	bool ret = true;

	//If default || immutable, we can't map
	if (resourceUsage == RESOURCE_USAGE_DEFAULT || resourceUsage == RESOURCE_USAGE_IMMUTABLE)
		ret = false;

	else if (mapMode == RESOURCE_MAP_READ)
	{
		//For resource reads, must be a staging resource
		if (resourceUsage != RESOUCE_USAGE_STAGING)
			ret = false;

		//Must have read access
		if ( (resourceCPUAccessFlag & RESOURCE_CPU_ACCESS_READ_BIT) == false )
			ret = false;
	}

	else if (mapMode == RESOURCE_MAP_WRITE)
	{
		//
		//TODO: Can we write to a staging buffer???
		//

		//Can write to dynamic resources && staging resources (???)
		if ( (resourceUsage != RESOURCE_USAGE_DYNAMIC) && (resourceUsage != RESOUCE_USAGE_STAGING) )
			ret = false;

		//Must have write access
		if ( (resourceCPUAccessFlag & RESOURCE_CPU_ACCESS_WRITE_BIT) == false )
			ret = false;
	}

	else if (mapMode == RESOURCE_MAP_READ_WRITE)
	{
		//Must be a staging buffer for reading
		if (resourceUsage != RESOUCE_USAGE_STAGING)
			ret = false;

		//Must have read and write flags
		if ( (resourceCPUAccessFlag & (RESOURCE_CPU_ACCESS_READ_BIT | RESOURCE_CPU_ACCESS_WRITE_BIT)) == false )
			ret = false;
	}

	else if (mapMode == RESOURCE_MAP_WRITE_DISCARD)
	{
		//
		//TODO: Can we write to a staging buffer?
		//

		//Can write to dynamic resources && staging resources (???)
		if ( (resourceUsage != RESOURCE_USAGE_DYNAMIC) && (resourceUsage != RESOUCE_USAGE_STAGING) )
			ret = false;

		//Must have write access
		if ( (resourceCPUAccessFlag & RESOURCE_CPU_ACCESS_WRITE_BIT) == false )
			ret = false;
	}

	else if (mapMode == RESOURCE_MAP_WRITE_NO_OVERWRITE)
	{
		//Vertex buffers and index buffers only - IE, not constant buffers
		if ( (resourceType != RESOURCE_TYPE_VERTEX_BUFFER) && (resourceType != RESOURCE_TYPE_INDEX_BUFFER) )
			ret = false;

		//
		//TODO: Can we write to staging buffers??
		//

		//Can write to dynamic resources && staging resources (???)
		if ( (resourceUsage != RESOURCE_USAGE_DYNAMIC) && (resourceUsage != RESOUCE_USAGE_STAGING) )
			ret = false;

		//Must have write access
		if ( (resourceCPUAccessFlag & RESOURCE_CPU_ACCESS_WRITE_BIT) == false )
			ret = false;
	}

	//We have an warning - print something useful.
	if (ret == false)
	{
		std::string o = std::string(__FUNCTION__) + ": Failed to map resource - " + GetDebugName();
		EngineAPI::Debug::DebugLog::PrintWarningMessage(o.c_str());
	}

	//Done
	return ret;
}