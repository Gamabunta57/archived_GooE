#include "gepch.h"

#include <glm/glm.hpp>
#include <GooE/Renderer/Renderer2D.h>

#include "Scene.h"
#include "Components.h"
#include "Entity.h"

namespace GooE {

	Scene::Scene() {}

	Entity Scene::CreateEntity(const std::string& name) {
		Entity entity = { registry.create(), this };
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>(name.empty() 
			? "Entity"
			: name
		);

		return entity;
	}

	void Scene::DestroyEntity(Entity entity) {
		registry.destroy(entity);
	}

	void Scene::OnUpdate(Timestep ts) {
		//Update scripts
		{
			registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
				//TODO: move to Scene::OnScenePlay
				if (!nsc.instance){
					nsc.instance = nsc.InstantiateScript();
					nsc.instance->entity = Entity{ entity, this };

					nsc.instance->OnCreate();
				}

				nsc.instance->OnUpdate(ts);
			});
		}

		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto group = registry.group<CameraComponent, TransformComponent>();
			for (auto entity : group) {
				auto [camera, transform] = group.get<CameraComponent, TransformComponent>(entity);
				if (camera.primary) {
					mainCamera = &camera.camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		if (mainCamera) {
			Renderer2D::BeginScene(mainCamera->GetProjection(), cameraTransform);
			{
				auto group = registry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
				for (auto entity : group) {
					auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

					Renderer2D::DrawQuad(transform.GetTransform(), sprite.color);
				}
			}
			Renderer2D::EndScene();
		}
	}

	void Scene::SetViewportSize(uint32_t width, uint32_t height) {
		viewportWidth = width;
		viewportHeight = height;

		auto view = registry.view<CameraComponent>();
		for (auto entity : view) {
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.fixedAspectRatio) {
				cameraComponent.camera.SetViewportSize(width, height);
			}
		}
	}

	Entity Scene::GetPrimaryCameraEntity() {
		auto view = registry.view<CameraComponent>();
		for (auto entity : view) {
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.primary)
				return Entity(entity, this);
		}

		return {};
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component) {
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component) { }

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component) {
		component.camera.SetViewportSize(viewportWidth, viewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component) {}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component) {}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component) {}
}