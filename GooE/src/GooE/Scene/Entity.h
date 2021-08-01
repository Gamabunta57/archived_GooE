#pragma once

#include <entt/entt.hpp>
#include "Scene.h"

namespace GooE {
	class Entity {
	public:
		Entity() = default;
		Entity(entt::entity entity, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			//GOOE_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			return scene->registry.emplace<T>(entityId, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent() {
			//GOOE_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have component!");
			return scene->registry.get<T>(entityId);
		}

		//template<typename T>
		//bool HasComponent() {
		//	return scene->registry.has<T>(entityId);
		//}

		template<typename T>
		void RemoveComponent() {
			scene->registry.remove<T>(entityId);
		}

		operator bool() const { 
			return entityId != entt::null;
		}

	private:
		entt::entity entityId{ entt::null };
		Scene* scene = nullptr;
	};
}
