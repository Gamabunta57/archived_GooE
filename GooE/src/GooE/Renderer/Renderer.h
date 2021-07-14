#pragma once

#include "RendererApi.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace GooE {

	class Renderer {
	public:
		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);

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

		static Scope<SceneData> sceneData;
	};
}
