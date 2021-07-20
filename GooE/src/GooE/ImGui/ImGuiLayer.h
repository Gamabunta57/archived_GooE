#pragma once
#include <GooE/Events/ApplicationEvent.h>
#include <GooE/Events/KeyEvent.h>
#include <GooE/Events/MouseEvent.h>
#include <GooE/Core/Layer.h>

namespace GooE {
	class GOOE_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();

	private:
		float time = 0.0f;
	};
}
