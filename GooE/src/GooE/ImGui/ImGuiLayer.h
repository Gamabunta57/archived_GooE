#pragma once
#include "GooE/Layer.h"

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
		float time = 0.0f;
	};
}

