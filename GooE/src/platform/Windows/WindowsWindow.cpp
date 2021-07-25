#include "gepch.h"

#include <GooE/Events/ApplicationEvent.h>
#include <GooE/Events/KeyEvent.h>
#include <GooE/Events/MouseEvent.h>
#include <Platform/OpenGL/OpenGLContext.h>

#include "WindowsWindow.h"

namespace GooE {
	static bool isGLFWinitialized = false;

	static void GLFWErrorCallback(int error, const char* description) {
		//GOOE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window* Window::Create(const WindowProperties& properties) {
		return new WindowsWindow(properties);
	}

	WindowsWindow::WindowsWindow(const WindowProperties& properties) {
		GOOE_PROFILE_FUNCTION();

		Init(properties);
	}

	WindowsWindow::~WindowsWindow() {
		GOOE_PROFILE_FUNCTION();
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProperties& properties) {
		GOOE_PROFILE_FUNCTION();

		data.title = properties.title;
		data.width = properties.width;
		data.height = properties.height;

		GOOE_CORE_INFO("Initializing window {0} ({1}, {2})", data.title, data.width, data.height);

		if (!isGLFWinitialized) {
			GOOE_PROFILE_SCOPE("GLFW Init");
			int success = glfwInit();
			GOOE_CORE_ASSERT(success, "GLFW has not been initialized!");
			glfwSetErrorCallback(GLFWErrorCallback);
			isGLFWinitialized = true;
		}

		GOOE_CORE_INFO("Initialized window {0} ({1}, {2})", data.title, data.width, data.height);

		{
			GOOE_PROFILE_SCOPE("GLFW Create Window");
			window = glfwCreateWindow((int)data.width, (int)data.height, data.title.c_str(), nullptr, nullptr);
		}
		
		context = CreateScope<OpenGLContext>(window);
		context->Init();

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

		glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int codepoint) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent e(codepoint);
			data.eventCallback(e);
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
			MouseMovedEvent e((unsigned int)x, (unsigned int)y);
			data.eventCallback(e);
		});

		glfwSetScrollCallback(window, [](GLFWwindow* window, double x, double y) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent e((unsigned int)x, (unsigned int)y);
			data.eventCallback(e);
		});

		GOOE_CORE_INFO("GLFW event callback set.");
	}

	void WindowsWindow::Shutdown() {
		GOOE_PROFILE_FUNCTION();

		glfwDestroyWindow(window);
		window = nullptr;
	}

	void WindowsWindow::OnUpdate() {
		GOOE_PROFILE_FUNCTION();

		glfwPollEvents();
		context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool isEnabled) {
		GOOE_PROFILE_FUNCTION();

		glfwSwapInterval(isEnabled ? 1 : 0);
		data.vSync = isEnabled;
	}

	bool WindowsWindow::IsVSync() const {
		return data.vSync;
	}
}
