#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

#include <Platform/OpenGL/OpenGLShader.h>

#include "Sandbox2D.h"

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), cameraController(16.0f / 9.0f, true) {
}

void Sandbox2D::OnAttach() {
	GOOE_PROFILE_FUNCTION();

	texture = GooE::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach() {
	GOOE_PROFILE_FUNCTION();

}

void Sandbox2D::OnUpdate(GooE::Timestep ts) {
	GOOE_PROFILE_FUNCTION();

	cameraController.OnUpdate(ts);

	{
		GOOE_PROFILE_SCOPE("Renderer clear");
		GooE::RenderCommand::Clear();
	}
	static float rotation = 0.0f;
	rotation += ts * 2.0f;
	{
		GOOE_PROFILE_SCOPE("Renderer draw");
		GooE::Renderer2D::BeginScene(cameraController.GetCamera());
		GooE::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, rotation, color);
		GooE::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, color);
		GooE::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.5f, 1.0f, 1.0f, 1.0f });
		GooE::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, glm::radians(-45.0f), texture, color, 10.0f);
		GooE::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, -0.01f }, { 1.0f, 1.0f }, glm::radians(-45.0f), texture, color, 10.0f);
		GooE::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnEvent(GooE::Event& e) {
	GOOE_PROFILE_FUNCTION();

	cameraController.OnEvent(e);
}

void Sandbox2D::OnImGuiRender() {
	GOOE_PROFILE_FUNCTION();
	ImGui::Begin("Color");
	ImGui::ColorEdit4("Color", glm::value_ptr(color));
	ImGui::End();
}
