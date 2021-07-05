#include "gepch.h"
#include "glad/glad.h"
#include "OpenGLContext.h"

namespace GooE {

	OpenGLContext::OpenGLContext(GLFWwindow* window) : window(window) {
		GOOE_CORE_ASSERT(window, "Window handle is null!")
	}

	void OpenGLContext::Init() {
		glfwMakeContextCurrent(window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		GOOE_CORE_ASSERT(status, "Failed to initialize Glad!")
	}

	void OpenGLContext::SwapBuffers() {
		glfwSwapBuffers(window);
	}
}