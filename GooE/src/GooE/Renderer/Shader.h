#pragma once
#include <string>

namespace GooE {
	class Shader {
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Shader* Create(const std::string& path);
		static Shader* Create(const std::string& verstexSrc, const std::string& fragmentSrc);
	private:
		uint32_t rendererId;
	};
}
