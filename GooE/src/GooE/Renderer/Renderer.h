#pragma once

#include "RendererApi.h"

namespace GooE {

	class Renderer {
	public:
		static void BeginScene();
		static void EndScene();
		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererApi::Api GetApi() { return RendererApi::GetApi(); }
	};
}

