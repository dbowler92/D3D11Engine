#include "CoreSubsystem.h"

using namespace EngineAPI::Core;

void CoreSubsystem::ShutdownSubsystem()
{
	std::string o = std::string(__FUNCTION__) + ": Shutting down subsystem: " + _subsystemDebugNameString;
	EngineAPI::Debug::DebugLog::PrintInfoMessage(o.c_str());
}