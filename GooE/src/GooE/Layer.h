#pragma once
#include "GooE/Core.h"
#include "GooE/Events/Event.h"

namespace GooE {

	class GOOE_API Layer
	{
	public:
		Layer(const std::string & name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return debugName; }

	private:
		std::string debugName;
	};
}
