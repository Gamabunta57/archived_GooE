#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

#include <Platform/OpenGL/OpenGLShader.h>

#include "EditoorLayer.h"

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
namespace GooE {
	EditoorLayer::EditoorLayer() : Layer("EditoorLayer"), cameraController(16.0f / 9.0f, true) {
	}

	void EditoorLayer::OnAttach() {
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

		cameraController.SetZoomLevel(7.0f);

		activeScene = CreateRef<Scene>();
		squareEntity = activeScene->CreateEntity("square");
		squareEntity.AddComponent<SpriteRendererComponent>(glm::vec4{0.2f, 0.8f, 0.2f, 1.0f});

		square2 = activeScene->CreateEntity("square 2");
		square2.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.8f, 0.2f, 0.8f, 1.0f });

		cameraEntity = activeScene->CreateEntity("camera");
		cameraEntity.AddComponent<CameraComponent>();
		cameraEntity.GetComponent<CameraComponent>().camera.SetProjectionType(SceneCamera::ProjectionType::Orthographic);

		secondCameraEntity = activeScene->CreateEntity("2nd camera");
		secondCameraEntity.AddComponent<CameraComponent>();
		secondCameraEntity.GetComponent<CameraComponent>().primary = false;

		class CameraController : public ScriptableEntity {
		public:
			void OnCreate() {
				auto& translation = GetComponent<TransformComponent>().translation;
				translation.x = rand() % 10 - 5.0f;
			}

			void OnDestroy() {

			}

			void OnUpdate(Timestep ts) {
				auto& translation = GetComponent<TransformComponent>().translation;
				float speed = 5.0f;

				if (Input::IsKeyPressed(GOOE_KEY_A))
					translation.x -= speed * ts;
				if (Input::IsKeyPressed(GOOE_KEY_D))
					translation.x += speed * ts;

				if (Input::IsKeyPressed(GOOE_KEY_W))
					translation.y += speed * ts;
				if (Input::IsKeyPressed(GOOE_KEY_S))
					translation.y -= speed * ts;
			}
		};

		cameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		secondCameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

		sceneHierarchyPanel.SetContext(activeScene);
	}

	void EditoorLayer::OnDetach() {
		GOOE_PROFILE_FUNCTION();
	}

	void EditoorLayer::OnUpdate(Timestep ts) {
		GOOE_PROFILE_FUNCTION();

		FrameBufferSpecification spec = frameBuffer->GetSpecification();
		if (viewportSize.x > 0.0f && viewportSize.y > 0.0f && (spec.width != viewportSize.x || spec.height != viewportSize.y)) {
			frameBuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
			cameraController.Resize(viewportSize.x, viewportSize.y);

			activeScene->SetViewportSize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		}

		if (viewportFocused)
			cameraController.OnUpdate(ts);

		GooE::Renderer2D::ResetStats();
		frameBuffer->Bind();
		GooE::RenderCommand::Clear();
	
		activeScene->OnUpdate(ts);

		frameBuffer->Unbind();
	}

	void EditoorLayer::OnEvent(Event& e) {
		GOOE_PROFILE_FUNCTION();

		cameraController.OnEvent(e);
	}

	void EditoorLayer::OnImGuiRender() {
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
			ImGuiStyle& style = ImGui::GetStyle();
			float minWindowSizeX = style.WindowMinSize.x;
			style.WindowMinSize.x = 370.0f;

			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			style.WindowMinSize.x = minWindowSizeX;

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

			sceneHierarchyPanel.OnImGuiRender();

			auto stat = GooE::Renderer2D::GetStats();
			ImGui::Begin("Info");
			ImGui::Text("Renderer2D stats:");
			ImGui::Text("  Draw calls: %d", stat.drawCalls);
			ImGui::Text("  Quads: %d", stat.quadCount);
			ImGui::Text("  Vertices: %d", stat.GetTotalVertexCount());
			ImGui::Text("  Indices: %d", stat.GetTotalIndexCount());

			ImGui::End();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
			ImGui::Begin("viewport");
			viewportFocused = ImGui::IsWindowFocused();
			viewportHovered = ImGui::IsWindowHovered();
			Application::Get().GetImGuiLayer()->SetBlockEvents(!viewportFocused || !viewportHovered);

			ImVec2 panelSize = ImGui::GetContentRegionAvail();
			viewportSize = { panelSize.x, panelSize.y };
			uint32_t textureId = frameBuffer->GetColorAttachmentRendererID();
			ImGui::Image((void*)textureId, { viewportSize.x, viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			ImGui::End();
			ImGui::PopStyleVar();
			ImGui::End();
		}
	}
}
