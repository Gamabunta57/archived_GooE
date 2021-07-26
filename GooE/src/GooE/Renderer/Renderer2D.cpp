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
		// color, texId
	};

	struct Renderer2DData {
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;

		Ref<VertexArray> quadVertexArray;
		Ref<VertexBuffer> quadVertexBuffer;
		Ref<Shader> textureShader;
		Ref<Texture2D> whiteTexture;

		uint32_t quandIndexCount = 0;
		QuadVertex* quadVertexBufferBase = nullptr;
		QuadVertex* quadVertexBufferPointer = nullptr;
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
		
		data.whiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		data.whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		data.textureShader = Shader::Create("assets/shaders/texture.glsl");
		data.textureShader->Bind();
		data.textureShader->SetInt("_texture", 0);
	}

	void Renderer2D::Shutdown() {
		GOOE_PROFILE_FUNCTION();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		GOOE_PROFILE_FUNCTION();

		data.textureShader->Bind();
		data.textureShader->SetMat4("viewProjection", camera.GetViewProjectionMatrix());

		data.quandIndexCount = 0;
		data.quadVertexBufferPointer = data.quadVertexBufferBase;
	}

	void Renderer2D::EndScene() {
		GOOE_PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t*)data.quadVertexBufferPointer - (uint8_t*)data.quadVertexBufferBase;
		data.quadVertexBuffer->SetData(data.quadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush() {
		RenderCommand::DrawIndexed(data.quadVertexArray, data.quandIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		GOOE_PROFILE_FUNCTION();

		data.quadVertexBufferPointer->position = position;
		data.quadVertexBufferPointer->color = color;
		data.quadVertexBufferPointer->texCoords = { 0.0f, 0.0f };
		data.quadVertexBufferPointer++;

		data.quadVertexBufferPointer->position = { position.x + size.x, position.y, position.z };
		data.quadVertexBufferPointer->color = color;
		data.quadVertexBufferPointer->texCoords = { 1.0f, 0.0f };
		data.quadVertexBufferPointer++;

		data.quadVertexBufferPointer->position = { position.x + size.x, position.y + size.y, position.z };
		data.quadVertexBufferPointer->color = color;
		data.quadVertexBufferPointer->texCoords = { 1.0f, 1.0f };
		data.quadVertexBufferPointer++;

		data.quadVertexBufferPointer->position = { position.x, position.y + size.y, position.z };
		data.quadVertexBufferPointer->color = color;
		data.quadVertexBufferPointer->texCoords = { 0.0f, 1.0f };
		data.quadVertexBufferPointer++;

		data.quandIndexCount += 6;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint, const float tilingFactor) {
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tint, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint, const float tilingFactor) {
		GOOE_PROFILE_FUNCTION();

		data.textureShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		data.textureShader->SetMat4("transform", transform);
		data.textureShader->SetFloat4("_tint", tint);
		data.textureShader->SetFloat("_tilingFactor", tilingFactor);

		texture->Bind();

		data.quadVertexArray->Bind();
		RenderCommand::DrawIndexed(data.quadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const glm::vec4& color) {
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const glm::vec4& color) {
		GOOE_PROFILE_FUNCTION();

		data.textureShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		data.textureShader->SetMat4("transform", transform);
		data.textureShader->SetFloat4("color", color);

		data.quadVertexArray->Bind();
		RenderCommand::DrawIndexed(data.quadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const Ref<Texture2D>& texture, const glm::vec4& tint, const float tilingFactor) {
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tint, tilingFactor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const Ref<Texture2D>& texture, const glm::vec4& tint, const float tilingFactor) {
		GOOE_PROFILE_FUNCTION();

		data.textureShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		data.textureShader->SetMat4("transform", transform);
		data.textureShader->SetFloat4("_tint", tint);
		data.textureShader->SetFloat("_tilingFactor", tilingFactor);

		texture->Bind();

		data.quadVertexArray->Bind();
		RenderCommand::DrawIndexed(data.quadVertexArray);
	}
}
