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
		virtual void OnEvent(Event& event) override;

		void Begin();
		void End();
		void SetBlockEvents(bool blockEvents) { this->blockEvents = blockEvents; }

	private:
		bool blockEvents = true;
		float time = 0.0f;
	};
}
