#include "gepch.h"
#include "Application.h"
#include <GLFW/glfw3.h>

namespace GooE {
	Application::Application() {
		window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application() {
	}

	void Application::Run() {
		while (isRunning) {
			glClearColor(0.7, 0.3, 0.9, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			window->OnUpdate();
		}
	}
}
