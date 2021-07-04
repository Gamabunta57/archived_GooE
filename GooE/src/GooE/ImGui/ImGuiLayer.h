#pragma once
#include "GooE/Layer.h"
#include "GooE/Events/ApplicationEvent.h"
#include "GooE/Events/KeyEvent.h"
#include "GooE/Events/MouseEvent.h"

namespace GooE {
	class GOOE_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnUpdate();
		virtual void OnEvent(Event& e);
		virtual void OnAttach();
		virtual void OnDetach();

	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);

		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e);

		bool OnWindowResizeEvent(WindowResizeEvent& e);

	private:
		float time = 0.0f;
	};
}

