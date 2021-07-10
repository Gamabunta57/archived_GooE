#pragma once

#include <GLFW/glfw3.h>
#include <GooE/Renderer/GraphicsContext.h>

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
