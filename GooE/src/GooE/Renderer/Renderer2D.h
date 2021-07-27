#pragma once

#include "OrthographicCamera.h"
#include "Texture.h"

namespace GooE {

	class Renderer2D {
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Flush();
		
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint = { 1.0f, 1.0f, 1.0f, 1.0f }, const float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint = {1.0f, 1.0f, 1.0f, 1.0f}, const float tilingFactor = 1.0f);

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const Ref<Texture2D>& texture, const glm::vec4& tint = { 1.0f, 1.0f, 1.0f, 1.0f }, const float tilingFactor = 1.0f);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const Ref<Texture2D>& texture, const glm::vec4& tint = { 1.0f, 1.0f, 1.0f, 1.0f }, const float tilingFactor = 1.0f);
	};
}
