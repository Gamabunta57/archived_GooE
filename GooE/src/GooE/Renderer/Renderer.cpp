#include "gepch.h"

#include <Platform/OpenGL/OpenGLShader.h>

#include "Renderer.h"
#include "RenderCommand.h"
#include "Shader.h"

namespace GooE {

	Renderer::SceneData* Renderer::sceneData = new Renderer::SceneData;

	void Renderer::Init() {
		RenderCommand::Init();
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
