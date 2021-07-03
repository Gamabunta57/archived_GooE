#pragma once
#include "Application.h"
#include <stdio.h>

#ifdef GOOE_PLATFORM_WINDOWS

extern GooE::Application* GooE::CreateApplication();

int main(int argc, char** argv) {
	printf("GooE Engine Start");
	auto app = GooE::CreateApplication();
	app->Run();
	delete app;
}
#else
	#error Windows only is supported!
#endif

