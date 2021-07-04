#include "gepch.h"
#include "Application.h"
#include <GLFW/glfw3.h>

namespace GooE {

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application::Application() {
		window = std::unique_ptr<Window>(Window::Create());
		window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application() {
	}

	void Application::Run() {
		while (isRunning) {
			glClearColor(0.7, 0.3, 0.9, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : layerStack)
				layer->OnUpdate();

			window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

		GOOE_CORE_TRACE("{0}", e);

		for (auto it = layerStack.end(); it != layerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.isHandled) break;
		}
	}

	void Application::PushLayer(Layer* layer) {
		layerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer) {
		layerStack.PushOverlay(layer);
	}

	bool Application::OnWindowClose(WindowCloseEvent &e) {
		isRunning = false;
		return true;
	}
}
