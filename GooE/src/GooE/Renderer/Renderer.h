#pragma once

#include "RendererApi.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace GooE {

	class Renderer {
	public:
		static void Init();

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();
		static void Submit(
			const Ref<Shader>& shader,
			const Ref<VertexArray>& vertexArray,
			const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererApi::Api GetApi() { return RendererApi::GetApi(); }

	private:
		struct SceneData {
			glm::mat4 viewProjectionMatrix;
		};

		static SceneData* sceneData;
	};
}
