#pragma once
#include <GooE/Core.h>
#include <GooE/Events/Event.h>
#include <GooE/Core/Timestep.h>

namespace GooE {
	class GOOE_API Layer {
	public:
		Layer(const std::string & name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return debugName; }

	private:
		std::string debugName;
	};
}
