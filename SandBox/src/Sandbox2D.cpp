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

	GooE::Renderer2D::ResetStats();

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
		GooE::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, glm::radians(-45.0f), texture, color, 10.0f);
		GooE::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, -0.01f }, { 1.0f, 1.0f }, glm::radians(-45.0f), texture, color, 10.0f);
		GooE::Renderer2D::EndScene();

		GooE::Renderer2D::BeginScene(cameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f) {
			for (float x = -5.0f; x < 5.0f; x += 0.5f) {
				glm::vec4 rb = { (x + 5.0f) / 10.0f, 0.5f, (y + 5.0f) / 10.0f, 0.7f};
				GooE::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, rb);
			}
		}

		GooE::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnEvent(GooE::Event& e) {
	GOOE_PROFILE_FUNCTION();

	cameraController.OnEvent(e);
}

void Sandbox2D::OnImGuiRender() {
	GOOE_PROFILE_FUNCTION();

	auto stat = GooE::Renderer2D::GetStats();

	ImGui::Begin("Info");
	ImGui::Text("Renderer2D stats:");
	ImGui::Text("  Draw calls: %d", stat.drawCalls);
	ImGui::Text("  Quads: %d", stat.quadCount);
	ImGui::Text("  Vertices: %d", stat.GetTotalVertexCount());
	ImGui::Text("  Indices: %d", stat.GetTotalIndexCount());

	ImGui::ColorEdit4("Color", glm::value_ptr(color));
	ImGui::End();
}
