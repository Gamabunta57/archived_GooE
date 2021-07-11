#pragma once

#include "VertexArray.h"
#include "RendererApi.h"

namespace GooE {

	class RenderCommand {
	public:

		inline static void Init() {
			rendererApi->Init();
		}

		inline static void SetClearColor(const glm::vec4& color) {
			rendererApi->SetClearColor(color);
		}

		inline static void Clear() {
			rendererApi->Clear();
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertextArray) {
			rendererApi->DrawIndexed(vertextArray);
		}

	private:
		static RendererApi* rendererApi;
	};
}
