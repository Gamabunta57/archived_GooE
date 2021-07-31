#include "gepch.h"

#include <glm/glm.hpp>
#include <GooE/Renderer/Renderer2D.h>

#include "Scene.h"
#include "Components.h"
#include "Entity.h"

namespace GooE {

	Scene::Scene() {}

	Scene::~Scene() {
	}

	Entity Scene::CreateEntity(const std::string& name) {
		Entity entity = { registry.create(), this };
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>(name.empty() 
			? "Entity"
			: name
		);

		return entity;
	}

	void Scene::OnUpdate(Timestep ts) {
		auto group = registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group) {
			auto& [transform, sprite]= group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawQuad(transform, sprite.color);
		}
	}


}