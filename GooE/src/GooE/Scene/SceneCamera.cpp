#include "gepch.h"

#include <glm/gtc/matrix_transform.hpp>

#include "SceneCamera.h"

namespace GooE {
	SceneCamera::SceneCamera() {
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip) {
		projectionType = ProjectionType::Orthographic;
		orthographicSize = size;
		orthographicNear = nearClip;
		orthographicFar = farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float verticalFov, float nearClip, float farClip) {
		projectionType = ProjectionType::Perspective;
		this->verticalFov = verticalFov;
		perspectiveNear = nearClip;
		perspectiveFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height) {
		aspectRatio = (float)width / (float)height;

		RecalculateProjection();
	}
	
	void SceneCamera::RecalculateProjection() {
		if (projectionType == ProjectionType::Perspective)
			projection = glm::perspective(verticalFov, aspectRatio, perspectiveNear, perspectiveFar);
		else {
			float orthoLeft = -orthographicSize * aspectRatio * 0.5f;
			float orthoRight = orthographicSize * aspectRatio * 0.5f;
			float orthoBottom = -orthographicSize * 0.5f;
			float orthoTop = orthographicSize * 0.5f;

			projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, orthographicNear, orthographicFar);
		}
	}
}
