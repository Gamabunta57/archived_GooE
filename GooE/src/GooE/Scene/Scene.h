#pragma once

#include <entt/entt.hpp>
#include <GooE/Core/Timestep.h>

namespace GooE {
	class Scene {
	public:
		Scene();
		~Scene();

		entt::entity CreateEntity();

		void OnUpdate(Timestep ts);

		//TMP
		entt::registry& GetRegistry() { return registry; }

	private:
		entt::registry registry;
	};
}
