#pragma once

#include <GooE/Renderer/Texture.h>

namespace GooE {

	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();
		virtual uint32_t GetWidth() const override { return width; }
		virtual uint32_t GetHeight() const override { return height; }
		//virtual uint32_t GetFormat() const override;

		virtual void Bind(uint32_t slot = 0) const override;

	private:
		std::string path;
		uint32_t width, height;
		uint32_t rendererId;
	};
}