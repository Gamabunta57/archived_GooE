#pragma once

namespace GooE {

	enum class RendererApi {
		None = 0,
		OpenGl = 1
	};

	class Renderer {
	public:
		inline static RendererApi GetApi() { return rendererApi; }
		static RendererApi rendererApi;
	};

}

