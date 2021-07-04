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

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		float time = 0.0f;
	};
}

