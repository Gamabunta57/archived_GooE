#pragma once
#include "Window.h"

#include "Core.h"
#include "GooE/Events/ApplicationEvent.h"
#include "GooE/Layer.h"
#include "GooE/LayerStack.h"
#include "ImGui/ImGuiLayer.h"
#include <GooE/Renderer/Shader.h>

namespace GooE {
	class GOOE_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);
		
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *instance; }
		inline Window& GetWindow() { return *window; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> window;
		ImGuiLayer* imguiLayer;
		bool isRunning = true;
		LayerStack layerStack;

		unsigned int vertexArray, vertexBuffer, indexBuffer;

		std::unique_ptr<Shader> shader;

	private:
		static Application* instance;
	};

	Application* CreateApplication();	
}
