#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

#include <Platform/OpenGL/OpenGLShader.h>

#include "Sandbox2D.h"

static const uint32_t MAP_WIDTH = 24;
static const char* mapTiles =
"wwwwwwwwwwwwwwwwwwwwwwww"
"wwwwwwwdddddddwwwwwwwwww"
"wwwwwwddddddddddwwwwwwww"
"wwwwddddddddddddddwwwwww"
"wwdddddddddwddddddddwwww"
"wwdddddddwwwddddddddddww"
"wwdddddddwwvwwddddddddww"
"wdddddddddwwdddddddddwww"
"wdddddddddwddddddddwwwww"
"wwwdddddddddddddddwwwwww"
"wwwwwdddddddddddwwwwwwww"
"wwwwwwwwdddddwwwwwwwwwww"
"wwwwwwwwwwwwwwwwwwwwwwww"
"wwwwwwwwwwwwwwwwwwwwwwww"
;

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), cameraController(16.0f / 9.0f, true) {
}

void Sandbox2D::OnAttach() {
	GOOE_PROFILE_FUNCTION();

	mapWidth = MAP_WIDTH;
	mapHeight = strlen(mapTiles) / mapWidth;

	texture = GooE::Texture2D::Create("assets/textures/Checkerboard.png");
	spriteSheet = GooE::Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png");

	textureMap['w'] = GooE::SubTexture2D::CreateFromCoords(spriteSheet, { 11.0f, 11.0f }, { 128.0f, 128.0f });
	textureMap['d'] = GooE::SubTexture2D::CreateFromCoords(spriteSheet, { 6.0f, 11.0f }, { 128.0f, 128.0f });

	barrel = GooE::SubTexture2D::CreateFromCoords(spriteSheet, { 8.0f, 1.0f }, { 128.0f, 128.0f });

	GooE::FrameBufferSpecification fbSpec;
	fbSpec.width = 1280;
	fbSpec.height = 720;
	frameBuffer = GooE::FrameBuffer::Create(fbSpec);

	particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	particle.SizeBegin = 0.5f, particle.SizeVariation = 0.3f, particle.SizeEnd = 0.0f;
	particle.LifeTime = 1.0f;
	particle.Velocity = { 0.0f, 0.0f };
	particle.VelocityVariation = { 3.0f, 1.0f };
	particle.Position = { 0.0f, 0.0f };

	cameraController.SetZoomLevel(7.0f);
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
		frameBuffer->Bind();
		GooE::RenderCommand::Clear();
	}
	{
		static float rotation = 0.0f;
		rotation += ts * 2.0f;

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
		frameBuffer->Unbind();

		if (GooE::Input::IsMouseButtonPressed(GOOE_MOUSE_BUTTON_LEFT))
		{
			auto [x, y] = GooE::Input::GetMousePosition();
			auto width = GooE::Application::Get().GetWindow().GetWidth();
			auto height = GooE::Application::Get().GetWindow().GetHeight();

			auto bounds = cameraController.GetBounds();
			auto pos = cameraController.GetCamera().GetPosition();
			x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
			y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
			particle.Position = { x + pos.x, y + pos.y };
			for (int i = 0; i < 5; i++)
				particleSystem.Emit(particle);

		}
		particleSystem.OnUpdate(ts);
		particleSystem.OnRender(cameraController.GetCamera());
	}
}

void Sandbox2D::OnEvent(GooE::Event& e) {
	GOOE_PROFILE_FUNCTION();

	cameraController.OnEvent(e);
}

void Sandbox2D::OnImGuiRender() {
	GOOE_PROFILE_FUNCTION();
	
	static bool dockingEnabled = true;
	if (dockingEnabled) {
		static bool dockSpaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockSpaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::Separator();

				if (ImGui::MenuItem("Exit")) 
					GooE::Application::Get().Close();

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		auto stat = GooE::Renderer2D::GetStats();
		ImGui::Begin("Info");
		ImGui::Text("Renderer2D stats:");
		ImGui::Text("  Draw calls: %d", stat.drawCalls);
		ImGui::Text("  Quads: %d", stat.quadCount);
		ImGui::Text("  Vertices: %d", stat.GetTotalVertexCount());
		ImGui::Text("  Indices: %d", stat.GetTotalIndexCount());

		ImGui::ColorEdit4("Color", glm::value_ptr(color));
		uint32_t textureId = frameBuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureId, ImVec2{ 1280, 720 });
		ImGui::End();

		ImGui::End();
	}
}
