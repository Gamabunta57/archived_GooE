#pragma once

#include "glm/glm.hpp"

#include <GooE/Renderer/OrthographicCamera.h>
#include <GooE/Core/Timestep.h>
#include <GooE/Events/Event.h>
#include <GooE/Events/MouseEvent.h>
#include <GooE/Events/ApplicationEvent.h>

namespace GooE {

	struct OrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class OrthographicCameraController {
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return camera; }
		const OrthographicCamera& GetCamera() const { return camera; }

		float GetZoomLevel() const { return zoomLevel; }
		void SetZoomLevel(float zoomLevel) { this->zoomLevel = zoomLevel; CalculateView(); }

		const OrthographicCameraBounds& GetBounds() const { return bounds; }
	private:
		void CalculateView();

		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float aspectRatio;
		float zoomLevel = 1.0f;
		float zoomSpeed = 0.25f;
		bool rotation;
		float cameraTranslationSpeed = 5.0f, cameraRotationSpeed = 180.0f;
		float cameraRotation = 0.0f;
		OrthographicCameraBounds bounds;
		glm::vec3 cameraPosition = { 0.0f, 0.0f, 0.0f };

		OrthographicCamera camera;
	};
}
