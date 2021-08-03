#pragma once

#include <GooE/Core/Log.h>
#include <GooE/Core/Core.h>
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
			GOOE_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			T& component = scene->registry.emplace<T>(entityId, std::forward<Args>(args)...);
			scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& GetComponent() {
			GOOE_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have component!");
			return scene->registry.get<T>(entityId);
		}

		template<typename T>
		bool HasComponent() {
			return scene->registry.any_of<T>(entityId);
		}

		template<typename T>
		void RemoveComponent() {
			scene->registry.remove<T>(entityId);
		}

		operator bool() const { return entityId != entt::null; }
		operator uint32_t() const { return (uint32_t)entityId; }
		operator entt::entity() const { return entityId; }
		bool operator==(const Entity& other) const { return entityId == other.entityId && scene == other.scene; }
		bool operator!=(const Entity& other) const { return !(*this == other); }

	private:
		entt::entity entityId{ entt::null };
		Scene* scene = nullptr;
	};
}
