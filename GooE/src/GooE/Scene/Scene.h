#pragma once

#include <entt/entt.hpp>
#include <GooE/Core/Timestep.h>

namespace GooE {
	class Entity;

	class Scene {
		friend class Entity;
		friend class SceneHierarchyPanel;
	public:
		Scene();
		~Scene() = default;

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnUpdate(Timestep ts);
		void SetViewportSize(uint32_t width, uint32_t height);

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		entt::registry registry;
		uint32_t viewportWidth, viewportHeight;
	};
}
