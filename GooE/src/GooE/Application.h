#pragma once
#include "Window.h"

#include "Core.h"
#include "GooE/Events/ApplicationEvent.h"
#include "GooE/Layer.h"
#include "GooE/LayerStack.h"

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
		bool isRunning = true;
		LayerStack layerStack;

	private:
		static Application* instance;
	};

	Application* CreateApplication();	
}
