#include "gepch.h"

#include <GLFW/glfw3.h>

#include <GooE/Renderer/Renderer.h>
#include <GooE/Renderer/RenderCommand.h>

#include "Application.h"
#include "Input.h"

namespace GooE {
	Application* Application::instance = nullptr;

	Application::Application() {
		GOOE_CORE_ASSERT(!instance, "Application already exists!");
		instance = this;

		window = std::unique_ptr<Window>(Window::Create());
		window->SetEventCallback(GOOE_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		imguiLayer = new ImGuiLayer();
		PushOverlay(imguiLayer);		
	}

	Application::~Application() {
	}

	void Application::Run() {
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		while (isRunning) {

			float time = (float) glfwGetTime(); //Should be something like Platfrom::GetTime() instead
			Timestep timestep = time - lastFrameTime;
			lastFrameTime = time;

			if (!minimized) {
				for (Layer* layer : layerStack)
					layer->OnUpdate(timestep);
			}

			imguiLayer->Begin();
			for (Layer* layer : layerStack)
				layer->OnImGuiRender();
			imguiLayer->End();

			window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(GOOE_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(GOOE_BIND_EVENT_FN(Application::OnWindowResize));

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

	bool Application::OnWindowResize(WindowResizeEvent& e) {
		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			minimized = true;
			return false;
		}

		minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}
}
