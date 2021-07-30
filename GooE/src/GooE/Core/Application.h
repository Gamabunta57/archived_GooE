#pragma once
#include "Window.h"

#include <GooE/Events/ApplicationEvent.h>
#include <GooE/ImGui/ImGuiLayer.h>
#include <GooE/Core/Layer.h>
#include <GooE/Core/LayerStack.h>
#include <GooE/Core/Core.h>
#include <GooE/Renderer/Buffer.h>
#include <GooE/Renderer/Shader.h>
#include <GooE/Renderer/VertexArray.h>
#include <GooE/Renderer/OrthographicCamera.h>
#include <GooE/Core/Timestep.h>

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

		void Close();

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		std::unique_ptr<Window> window;
		ImGuiLayer* imguiLayer;
		bool isRunning = true;
		bool minimized = false;
		LayerStack layerStack;
		float lastFrameTime = 0.0f;

	private:
		static Application* instance;
	};

	Application* CreateApplication();	
}
