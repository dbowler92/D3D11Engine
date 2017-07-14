#include "CoreObject.h"

using namespace EngineAPI::Core;

void CoreObject::Shutdown()
{
	std::string o = std::string(__FUNCTION__) + ": Shutting down object: " + _debugNameString;
	EngineAPI::Debug::DebugLog::PrintInfoMessage(o.c_str());
}