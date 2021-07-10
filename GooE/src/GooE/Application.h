#pragma once
#include "Window.h"

#include <GooE/Events/ApplicationEvent.h>
#include <GooE/ImGui/ImGuiLayer.h>
#include <GooE/Layer.h>
#include <GooE/LayerStack.h>
#include <GooE/Renderer/Buffer.h>
#include <GooE/Renderer/Shader.h>
#include <GooE/Renderer/VertexArray.h>
#include <GooE/Renderer/OrthographicCamera.h>

#include "Core.h"

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

		std::shared_ptr<Shader> shader;
		std::shared_ptr<VertexArray> vertexArray;
		
		std::shared_ptr<Shader> squareShader;
		std::shared_ptr<VertexArray> squareVertexArray;

		OrthographicCamera camera;
	private:
		static Application* instance;
	};

	Application* CreateApplication();	
}
