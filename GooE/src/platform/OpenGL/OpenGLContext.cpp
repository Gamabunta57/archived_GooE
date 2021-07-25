#include "gepch.h"

#include <glad/glad.h>

#include "OpenGLContext.h"

namespace GooE {

	OpenGLContext::OpenGLContext(GLFWwindow* window) : window(window) {
		GOOE_CORE_ASSERT(window, "Window handle is null!")
	}

	void OpenGLContext::Init() {
		GOOE_PROFILE_FUNCTION();

		glfwMakeContextCurrent(window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		GOOE_CORE_ASSERT(status, "Failed to initialize Glad!")

		GOOE_CORE_INFO("OpenGL Renderer:");
		GOOE_CORE_INFO("  Vendor:   {0}", glGetString(GL_VENDOR));
		GOOE_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		GOOE_CORE_INFO("  Version:  {0}", glGetString(GL_VERSION));

		#ifdef GOOE_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		GOOE_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "GooE requires at lest OpenGL version 4.5 to run!");
		#endif
	}

	void OpenGLContext::SwapBuffers() {
		GOOE_PROFILE_FUNCTION();

		glfwSwapBuffers(window);
	}
}
