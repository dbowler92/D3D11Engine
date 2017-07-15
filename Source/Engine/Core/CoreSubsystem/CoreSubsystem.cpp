#include "CoreSubsystem.h"

using namespace EngineAPI::Core;

void CoreSubsystem::ShutdownSubsystem()
{
	if (!_subsystemDebugNameString.empty())
	{
		std::string o = std::string(__FUNCTION__) + ": Shutting down subsystem: " + _subsystemDebugNameString;
		EngineAPI::Debug::DebugLog::PrintInfoMessage(o.c_str());
	}
	else
	{
		std::string o = std::string(__FUNCTION__) + ": Shutting down subsystem: <NULL_SUBSYSTEM_DEBUG_NAME>";
		EngineAPI::Debug::DebugLog::PrintInfoMessage(o.c_str());
	}
}