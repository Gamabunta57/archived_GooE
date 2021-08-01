#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <GooE/Scene/Components.h>

namespace GooE {
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene> scene) {
		SetContext(scene);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene> context) {
		this->context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender() {
		ImGui::Begin("Scene hierachy");
		context->registry.each([&](auto entityId) {
			Entity entity{ entityId, context.get() };
			DrawEntityNode(entity);			
		});
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
}