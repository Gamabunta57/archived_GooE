#include "gepch.h"

#include <Platform/OpenGL/OpenGLTexture.h>

#include "RendererApi.h"
#include "Renderer.h"
#include "Texture.h"

namespace GooE {

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height) {
		switch (Renderer::GetApi()) {
			case RendererApi::Api::None: GOOE_CORE_ASSERT(false, "RendererApi::None is currently not supported!"); return nullptr;
			case RendererApi::Api::OpenGL: return CreateRef<OpenGLTexture2D>(width, height);
		}
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path) {
		switch (Renderer::GetApi()) {
			case RendererApi::Api::None: GOOE_CORE_ASSERT(false, "RendererApi::None is not supported!") return nullptr;
			case RendererApi::Api::OpenGL: return std::make_shared<OpenGLTexture2D>(path);
		}

		GOOE_CORE_ASSERT(false, "Unknown RendererApi");
		return nullptr;
	}
}
