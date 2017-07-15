#include "CoreObject.h"

using namespace EngineAPI::Core;

void CoreObject::Shutdown()
{
	if (!_debugNameString.empty())
	{
		std::string o = std::string(__FUNCTION__) + ": Shutting down object: " + _debugNameString;
		EngineAPI::Debug::DebugLog::PrintInfoMessage(o.c_str());
	}
	else
	{
		std::string o = std::string(__FUNCTION__) + ": Shutting down object: <NULL_COREOBJECT_DEBUG_NAME>";
		EngineAPI::Debug::DebugLog::PrintInfoMessage(o.c_str());
	}
}