#pragma once

#include <entt/entt.hpp>
#include <GooE/Core/Timestep.h>

namespace GooE {
	class Entity;

	class Scene {
		friend Entity;
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());

		void OnUpdate(Timestep ts);
		void SetViewportSize(uint32_t width, uint32_t height);

	private:
		entt::registry registry;
		uint32_t viewportWidth, viewportHeight;
	};
}
