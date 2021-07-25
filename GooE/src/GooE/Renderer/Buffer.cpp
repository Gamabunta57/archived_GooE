#include "gepch.h"

#include <Platform/OpenGL/OpenGLBuffer.h>

#include "Buffer.h"
#include "Renderer.h"

namespace GooE {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
		switch (Renderer::GetApi()) {
			case RendererApi::Api::None: {
				GOOE_CORE_ASSERT(false, "RendererApi::None is not supported!");
				return nullptr;
			}
			case RendererApi::Api::OpenGL: {
				return new OpenGLVertexBuffer(vertices, size);
			}
		}

		GOOE_CORE_ASSERT(false, "Unknown RendererApi");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size) {
		switch (Renderer::GetApi()) {
		case RendererApi::Api::None: {
			GOOE_CORE_ASSERT(false, "RendererApi::None is not supported!");
			return nullptr;
		}
		case RendererApi::Api::OpenGL: {
			return new OpenGLIndexBuffer(indices, size);
		}
		}

		GOOE_CORE_ASSERT(false, "Unknown RendererApi");
		return nullptr;
	}
}
