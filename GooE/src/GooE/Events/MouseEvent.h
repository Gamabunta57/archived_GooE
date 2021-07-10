#pragma once
#include <GooE/Core.h>

#include "Event.h"

namespace GooE {

	class GOOE_API MouseButtonPressedEvent : public Event {
	public:
		MouseButtonPressedEvent(int button) : button(button) {}

		inline int GetButton() const { return button; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << button;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(MouseEventCategory | MouseButtonEventCategory | InputEventCategory)
		EVENT_CLASS_TYPE (MouseButtonPressed)


	private:
		int button;
	};

	class GOOE_API MouseButtonReleasedEvent : public Event {
	public:
		MouseButtonReleasedEvent(int button) : button(button) {}

		inline int GetButton() const { return button; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << button;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(MouseEventCategory | MouseButtonEventCategory | InputEventCategory)
		EVENT_CLASS_TYPE(MouseButtonReleased)

	private:
		int button;
	};
		
	class GOOE_API MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(unsigned int x, unsigned int y) : x(x), y(y) {}

		inline unsigned int GetX() const { return x; }
		inline unsigned int GetY() const { return y; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << x << ", " << y;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(MouseEventCategory | InputEventCategory)
		EVENT_CLASS_TYPE(MouseMoved)

	private:
		unsigned int x;
		unsigned int y;
	};
	
	class GOOE_API MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(int offsetX, int offsetY) : offsetX(offsetX), offsetY(offsetY){}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << offsetX << ", " << offsetY;
			return ss.str();
		}

		inline int GetOffsetX() const { return offsetX; }
		inline int GetOffsetY() const { return offsetY; }

		EVENT_CLASS_CATEGORY(MouseEventCategory | InputEventCategory)
		EVENT_CLASS_TYPE(MouseScrolled)

	private:
		int offsetX, offsetY;
	};
}
