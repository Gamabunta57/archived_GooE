#include "gepch.h"
#include "Buffer.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace GooE {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
		switch (Renderer::GetApi()) {
			case RendererApi::None: {
				GOOE_CORE_ASSERT(false, "RendererApi::None is not supported!");
				return nullptr;
			}
			case RendererApi::OpenGl: {
				return new OpenGLVertexBuffer(vertices, size);
			}
		}

		GOOE_CORE_ASSERT(false, "Unknown RendererApi");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size) {
		switch (Renderer::GetApi()) {
		case RendererApi::None: {
			GOOE_CORE_ASSERT(false, "RendererApi::None is not supported!");
			return nullptr;
		}
		case RendererApi::OpenGl: {
			return new OpenGLIndexBuffer(indices, size);
		}
		}

		GOOE_CORE_ASSERT(false, "Unknown RendererApi");
		return nullptr;
	}
}