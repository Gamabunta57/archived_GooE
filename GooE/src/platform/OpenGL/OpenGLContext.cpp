#include "gepch.h"

#include <glad/glad.h>

#include "OpenGLContext.h"

namespace GooE {

	OpenGLContext::OpenGLContext(GLFWwindow* window) : window(window) {
		GOOE_CORE_ASSERT(window, "Window handle is null!")
	}

	void OpenGLContext::Init() {
		glfwMakeContextCurrent(window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		GOOE_CORE_ASSERT(status, "Failed to initialize Glad!")

		GOOE_CORE_INFO("OpenGL Renderer:");
		GOOE_CORE_INFO("  Vendor:   {0}", glGetString(GL_VENDOR));
		GOOE_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		GOOE_CORE_INFO("  Version:  {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers() {
		glfwSwapBuffers(window);
	}
}
