#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <GooE/Renderer/Shader.h>

namespace GooE {
	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& verstexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void UploadUniformInt(const std::string name, const int value);

		void UploadUniformFloat(const std::string name, const float value);
		void UploadUniformFloat2(const std::string name, const glm::vec3& value);
		void UploadUniformFloat3(const std::string name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string name, const glm::vec4& value);

		void UploadUniformMat3(const std::string name, const glm::mat3& value);
		void UploadUniformMat4(const std::string name, const glm::mat4& value);

	private:
		const int GetUniformLocation(const std::string name);

	private:
		uint32_t rendererId;
		std::unordered_map<std::string, int> uniformLocationsCache;
	};
}
