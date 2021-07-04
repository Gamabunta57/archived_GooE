#pragma once
#include "GooE/Core.h"
#include "Event.h"

namespace GooE {
	class GOOE_API WindowResizeEvent : public Event {

	public:
		WindowResizeEvent(unsigned int width, unsigned int height) : width(width), height(height) {}

		inline unsigned int GetWidth() const { return width; }
		inline unsigned int GetHeight() const { return height; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowResizeEvent: " << width << "," << height;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(ApplicationEventCategory)
		EVENT_CLASS_TYPE(WindowResize)

	private:
		unsigned int width;
		unsigned int height;
	};

	class GOOE_API WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowCloseEvent";
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(ApplicationEventCategory)
		EVENT_CLASS_TYPE(WindowClose)
	};

	class GOOE_API WindowFocusEvent : public Event {
	public:
		WindowFocusEvent() {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowFocusEvent";
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(ApplicationEventCategory)
		EVENT_CLASS_TYPE(WindowFocus)
	};
		
	class GOOE_API WindowLostFocusEvent : public Event {
	public:
		WindowLostFocusEvent() {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowLostFocusEvent";
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(ApplicationEventCategory)
		EVENT_CLASS_TYPE(WindowLostFocus)
	};
	
	class GOOE_API WindowMovedEvent : public Event {
	public:
		WindowMovedEvent(unsigned int x, unsigned int y) : x(x), y(y) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowMovedEvent: " << x << ", " << y;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(ApplicationEventCategory)
		EVENT_CLASS_TYPE(WindowMoved)

	private:
		unsigned int x;
		unsigned int y;
	};
}
