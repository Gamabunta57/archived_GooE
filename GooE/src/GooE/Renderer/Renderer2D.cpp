#include "gepch.h"

#include <glm/gtc/matrix_transform.hpp>

#include "RenderCommand.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"

namespace GooE {

	struct Renderer2DData {
		Ref<VertexArray> vertexArray;
		Ref<Shader> shader;
		Ref<Shader> textureShader;
	};

	static Renderer2DData* data;

	void Renderer2D::Init() {
		GOOE_PROFILE_FUNCTION();

		data = new Renderer2DData();
		data->vertexArray = VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		};

		Ref<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "position" },
			{ ShaderDataType::Float2, "texCoords" },
		});
		data->vertexArray->AddVertexBuffer(squareVB);

		unsigned int squareIndices[6] = { 0, 1 ,2, 2, 3, 0 };
		Ref<IndexBuffer> squareIb;
		squareIb.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices)));
		data->vertexArray->SetIndexBuffer(squareIb);

		data->shader = Shader::Create("assets/shaders/flatColor.glsl");
		data->textureShader = Shader::Create("assets/shaders/texture.glsl");
		data->textureShader->Bind();
		data->textureShader->SetInt("_texture", 0);
	}

	void Renderer2D::Shutdown() {
		GOOE_PROFILE_FUNCTION();

		delete data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		GOOE_PROFILE_FUNCTION();

		data->shader->Bind();
		data->shader->SetMat4("viewProjection", camera.GetViewProjectionMatrix());

		data->textureShader->Bind();
		data->textureShader->SetMat4("viewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene() {

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		GOOE_PROFILE_FUNCTION();

		data->shader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		data->shader->SetMat4("transform", transform);
		data->shader->SetFloat4("color", color);

		data->vertexArray->Bind();
		RenderCommand::DrawIndexed(data->vertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint, const float tilingFactor) {
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tint, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint, const float tilingFactor) {
		GOOE_PROFILE_FUNCTION();

		data->textureShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		data->textureShader->SetMat4("transform", transform);
		data->textureShader->SetFloat4("_tint", tint);
		data->textureShader->SetFloat("_tilingFactor", tilingFactor);

		texture->Bind();

		data->vertexArray->Bind();
		RenderCommand::DrawIndexed(data->vertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const glm::vec4& color) {
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const glm::vec4& color) {
		GOOE_PROFILE_FUNCTION();

		data->shader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		data->shader->SetMat4("transform", transform);
		data->shader->SetFloat4("color", color);

		data->vertexArray->Bind();
		RenderCommand::DrawIndexed(data->vertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const Ref<Texture2D>& texture, const glm::vec4& tint, const float tilingFactor) {
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tint, tilingFactor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const Ref<Texture2D>& texture, const glm::vec4& tint, const float tilingFactor) {
		GOOE_PROFILE_FUNCTION();

		data->textureShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		data->textureShader->SetMat4("transform", transform);
		data->textureShader->SetFloat4("_tint", tint);
		data->textureShader->SetFloat("_tilingFactor", tilingFactor);

		texture->Bind();

		data->vertexArray->Bind();
		RenderCommand::DrawIndexed(data->vertexArray);
	}
}