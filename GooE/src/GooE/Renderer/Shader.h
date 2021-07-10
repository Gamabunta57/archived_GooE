#pragma once
#include <string>
#include <glm/glm.hpp>

namespace GooE {
	class Shader {
	public:
		Shader(const std::string& verstexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void UploadUniformMat4(const std::string name, const glm::mat4 matrix);

	private:
		uint32_t rendererId;
	};
}
