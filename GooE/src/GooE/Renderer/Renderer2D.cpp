#include "gepch.h"

#include <Platform/OpenGL/OpenGLShader.h>

#include "RenderCommand.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"

namespace GooE {

	struct Renderer2DData {
		Ref<VertexArray> vertexArray;
		Ref<Shader> shader;
	};

	static Renderer2DData* data;

	void Renderer2D::Init() {
		data = new Renderer2DData();
		data->vertexArray = VertexArray::Create();

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		Ref<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "position" },
		});
		data->vertexArray->AddVertexBuffer(squareVB);

		unsigned int squareIndices[6] = { 0, 1 ,2, 2, 3, 0 };
		Ref<IndexBuffer> squareIb;
		squareIb.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices)));
		data->vertexArray->SetIndexBuffer(squareIb);

		data->shader = Shader::Create("assets/shaders/flatColor.glsl");
	}

	void Renderer2D::Shutdown() {
		delete data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		data->shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(data->shader)->UploadUniformMat4("viewProjection", camera.GetViewProjectionMatrix());
		std::dynamic_pointer_cast<OpenGLShader>(data->shader)->UploadUniformMat4("transform", glm::mat4(1.0f));
	}

	void Renderer2D::EndScene() {

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		data->shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(data->shader)->UploadUniformFloat4("color", color);

		data->vertexArray->Bind();
		RenderCommand::DrawIndexed(data->vertexArray);
	}
}
