#pragma once

#include <GooE/Core/Core.h>
#include <GooE/Events/Event.h>

namespace GooE {

	struct WindowProperties {
		std::string title;
		uint32_t width;
		uint32_t height;

		WindowProperties(
			const std::string& title = "GooE Engine",
			uint32_t width = 1600,
			uint32_t height = 900)
			: title(title), width(width), height(height) {}
	};

	class GOOE_API Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;
		
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool isEnabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProperties& properties = WindowProperties());
	};
}
