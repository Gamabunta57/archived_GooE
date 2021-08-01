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
		//Update scripts
		{
			registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
				if (!nsc.instance){
					nsc.InstantiateFunction();
					nsc.instance->entity = Entity{ entity, this };

					nsc.OnCreateFunction(nsc.instance);
				}

				nsc.OnUpdateFunction(nsc.instance, ts);
			});
		}

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
}