#include "gepch.h"

#include <GLFW/glfw3.h>

#include <GooE/Renderer/Renderer.h>
#include <GooE/Renderer/RenderCommand.h>

#include "Application.h"
#include "Input.h"

namespace GooE {
	Application* Application::instance = nullptr;

	Application::Application(const std::string& name) {
		GOOE_PROFILE_FUNCTION();

		GOOE_CORE_ASSERT(!instance, "Application already exists!");
		instance = this;

		window = std::unique_ptr<Window>(Window::Create(WindowProperties(name)));
		window->SetEventCallback(GOOE_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		imguiLayer = new ImGuiLayer();
		PushOverlay(imguiLayer);		
	}

	Application::~Application() {
		GOOE_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}

	void Application::Run() {
		GOOE_PROFILE_FUNCTION();

		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		while (isRunning) {
			GOOE_PROFILE_SCOPE("Game Loop");
			float time = (float) glfwGetTime(); //Should be something like Platfrom::GetTime() instead
			Timestep timestep = time - lastFrameTime;
			lastFrameTime = time;

			if (!minimized) {
				{
					GOOE_PROFILE_SCOPE("LayerStack OnUpdate");
					for (Layer* layer : layerStack)
						layer->OnUpdate(timestep);
				}
			}

			{
				GOOE_PROFILE_SCOPE("ImGui Rendering");
				imguiLayer->Begin();
				for (Layer* layer : layerStack)
					layer->OnImGuiRender();
				imguiLayer->End();
			}

			window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e) {
		GOOE_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(GOOE_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(GOOE_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = layerStack.end(); it != layerStack.begin();) {
			if (e.isHandled) break;
			(*--it)->OnEvent(e);
		}
	}

	void Application::PushLayer(Layer* layer) {
		GOOE_PROFILE_FUNCTION();

		layerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer) {
		GOOE_PROFILE_FUNCTION();

		layerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent &e) {
		isRunning = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e) {
		GOOE_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			minimized = true;
			return false;
		}

		minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}

	void Application::Close() {
		isRunning = false;
	}
}
