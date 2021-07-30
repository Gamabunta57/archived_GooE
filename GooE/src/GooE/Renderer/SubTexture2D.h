#pragma once

#include <glm/glm.hpp>

#include "Texture.h"

namespace GooE {
	class SubTexture2D {
	public:
		SubTexture2D(const Ref<Texture2D>& texture, glm::vec2 min, glm::vec2 max);

		const Ref<Texture2D> GetTexture() const { return texture; }
		const glm::vec2* GetCoords() const { return texCoords; }

		static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& tileSize, const glm::vec2& spriteSize = { 1, 1 });
	private:
		Ref<Texture2D> texture;
		glm::vec2 texCoords[4];
	};
}