#pragma once
#include "Core.h"
#include "Window.h"

namespace GooE {
	class GOOE_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();

	private:
		std::unique_ptr<Window> window;
		bool isRunning = true;
	};

	Application* CreateApplication();
}
