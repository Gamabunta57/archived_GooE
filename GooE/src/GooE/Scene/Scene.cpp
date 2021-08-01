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

		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto group = registry.group<CameraComponent, TransformComponent>();
			for (auto entity : group) {
				auto& [camera, transform] = group.get<CameraComponent, TransformComponent>(entity);
				if (camera.primary) {
					mainCamera = &camera.camera;
					cameraTransform = &transform.transform;
					break;
				}
			}
		}

		if (mainCamera) {
			Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);
			{
				auto group = registry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
				for (auto entity : group) {
					auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

					Renderer2D::DrawQuad(transform, sprite.color);
				}
			}
			Renderer2D::EndScene();
		}
	}
}