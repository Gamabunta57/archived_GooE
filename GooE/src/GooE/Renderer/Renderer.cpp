#include "gepch.h"

#include <Platform/OpenGL/OpenGLShader.h>

#include "Renderer.h"
#include "Renderer2D.h"
#include "RenderCommand.h"
#include "Shader.h"

namespace GooE {

	Scope<Renderer::SceneData> Renderer::sceneData = CreateScope<Renderer::SceneData>();

	void Renderer::Init() {
		GOOE_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera) {
		sceneData->viewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene() {

	}

	void Renderer::Submit(
			const Ref<Shader>& shader,
			const Ref<VertexArray>& vertexArray,
			const glm::mat4& transform) {
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("viewProjection", sceneData->viewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}
