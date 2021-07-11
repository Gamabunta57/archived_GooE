#include "gepch.h"

#include "Renderer.h"
#include "RenderCommand.h"
#include "Shader.h"

namespace GooE {

	Renderer::SceneData* Renderer::sceneData = new Renderer::SceneData;

	void Renderer::BeginScene(OrthographicCamera& camera) {
		sceneData->viewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene() {

	}

	void Renderer::Submit(
			const std::shared_ptr<Shader>& shader,
			const std::shared_ptr<VertexArray>& vertexArray,
			const glm::mat4& transform) {
		shader->Bind();
		shader->UploadUniformMat4("viewProjection", sceneData->viewProjectionMatrix);
		shader->UploadUniformMat4("transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}
