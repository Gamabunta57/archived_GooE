#include "gepch.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <GooE/Core/Input.h>
#include <GooE/Core/KeyCodes.h>

#include "EditorCamera.h"

namespace GooE {
	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: fov(fov), aspectRatio(aspectRatio), nearClip(nearClip), farClip(farClip), Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip)) {
		UpdateView();
	}

	void EditorCamera::UpdateProjection() {
		aspectRatio = viewportWidth / viewportHeight;
		projection = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
	}

	void EditorCamera::UpdateView() {
		position = CalculatePosition();

		glm::quat orientation = GetOrientation();
		viewMatrix = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(orientation);
		viewMatrix = glm::inverse(viewMatrix);
	}

	std::pair<float, float> EditorCamera::PanSpeed() const {
		float x = std::min(viewportWidth / 1000.0f, 2.4f);
		float xFactor = 0.3366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(viewportHeight / 1000.0f, 2.4f);
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::RotationSpeed() const {
		return 0.8f;
	}

	float EditorCamera::ZoomSpeed() const {
		float distance = this->distance * 0.2f;
		distance = std::max(distance, 0.0f);

		float speed = distance * distance;
		speed = std::min(speed, 100.0f);
		return speed;
	}

	void EditorCamera::OnUpdate(Timestep ts) {
		if (Input::IsKeyPressed(GOOE_KEY_LEFT_ALT)) {
			const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
			glm::vec2 delta = (mouse - initialMousePosition) * 0.003f;
			initialMousePosition = mouse;

			if (Input::IsMouseButtonPressed(GOOE_MOUSE_BUTTON_MIDDLE))
				MousePan(delta);

			else if (Input::IsMouseButtonPressed(GOOE_MOUSE_BUTTON_LEFT))
				MouseRotate(delta);

			else if (Input::IsMouseButtonPressed(GOOE_MOUSE_BUTTON_RIGHT))
				MouseZoom(delta.y);
		}

		UpdateView();
	}

	void EditorCamera::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(GOOE_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e) {
		float delta = e.GetOffsetY() * 0.1f;
		MouseZoom(delta);
		UpdateView();
		return false;
	}

	void EditorCamera::MousePan(const glm::vec2& delta) {
		auto [xSpeed, ySpeed] = PanSpeed();
		focalPoint += -GetRightDirection() * delta.x * xSpeed * distance;
		focalPoint += -GetUpDirection() * delta.y * ySpeed * distance;
	}

	void EditorCamera::MouseRotate(const glm::vec2& delta) {
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		yaw += yawSign * delta.x * RotationSpeed();
		pitch += delta.y * RotationSpeed();
	}

	void EditorCamera::MouseZoom(float delta) {
		distance -= delta * ZoomSpeed();
		if (distance < 1.0f) {
			focalPoint += GetForwardDirection();
			distance = 1.0f;
		}
	}

	glm::vec3 EditorCamera::GetUpDirection() const {
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetRightDirection() const {
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetForwardDirection() const {
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::vec3 EditorCamera::CalculatePosition() const {
		return focalPoint - GetForwardDirection() * distance;
	}

	glm::quat EditorCamera::GetOrientation() const {
		return glm::quat(glm::vec3(-pitch, -yaw, 0.0f));
	}
}