#include "gepch.h"

#include <GooE/Core/KeyCodes.h>
#include <GooE/Core/Input.h>

#include "OrthographicCameraController.h"

namespace GooE {
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation) 
		: aspectRatio(aspectRatio), bounds({ -aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel }), camera(bounds.Left, bounds.Right, bounds.Bottom, bounds.Top), rotation(rotation) {
	}

	void OrthographicCameraController::OnUpdate(Timestep ts) {
		GOOE_PROFILE_FUNCTION();

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
		GOOE_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(GOOE_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
		dispatcher.Dispatch<MouseScrolledEvent>(GOOE_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e) {
		GOOE_PROFILE_FUNCTION();

		zoomLevel -= e.GetOffsetY() * zoomSpeed;
		zoomLevel = std::max(zoomLevel, 0.25f);

		CalculateView();
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e) {
		GOOE_PROFILE_FUNCTION();

		aspectRatio = (float)e.GetWidth() / (float)e.GetHeight();

		CalculateView();
		return false;
	}

	void OrthographicCameraController::CalculateView() {
		bounds = { -aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel };
		camera.SetProjection(bounds.Left, bounds.Right, bounds.Bottom, bounds.Top);
	}
}