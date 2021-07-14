#include "gepch.h"

#include <GooE/Core/KeyCodes.h>
#include <GooE/Core/Input.h>

#include "OrthographicCameraController.h"

namespace GooE {
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation) 
		: aspectRatio(aspectRatio), camera(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel), rotation(rotation) {
	}

	void OrthographicCameraController::OnUpdate(Timestep ts) {
		if (Input::IsKeyPressed(GOOE_KEY_A))
			cameraPosition.x -= cameraTranslationSpeed * ts;
		else if (Input::IsKeyPressed(GOOE_KEY_D))
			cameraPosition.x += cameraTranslationSpeed * ts;

		if (Input::IsKeyPressed(GOOE_KEY_W))
			cameraPosition.y += cameraTranslationSpeed * ts;
		else if (Input::IsKeyPressed(GOOE_KEY_S))
			cameraPosition.y -= cameraTranslationSpeed * ts;

		if (rotation) {
			if (Input::IsKeyPressed(GOOE_KEY_Q))
				cameraRotation += cameraRotationSpeed * ts;
			else if (Input::IsKeyPressed(GOOE_KEY_E))
				cameraRotation -= cameraRotationSpeed * ts;

			if (cameraRotation > 180.0f)
				cameraRotation -= 360.0f;
			else if (cameraRotation < -180.0f)
				cameraRotation += 360.0f;

			camera.SetRotation(cameraRotation);
		}

		camera.SetPosition(cameraPosition);
		cameraTranslationSpeed = zoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(GOOE_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
		dispatcher.Dispatch<MouseScrolledEvent>(GOOE_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e) {
		zoomLevel -= e.GetOffsetY() * zoomSpeed;
		zoomLevel = std::max(zoomLevel, 0.25f);
		camera.SetProjection(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e) {
		aspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		camera.SetProjection(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel);
		return false;
	}
}