#pragma once

#include "GooE/Renderer/RendererApi.h"

namespace GooE {
	class OpenGLRendererApi : public RendererApi {
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
	};
}