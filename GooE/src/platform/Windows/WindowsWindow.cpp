#include "gepch.h"
#include "WindowsWindow.h"

namespace GooE {
	static bool isGLFWinitialized = false;

	Window* Window::Create(const WindowProperties& properties) {
		return new WindowsWindow(properties);
	}

	WindowsWindow::WindowsWindow(const WindowProperties& properties) {
		Init(properties);
	}

	WindowsWindow::~WindowsWindow() {
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProperties& properties) {
		data.title = properties.title;
		data.width = properties.width;
		data.height = properties.height;

		GOOE_CORE_INFO("Initialized window {0} ({1}, {2})", data.title, data.width, data.height);

		if (!isGLFWinitialized) {
			int success = glfwInit();
			GOOE_CORE_ASSERT(success, "GLFW has not been initialized!");

			isGLFWinitialized = true;
		}

		window = glfwCreateWindow((int)data.width, (int)data.height, data.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(window);
		glfwSetWindowUserPointer(window, &data);
		SetVSync(true);
	}

	void WindowsWindow::Shutdown() {
		glfwDestroyWindow(window);
		window = nullptr;
	}

	void WindowsWindow::OnUpdate() {
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	void WindowsWindow::SetVSync(bool isEnabled) {
		glfwSwapInterval(isEnabled ? 1 : 0);
		data.vSync = isEnabled;
	}

	bool WindowsWindow::IsVSync() const {
		return data.vSync;
	}
}