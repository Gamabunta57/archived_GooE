#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

#include <Platform/OpenGL/OpenGLShader.h>

#include <GooE/Utils/PlatformUtils.h>
#include <GooE/Scene/SceneSerializer.h>
#include <GooE/Math/Math.h>

#include <ImGuizmo.h>

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
		sceneHierarchyPanel.SetContext(activeScene);

		//TMP: NatvieScript Example
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

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(GOOE_BIND_EVENT_FN(EditoorLayer::OnKeyPressed));
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
					if (ImGui::MenuItem("New", "Ctrl+N"))
						NewScene();

					if (ImGui::MenuItem("Open...", "Ctrl+O"))
						OpenScene();

					if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
						SaveSceneAs();

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
			Application::Get().GetImGuiLayer()->SetBlockEvents(!viewportFocused && !viewportHovered);

			ImVec2 panelSize = ImGui::GetContentRegionAvail();
			viewportSize = { panelSize.x, panelSize.y };
			uint32_t textureId = frameBuffer->GetColorAttachmentRendererID();
			ImGui::Image((void*)textureId, { viewportSize.x, viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			//Guizmos
			Entity selectedEntity = sceneHierarchyPanel.GetSelectedEntity();
			if (selectedEntity && guizmoType > -1) {
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();

				float windowWidth = (float)ImGui::GetWindowWidth();
				float windowHeight = (float)ImGui::GetWindowHeight();
				auto& position = ImGui::GetWindowPos();
				ImGuizmo::SetRect(position.x, position.y, windowWidth, windowHeight);

				auto cameraEntity = activeScene->GetPrimaryCameraEntity();
				const auto& camera = cameraEntity.GetComponent<CameraComponent>().camera;
				const glm::mat4& cameraProjection = camera.GetProjection();
				glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

				auto& tc = selectedEntity.GetComponent<TransformComponent>();
				glm::mat4 transform = tc.GetTransform();
				glm::vec3 originalRotation = tc.rotation;

				//snapping
				bool snap = Input::IsKeyPressed(GOOE_KEY_LEFT_CONTROL) || Input::IsKeyPressed(GOOE_KEY_RIGHT_CONTROL);
				float snapValue = 0.5f;
				if (guizmoType == ImGuizmo::OPERATION::ROTATE)
					snapValue = 5.0f;

				float snapValues[3] = { snapValue, snapValue, snapValue };

				ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), (ImGuizmo::OPERATION)guizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);

				if (ImGuizmo::IsUsing()) {
					glm::vec3 translation, rotation, scale;
					Math::decomposeTransform(transform, translation, rotation, scale);

					glm::vec3 deltaRotation = rotation - tc.rotation;

					tc.translation = translation;
					tc.rotation += deltaRotation;
					tc.scale = scale;
				}
			}

			ImGui::End();
			ImGui::PopStyleVar();
			ImGui::End();
		}
	}

	bool EditoorLayer::OnKeyPressed(const KeyPressedEvent& e) {
		if (e.GetRepeatCount() > 0) return false;

		bool ctrl = Input::IsKeyPressed(GOOE_KEY_LEFT_CONTROL) || Input::IsKeyPressed(GOOE_KEY_RIGHT_CONTROL);
		bool shift = Input::IsKeyPressed(GOOE_KEY_LEFT_SHIFT) || Input::IsKeyPressed(GOOE_KEY_RIGHT_SHIFT);

		switch (e.GetKeyCode()) {
			case GOOE_KEY_N: {
				if (ctrl) 
					NewScene();
				break;
			}
			case GOOE_KEY_O: {
				if (ctrl) 
					OpenScene();
				break;
			}
			case GOOE_KEY_S: {
				if (ctrl && shift) 
					SaveSceneAs();
				break;
			}

			//Guizmo
			case GOOE_KEY_Q: {
				if (!ctrl && !shift)
					guizmoType = -1;
				break;
			}
			case GOOE_KEY_W: {
				if (!ctrl && !shift)
					guizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			}
			case GOOE_KEY_E: {
				if (!ctrl && !shift)
					guizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			}
			case GOOE_KEY_R: {
				if (!ctrl && !shift) 
					guizmoType = ImGuizmo::OPERATION::SCALE;
				break;
			}
		}
	}

	void EditoorLayer::NewScene() {
		activeScene = CreateRef<Scene>();
		activeScene->SetViewportSize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		sceneHierarchyPanel.SetContext(activeScene);
	}

	void EditoorLayer::OpenScene() {
		std::string path = FileDialogs::OpenFile("Hazel Scene (*.gooe)\0*.gooe\0All Files (*.*)\0*.*\0");
		if (!path.empty()) {
			activeScene = CreateRef<Scene>();
			activeScene->SetViewportSize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
			sceneHierarchyPanel.SetContext(activeScene);

			SceneSerializer serializer(activeScene);
			serializer.Deserialize(path);
		}
	}

	void EditoorLayer::SaveSceneAs() {
		std::string path = FileDialogs::SaveFile("Hazel Scene (*.gooe)\0*.gooe\0");
		if (!path.empty()) {
			SceneSerializer serializer(activeScene);
			serializer.Serialize(path);
		}
	}
}
