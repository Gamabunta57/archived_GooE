#pragma once

#include <glm/glm.hpp>
#include "VertexArray.h"

namespace GooE {
	class RendererApi {
	public:

		enum class Api {
			None = 0,
			OpenGl = 1
		};

	public:
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

		inline static Api GetApi() { return api; }

	private:
		static Api api;
	};
}
