#pragma once
#include "Core.h"
#include "Window.h"
#include "GooE/Events/ApplicationEvent.h"

namespace GooE {
	class GOOE_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> window;
		bool isRunning = true;
	};

	Application* CreateApplication();

	
}
