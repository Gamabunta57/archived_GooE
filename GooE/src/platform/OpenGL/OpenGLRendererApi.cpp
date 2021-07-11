#include "gepch.h"

#include <glad/glad.h>

#include "OpenGLRendererApi.h"

namespace GooE {
	void OpenGLRendererApi::SetClearColor(const glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
	};

	void OpenGLRendererApi::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	};

	void OpenGLRendererApi::DrawIndexed(const Ref<VertexArray>& vertexArray) {
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	};
}
