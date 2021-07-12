#include "gepch.h"

#include <Platform/OpenGL/OpenGLShader.h>
#include <GooE/Renderer/Renderer.h>

#include "Shader.h"


namespace GooE {
	Ref<Shader> Shader::Create(const std::string& path) {
		switch (Renderer::GetApi()) {
			case RendererApi::Api::None: GOOE_CORE_ASSERT(false, "RendererApi::None is not supported!") return nullptr;
			case RendererApi::Api::OpenGl: return std::make_shared<OpenGLShader>(path);
		}

		GOOE_CORE_ASSERT(false, "Unknown RendererApi")
			return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) {
		switch (Renderer::GetApi()) {
			case RendererApi::Api::None: GOOE_CORE_ASSERT(false, "RendererApi::None is not supported!") return nullptr;
			case RendererApi::Api::OpenGl: return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		GOOE_CORE_ASSERT(false, "Unknown RendererApi")
			return nullptr;
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader) {
		GOOE_CORE_ASSERT(shaders.find(name) == shaders.end(), "Shader already exists!");
		
		shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader) {
		const std::string& name = shader->GetName();
		Add(name, shader);
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& path) {
		auto shader = Shader::Create(path);
		Add(shader);

		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& path) {
		auto shader = Shader::Create(path);
		Add(name, shader);

		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name) {
		GOOE_CORE_ASSERT(shaders.find(name) != shaders.end(), "Shader doesn't exist!");

		return shaders[name];
	}
}
