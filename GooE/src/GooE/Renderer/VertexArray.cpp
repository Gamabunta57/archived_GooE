#include "gepch.h"

#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace GooE {
	VertexArray* VertexArray::Create() {
		switch (Renderer::GetApi()) {
			case RendererApi::None: GOOE_CORE_ASSERT(false, "RendererApi::None is not supported!") return nullptr;
			case RendererApi::OpenGl: return new OpenGLVertexArray();
		}

		GOOE_CORE_ASSERT(false, "Unknown RendererApi")
		return nullptr;
	}
}