#pragma once

#include "VertexArray.h"
#include "RendererApi.h"

namespace GooE {

	class RenderCommand {
	public:

		inline static void Init() {
			rendererApi->Init();
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
			rendererApi->SetViewport(x, y, width, height);
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

		inline static void DrawIndexed(const Ref<VertexArray>& vertextArray, const uint32_t count) {
			rendererApi->DrawIndexed(vertextArray, count);
		}

	private:
		static Scope<RendererApi> rendererApi;
	};
}
