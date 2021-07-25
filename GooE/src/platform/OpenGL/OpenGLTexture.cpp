#include "gepch.h"

#include <glad/glad.h>
#include <stb_image.h>

#include "OpenGLTexture.h"

namespace GooE {

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height) : width(width), height(height) {
		GOOE_PROFILE_FUNCTION();

		internalFormat = GL_RGBA8;
		dataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &rendererId);
		glTextureStorage2D(rendererId, 1, internalFormat, width, height);

		glTextureParameteri(rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(rendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(rendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(rendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : path(path) {
		GOOE_PROFILE_FUNCTION();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			GOOE_PROFILE_SCOPE("OpenGLTexture2D::OpenGLTexture2D (const std::string&) - stbi_load");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
		GOOE_CORE_ASSERT(data, "Failed to load image: {0}", path);
		this->width = width;
		this->height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4) {
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		} else if (channels == 3) {
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		GOOE_CORE_ASSERT(internalFormat, "Format not supported");
		GOOE_CORE_ASSERT(dataFormat, "Format not supported");

		glCreateTextures(GL_TEXTURE_2D, 1, &rendererId);
		glTextureStorage2D(rendererId, 1, internalFormat, this->width, this->height);

		glTextureParameteri(rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(rendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(rendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(rendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(rendererId, 0, 0, 0, this->width, this->height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		GOOE_PROFILE_FUNCTION();

		glDeleteTextures(1, &rendererId);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size) {
		GOOE_PROFILE_FUNCTION();

		uint32_t bpp = dataFormat == GL_RGBA ? 4 : 3;
		GOOE_CORE_ASSERT(size == width * height * bpp, "Data must fulfill the entire texture!");
		glTextureSubImage2D(rendererId, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const {
		GOOE_PROFILE_FUNCTION();

		glBindTextureUnit(slot, rendererId);
	}
}
