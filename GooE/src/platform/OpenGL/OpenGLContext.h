#pragma once

#include "GooE/Renderer/GraphicsContext.h"
#include "GLFW/glfw3.h"

namespace GooE {

	class OpenGLContext : public GraphicsContext {
	public:

		OpenGLContext(GLFWwindow* window);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* window;
	};
}