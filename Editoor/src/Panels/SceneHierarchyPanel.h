#pragma once

#include <GooE/Core/Core.h>
#include <GooE/Scene/Scene.h>
#include <GooE/Scene/Entity.h>

namespace GooE {
	class SceneHierarchyPanel {
		friend class Scene;

	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene> scene);

		void SetContext(const Ref<Scene> context);

		void OnImGuiRender();

	private:

		void DrawEntityNode(Entity entity);

	private:
		Ref<Scene> context;
		Entity selectionContext;
	};
}