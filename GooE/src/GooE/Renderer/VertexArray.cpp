#include "gepch.h"

#include <Platform/OpenGL/OpenGLVertexArray.h>

#include "VertexArray.h"
#include "Renderer.h"

namespace GooE {
	VertexArray* VertexArray::Create() {
		switch (Renderer::GetApi()) {
			case RendererApi::Api::None: GOOE_CORE_ASSERT(false, "RendererApi::None is not supported!") return nullptr;
			case RendererApi::Api::OpenGl: return new OpenGLVertexArray();
		}

		GOOE_CORE_ASSERT(false, "Unknown RendererApi")
		return nullptr;
	}
}
