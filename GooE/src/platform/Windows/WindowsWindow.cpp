#include "gepch.h"
#include "GLFW/glfw3.h"
#include "WindowsWindow.h"
#include "GooE/Events/ApplicationEvent.h"
#include "GooE/Events/KeyEvent.h"
#include "GooE/Events/MouseEvent.h"

namespace GooE {
	static bool isGLFWinitialized = false;

	static void GLFWErrorCallback(int error, const char* description) {
		//GOOE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

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

		GOOE_CORE_INFO("Initializing window {0} ({1}, {2})", data.title, data.width, data.height);

		if (!isGLFWinitialized) {
			int success = glfwInit();
			GOOE_CORE_ASSERT(success, "GLFW has not been initialized!");
			glfwSetErrorCallback(GLFWErrorCallback);
			isGLFWinitialized = true;
		}

		GOOE_CORE_INFO("Initialized window {0} ({1}, {2})", data.title, data.width, data.height);

		window = glfwCreateWindow((int)data.width, (int)data.height, data.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(window);
		glfwSetWindowUserPointer(window, &data);
		SetVSync(true);

		GOOE_CORE_INFO("GLFW context window created.");

		glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.width = width;
			data.height = height;

			WindowResizeEvent e(width, height);
			data.eventCallback(e);
		});

		glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent e;
			data.eventCallback(e);
		});

		glfwSetWindowFocusCallback(window, [](GLFWwindow* window, int focused) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			if (focused == GLFW_TRUE) {
				WindowFocusEvent e;
				data.eventCallback(e);
			}
			else {
				WindowLostFocusEvent e;
				data.eventCallback(e);
			}
		});

		glfwSetWindowIconifyCallback(window, [](GLFWwindow* window, int iconified) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			if (iconified == GLFW_TRUE) {
				WindowLostFocusEvent e;
				data.eventCallback(e);
			}
			else {
				WindowFocusEvent e;
				data.eventCallback(e);
			}
		});

		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action) {
				case GLFW_PRESS: {
					KeyPressedEvent e(key, 0);
					data.eventCallback(e);
					break;
				}
				case GLFW_REPEAT: {
					KeyPressedEvent e(key, 1);
					data.eventCallback(e);
					break;
				}
				case GLFW_RELEASE: {
					KeyReleasedEvent e(key);
					data.eventCallback(e);
					break;
				}
			}
		});

		glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action) {
				case GLFW_PRESS: {
					MouseButtonPressedEvent e(button);
					data.eventCallback(e);
					break;
				}
				case GLFW_RELEASE: {
					MouseButtonReleasedEvent e(button);
					data.eventCallback(e);
					break;
				}
			}
		});

		glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent e((float)x, (float)y);
			data.eventCallback(e);
		});

		glfwSetScrollCallback(window, [](GLFWwindow* window, double x, double y) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent e((float)x, (float)y);
			data.eventCallback(e);
		});

		GOOE_CORE_INFO("GLFW event callback set.");
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