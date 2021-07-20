#pragma once
#include "Application.h"

#ifdef GOOE_PLATFORM_WINDOWS
	#ifdef DIST
		#include <Windows.h>
	#endif
	extern GooE::Application* GooE::CreateApplication();

	int main(int argc, char** argv)
	{
#ifdef DIST
		FreeConsole();
#else
		GooE::Log::Init();
#endif
		GOOE_PROFILE_BEGIN_SESSION("Startup", "GooEProfile-startup.json");
		auto app = GooE::CreateApplication();
		GOOE_PROFILE_END_SESSION();

		GOOE_PROFILE_BEGIN_SESSION("Runtime", "GooEProfile-runtime.json");
		app->Run();
		GOOE_PROFILE_END_SESSION();

		GOOE_PROFILE_BEGIN_SESSION("Shutdown", "GooEProfile-shutdown.json");
		delete app;
		GOOE_PROFILE_END_SESSION();
	}
#else
	#error Windows only is supported!
#endif
