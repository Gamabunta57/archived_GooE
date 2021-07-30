#include "gepch.h"

#include <glm/gtc/matrix_transform.hpp>

#include "RenderCommand.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"

namespace GooE {

	struct QuadVertex {
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texCoords;
		float texIndex;
		float tilingFactor;
	};

	struct Renderer2DData {
		static const uint32_t MaxQuads = 10000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32; //TODO: RenderCaps

		Ref<VertexArray> quadVertexArray;
		Ref<VertexBuffer> quadVertexBuffer;
		Ref<Shader> textureShader;
		Ref<Texture2D> whiteTexture;

		uint32_t quadIndexCount = 0;
		QuadVertex* quadVertexBufferBase = nullptr;
		QuadVertex* quadVertexBufferPointer = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> textureSlots;
		uint32_t textureSlotIndex = 1; // 0 should be our white texture;

		glm::vec4 quadVertexPositions[4];

		Renderer2D::Statistics stats;
	};

	static Renderer2DData data;

	void Renderer2D::Init() {
		GOOE_PROFILE_FUNCTION();

		data.quadVertexArray = VertexArray::Create();

		data.quadVertexBuffer = VertexBuffer::Create(data.MaxVertices * sizeof(QuadVertex));
		data.quadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "position" },
			{ ShaderDataType::Float4, "color" },
			{ ShaderDataType::Float2, "texCoords" },
			{ ShaderDataType::Float, "texIndex" },
			{ ShaderDataType::Float, "tilingFactor" },
			});
		data.quadVertexArray->AddVertexBuffer(data.quadVertexBuffer);

		data.quadVertexBufferBase = new QuadVertex[data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[data.MaxIndices];

		for (uint32_t i = 0, offset = 0; i < data.MaxIndices; i += 6, offset += 4) {
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, data.MaxIndices);
		data.quadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		int32_t samplers[data.MaxTextureSlots];
		for (int32_t i = 0; i < data.MaxTextureSlots; i++)
			samplers[i] = i;
		
		data.whiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		data.whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		data.textureShader = Shader::Create("assets/shaders/texture.glsl");
		data.textureShader->Bind();
		data.textureShader->SetIntArray("_textures", samplers, data.MaxTextureSlots);
		data.textureSlots[0] = data.whiteTexture;

		data.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		data.quadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		data.quadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		data.quadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown() {
		GOOE_PROFILE_FUNCTION();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		GOOE_PROFILE_FUNCTION();

		data.textureShader->Bind();
		data.textureShader->SetMat4("viewProjection", camera.GetViewProjectionMatrix());

		data.quadIndexCount = 0;
		data.quadVertexBufferPointer = data.quadVertexBufferBase;

		data.textureSlotIndex = 1;
	}

	void Renderer2D::EndScene() {
		GOOE_PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t*)data.quadVertexBufferPointer - (uint8_t*)data.quadVertexBufferBase;
		data.quadVertexBuffer->SetData(data.quadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush() {
		for (uint32_t i = 0; i < data.textureSlotIndex; i++)
			data.textureSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(data.quadVertexArray, data.quadIndexCount);
		data.stats.drawCalls++;
	}

	void Renderer2D::FlushAndReset() {
		EndScene();

		data.quadIndexCount = 0;
		data.quadVertexBufferPointer = data.quadVertexBufferBase;

		data.textureSlotIndex = 1;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		GOOE_PROFILE_FUNCTION();

		if (data.quadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		constexpr size_t quadVertexCount = 4;
		const float textureIndex = 0.0f; // white texture
		constexpr glm::vec2 textureCoords[] = { 
			{0.0f, 0.0f}, 
			{1.0f, 0.0f}, 
			{1.0f, 1.0f}, 
			{0.0f, 1.0f} 
		};
		const float tilingFactor = 1.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (size_t i = 0; i < quadVertexCount; i++) {
			data.quadVertexBufferPointer->position = transform * data.quadVertexPositions[i];
			data.quadVertexBufferPointer->color = color;
			data.quadVertexBufferPointer->texCoords = textureCoords[i];
			data.quadVertexBufferPointer->texIndex = textureIndex;
			data.quadVertexBufferPointer->tilingFactor = tilingFactor;
			data.quadVertexBufferPointer++;
		}

		data.quadIndexCount += 6;
		data.stats.quadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint, const float tilingFactor) {
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tint, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint, const float tilingFactor) {
		GOOE_PROFILE_FUNCTION();

		if (data.quadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		constexpr glm::vec2 textureCoords[] = {
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};

		uint32_t textureIndex = 0.0f;

		for (uint32_t i = 1; i < data.textureSlotIndex; i++) {
			if (*data.textureSlots[i].get() == *texture.get()) {
				textureIndex = i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			textureIndex = (float)data.textureSlotIndex;
			data.textureSlots[data.textureSlotIndex] = texture;
			data.textureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		
		for (size_t i = 0; i < quadVertexCount; i++) {
			data.quadVertexBufferPointer->position = transform * data.quadVertexPositions[i];
			data.quadVertexBufferPointer->color = color;
			data.quadVertexBufferPointer->texCoords = textureCoords[i];
			data.quadVertexBufferPointer->texIndex = textureIndex;
			data.quadVertexBufferPointer->tilingFactor = tilingFactor;
			data.quadVertexBufferPointer++;
		}

		data.quadIndexCount += 6;

		data.stats.quadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& tint, const float tilingFactor) {
		DrawQuad({ position.x, position.y, 0.0f }, size, subTexture, tint, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& tint, const float tilingFactor) {
		GOOE_PROFILE_FUNCTION();

		if (data.quadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		const glm::vec2* textureCoords = subTexture->GetCoords();
		const Ref<Texture2D> texture = subTexture->GetTexture();
		uint32_t textureIndex = 0.0f;

		for (uint32_t i = 1; i < data.textureSlotIndex; i++) {
			if (*data.textureSlots[i].get() == *texture.get()) {
				textureIndex = i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			textureIndex = (float)data.textureSlotIndex;
			data.textureSlots[data.textureSlotIndex] = texture;
			data.textureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (size_t i = 0; i < quadVertexCount; i++) {
			data.quadVertexBufferPointer->position = transform * data.quadVertexPositions[i];
			data.quadVertexBufferPointer->color = color;
			data.quadVertexBufferPointer->texCoords = textureCoords[i];
			data.quadVertexBufferPointer->texIndex = textureIndex;
			data.quadVertexBufferPointer->tilingFactor = tilingFactor;
			data.quadVertexBufferPointer++;
		}

		data.quadIndexCount += 6;

		data.stats.quadCount++;
	}


	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const glm::vec4& color) {
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const glm::vec4& color) {
		GOOE_PROFILE_FUNCTION();

		if (data.quadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		const float textureIndex = 0.0f; // white texture
		const float tilingFactor = 1.0f;
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { 
			{0.0f, 0.0f}, 
			{1.0f, 0.0f}, 
			{1.0f, 1.0f}, 
			{0.0f, 1.0f} 
		};

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (size_t i = 0; i < quadVertexCount; i++) {
			data.quadVertexBufferPointer->position = transform * data.quadVertexPositions[i];
			data.quadVertexBufferPointer->color = color;
			data.quadVertexBufferPointer->texCoords = textureCoords[i];
			data.quadVertexBufferPointer->texIndex = textureIndex;
			data.quadVertexBufferPointer->tilingFactor = tilingFactor;
			data.quadVertexBufferPointer++;
		}

		data.quadIndexCount += 6;

		data.stats.quadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const Ref<Texture2D>& texture, const glm::vec4& tint, const float tilingFactor) {
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tint, tilingFactor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const Ref<Texture2D>& texture, const glm::vec4& tint, const float tilingFactor) {
		GOOE_PROFILE_FUNCTION();

		if (data.quadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { 
			{0.0f, 0.0f}, 
			{1.0f, 0.0f}, 
			{1.0f, 1.0f},
			{0.0f, 1.0f} 
		};
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		uint32_t textureIndex = 0.0f;

		for (uint32_t i = 1; i < data.textureSlotIndex; i++) {
			if (*data.textureSlots[i].get() == *texture.get()) {
				textureIndex = i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			textureIndex = (float)data.textureSlotIndex;
			data.textureSlots[data.textureSlotIndex] = texture;
			data.textureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f})
			* glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

		for (size_t i = 0; i < quadVertexCount; i++) {
			data.quadVertexBufferPointer->position = transform * data.quadVertexPositions[i];
			data.quadVertexBufferPointer->color = color;
			data.quadVertexBufferPointer->texCoords = textureCoords[i];
			data.quadVertexBufferPointer->texIndex = textureIndex;
			data.quadVertexBufferPointer->tilingFactor = tilingFactor;
			data.quadVertexBufferPointer++;
		}

		data.quadIndexCount += 6;

		data.stats.quadCount++;
	}

	void Renderer2D::ResetStats() {
		memset(&data.stats, 0, sizeof(Renderer2D::Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats() {
		return data.stats;
	}
}
