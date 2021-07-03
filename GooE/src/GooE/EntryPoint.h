#pragma once
#include "Application.h"

#ifdef GOOE_PLATFORM_WINDOWS

extern GooE::Application* GooE::CreateApplication();

int main(int argc, char** argv) {
	GooE::Log::Init();
	GOOE_CORE_WARN("Log initialized!");
	GOOE_INFO("Log initialized!");

	auto app = GooE::CreateApplication();
	app->Run();
	delete app;
}
#else
	#error Windows only is supported!
#endif

