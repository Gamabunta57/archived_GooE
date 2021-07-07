#pragma once
#include <string>

namespace GooE {
	class Shader {
	public:
		Shader(const std::string& verstexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;

	private:
		uint32_t rendererId;
	};
}
