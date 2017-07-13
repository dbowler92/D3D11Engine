#include "Win32OSWindow.h"

using namespace EngineAPI::OS::Platform;

Win32OSWindow::~Win32OSWindow()
{
	Shutdown();
}

void Win32OSWindow::Shutdown()
{
	__super::Shutdown();
}