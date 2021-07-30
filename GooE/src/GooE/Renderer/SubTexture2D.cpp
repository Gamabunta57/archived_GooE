#include "gepch.h"

#include "SubTexture2D.h"

namespace GooE {

	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, glm::vec2 min, glm::vec2 max) : texture(texture){
		texCoords[0] = { min.x, min.y };
		texCoords[1] = { max.x, min.y };
		texCoords[2] = { max.x, max.y };
		texCoords[3] = { min.x, max.y };
	}

	Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& tileSize, const glm::vec2& spriteSize) {
		glm::vec2 min = { coords.x * tileSize.x / texture->GetWidth(), coords.y * tileSize.y / texture->GetHeight() };
		glm::vec2 max = { (coords.x + spriteSize.x) * tileSize.x / texture->GetWidth(), (coords.y + spriteSize.y) * tileSize.y / texture->GetHeight()};
		return CreateRef<SubTexture2D>(texture, min, max);
	}
}