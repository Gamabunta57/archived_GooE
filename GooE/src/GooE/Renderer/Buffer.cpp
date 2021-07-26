#include "gepch.h"

#include <Platform/OpenGL/OpenGLBuffer.h>

#include "Buffer.h"
#include "Renderer.h"

namespace GooE {

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size) {
		switch (Renderer::GetApi()) {
			case RendererApi::Api::None: {
				GOOE_CORE_ASSERT(false, "RendererApi::None is not supported!");
				return nullptr;
			}
			case RendererApi::Api::OpenGL: {
				return CreateRef<OpenGLVertexBuffer>(size);
			}
		}

		GOOE_CORE_ASSERT(false, "Unknown RendererApi");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {
		switch (Renderer::GetApi()) {
			case RendererApi::Api::None: {
				GOOE_CORE_ASSERT(false, "RendererApi::None is not supported!");
				return nullptr;
			}
			case RendererApi::Api::OpenGL: {
				return CreateRef<OpenGLVertexBuffer>(vertices, size);
			}
		}

		GOOE_CORE_ASSERT(false, "Unknown RendererApi");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size) {
		switch (Renderer::GetApi()) {
		case RendererApi::Api::None: {
			GOOE_CORE_ASSERT(false, "RendererApi::None is not supported!");
			return nullptr;
		}
		case RendererApi::Api::OpenGL: {
			return CreateRef<OpenGLIndexBuffer>(indices, size);
		}
		}

		GOOE_CORE_ASSERT(false, "Unknown RendererApi");
		return nullptr;
	}
}
