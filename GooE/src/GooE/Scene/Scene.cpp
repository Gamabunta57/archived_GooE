#include "gepch.h"

#include <glm/glm.hpp>
#include <GooE/Renderer/Renderer2D.h>

#include "Scene.h"
#include "Components.h"


namespace GooE {

	Scene::Scene() {}

	Scene::~Scene() {
	}

	entt::entity Scene::CreateEntity() {
		return registry.create();
	}

	void Scene::OnUpdate(Timestep ts) {
		auto group = registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group) {
			auto& [transform, sprite]= group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawQuad(transform, sprite.color);
		}
	}


}