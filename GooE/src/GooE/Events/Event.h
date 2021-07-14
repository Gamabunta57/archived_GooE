#pragma once
#include "gepch.h"

#include <spdlog/fmt/ostr.h>
#include <GooE/Core/Core.h>

namespace GooE {
	
	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory {
		NoneEventCategory        = 0,
		ApplicationEventCategory = BIT(0),
		InputEventCategory       = BIT(1),
		KeyboardEventCategory    = BIT(2),
		MouseEventCategory       = BIT(3),
		MouseButtonEventCategory = BIT(4),
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
							   virtual EventType GetEventType() const override { return GetStaticType(); }\
							   virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class GOOE_API Event {
		friend class EventDispatcher;

	public:
		bool isHandled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char * GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category) {
			return GetCategoryFlags() & category;
		}
	};

	class GOOE_API EventDispatcher {
		template<typename T>
		using EventFn = std::function<bool(T&)>;

	public:
		EventDispatcher(Event& event) : event(event) {}

		template<typename T>
		bool Dispatch(EventFn<T> func) {
			if (event.GetEventType() != T::GetStaticType())
				return false;

			event.isHandled = func(*(T*)&event);
			return true;
		}

	private:
		Event& event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.ToString();
	}
}
