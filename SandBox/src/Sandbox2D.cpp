#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

#include <Platform/OpenGL/OpenGLShader.h>

#include "Sandbox2D.h"

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), cameraController(16.0f / 9.0f, true) {
}

void Sandbox2D::OnAttach() {
	texture = GooE::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach() {}

void Sandbox2D::OnUpdate(GooE::Timestep ts) {
	cameraController.OnUpdate(ts);

	GooE::RenderCommand::Clear();

	GooE::Renderer2D::BeginScene(cameraController.GetCamera());
	GooE::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, color);
	GooE::Renderer2D::DrawQuad({ 0.5f, -0.5f}, { 0.5f, 0.75f }, {0.5f, 1.0f, 1.0f, 1.0f});
	GooE::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, texture, color);
	GooE::Renderer2D::EndScene();
}

void Sandbox2D::OnEvent(GooE::Event& e) {
	cameraController.OnEvent(e);
}

void Sandbox2D::OnImGuiRender() {
	ImGui::Begin("Color");
	ImGui::ColorEdit4("Color", glm::value_ptr(color));
	ImGui::End();
}
