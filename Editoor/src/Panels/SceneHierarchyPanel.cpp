#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <GooE/Scene/Components.h>

namespace GooE {
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene> scene) {
		SetContext(scene);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene> context) {
		this->context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender() {
		ImGui::Begin("Scene hierarchy");
		context->registry.each([&](auto entityId) {
			Entity entity{ entityId, context.get() };
			DrawEntityNode(entity);			
		});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			selectionContext = {};

		ImGui::End();

		ImGui::Begin("Properties");
		if (selectionContext) {
			DrawComponents(selectionContext);
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity) {
		auto& tag = entity.GetComponent<TagComponent>().tag;

		ImGuiTreeNodeFlags flags = (selectionContext == entity ? ImGuiTreeNodeFlags_Selected : 0 )| ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked())
			selectionContext = entity;

		if (opened)
			ImGui::TreePop();
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity) {
		if (entity.HasComponent<TagComponent>()) {
			auto& tag = entity.GetComponent<TagComponent>().tag;

			char buffer[128];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, 128, tag.c_str());

			if (ImGui::InputText("Tag", buffer, sizeof(buffer))) {
				tag = std::string(buffer);
			}
		}

		if (entity.HasComponent<TransformComponent>()) {
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {
				auto& transform = entity.GetComponent<TransformComponent>().transform;
				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);

				ImGui::TreePop();
			}
		}
	}
}