#include "gepch.h"

#include <Platform/OpenGL/OpenGLShader.h>
#include <GooE/Renderer/Renderer.h>

#include "Shader.h"


namespace GooE {
	Shader* Shader::Create(const std::string& path) {
		switch (Renderer::GetApi()) {
			case RendererApi::Api::None: GOOE_CORE_ASSERT(false, "RendererApi::None is not supported!") return nullptr;
			case RendererApi::Api::OpenGl: return new OpenGLShader(path);
		}

		GOOE_CORE_ASSERT(false, "Unknown RendererApi")
			return nullptr;
	}

	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc) {
		switch (Renderer::GetApi()) {
			case RendererApi::Api::None: GOOE_CORE_ASSERT(false, "RendererApi::None is not supported!") return nullptr;
			case RendererApi::Api::OpenGl: return new OpenGLShader(vertexSrc, fragmentSrc);
		}

		GOOE_CORE_ASSERT(false, "Unknown RendererApi")
			return nullptr;
	}
}
