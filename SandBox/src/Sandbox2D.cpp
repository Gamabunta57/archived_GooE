#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

#include <Platform/OpenGL/OpenGLShader.h>

#include "Sandbox2D.h"

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), cameraController(16.0f / 9.0f, true) {
}

void Sandbox2D::OnAttach() {
	vertexArray = GooE::VertexArray::Create();

	squareVertexArray = GooE::VertexArray::Create();
	float squareVertices[3 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	GooE::Ref<GooE::VertexBuffer> squareVB;
	squareVB.reset(GooE::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	squareVB->SetLayout({
		{ GooE::ShaderDataType::Float3, "position" },
	});
	squareVertexArray->AddVertexBuffer(squareVB);

	unsigned int squareIndices[6] = { 0, 1 ,2, 2, 3, 0 };
	GooE::Ref<GooE::IndexBuffer> squareIb;
	squareIb.reset(GooE::IndexBuffer::Create(squareIndices, sizeof(squareIndices)));
	squareVertexArray->SetIndexBuffer(squareIb);

	shader = GooE::Shader::Create("assets/shaders/flatColor.glsl");
}

void Sandbox2D::OnDetach() {}

void Sandbox2D::OnUpdate(GooE::Timestep ts) {
	cameraController.OnUpdate(ts);

	GooE::RenderCommand::Clear();
	GooE::Renderer::BeginScene(cameraController.GetCamera());

	shader->Bind();
	std::dynamic_pointer_cast<GooE::OpenGLShader>(shader)->UploadUniformFloat4("color", color);

	GooE::Renderer::Submit(shader, squareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
	GooE::Renderer::EndScene();
}

void Sandbox2D::OnEvent(GooE::Event& e) {
	cameraController.OnEvent(e);
}

void Sandbox2D::OnImGuiRender() {
	ImGui::Begin("Color");
	ImGui::ColorEdit3("Color", glm::value_ptr(color));
	ImGui::End();
}