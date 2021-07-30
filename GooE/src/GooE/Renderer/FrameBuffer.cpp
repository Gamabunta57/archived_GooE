#include "gepch.h"

#include <GooE/Renderer/Renderer.h>
#include <Platform/OpenGL/OpenGLFrameBuffer.h>

#include "FrameBuffer.h"

namespace GooE {
	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& specification) {
		switch (Renderer::GetApi()) {
			case RendererApi::Api::None: {
				GOOE_CORE_ASSERT(false, "RendererApi::None is not supported!");
				return nullptr;
			}
			case RendererApi::Api::OpenGL: {
				return CreateRef<OpenGLFrameBuffer>(specification);
			}
		}

		GOOE_CORE_ASSERT(false, "Unknown RendererApi");
		return nullptr;
	}
}