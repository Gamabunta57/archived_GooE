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
		GOOE_CORE_WARN("Log initialized!");
		GOOE_INFO("Log initialized!");
#endif
		auto app = GooE::CreateApplication();
		app->Run();
		delete app;
	}
#else
	#error Windows only is supported!
#endif
