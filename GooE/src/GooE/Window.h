#pragma once

#include "GooE/Core.h"
#include "GooE/Events/Event.h"

namespace GooE {

	struct WindowProperties {
		std::string title;
		unsigned int width;
		unsigned int height;

		WindowProperties(
			const std::string& title = "GooE Engine",
			unsigned int width = 1280,
			unsigned int height = 720)
			: title(title), width(width), height(height) {}
	};

	class GOOE_API Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;
		
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool isEnabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProperties& properties = WindowProperties());
	};
}
