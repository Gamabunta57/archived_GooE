#include "gepch.h"

#include <glad/glad.h>

#include "OpenGLRendererApi.h"

namespace GooE {

	void OpenGlMessageCallback(unsigned source, unsigned type, unsigned id, unsigned severity, int length, const char* message, const void* userParam) {
		switch (severity) {
			case GL_DEBUG_SEVERITY_HIGH:
				GOOE_CORE_CRITICAL(message); return;
			case GL_DEBUG_SEVERITY_MEDIUM:
				GOOE_CORE_ERROR(message); return;
			case GL_DEBUG_SEVERITY_LOW:
				GOOE_CORE_WARN(message); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION:
				GOOE_CORE_TRACE(message); return;
		}

		GOOE_CORE_ASSERT(false, "OpenGL unknown severity level!");
	}

	void OpenGLRendererApi::Init() {
		GOOE_PROFILE_FUNCTION();

		#ifdef DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGlMessageCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
		#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererApi::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
		glViewport(x, y, width, height);
	}

	void OpenGLRendererApi::SetClearColor(const glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
	};

	void OpenGLRendererApi::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	};

	void OpenGLRendererApi::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) {
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
	};

	void OpenGLRendererApi::DrawIndexed(const Ref<VertexArray>& vertexArray) {
		DrawIndexed(vertexArray, vertexArray->GetIndexBuffer()->GetCount());
	};

}
