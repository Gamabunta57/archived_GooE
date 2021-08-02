#pragma once
#include <GooE/Renderer/Camera.h>

namespace GooE {
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType {
			Perspective = 0,
			Orthographic = 1
		};

	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetPerspective(float verticalFoc, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);

		float GetPerspectiveVerticalFov() const { return verticalFov; }
		void SetPerspectiveVerticalFov(float fov) { this->verticalFov = fov; RecalculateProjection(); }

		float GetPerspectiveNearClip() const { return perspectiveNear; }
		void SetPerspectiveNearClip(float perspectiveNear) { this->perspectiveNear = perspectiveNear; RecalculateProjection(); }

		float GetPerspectiveFarClip() const { return perspectiveFar; }
		void SetPerspectiveFarClip(float perspectiveFar) { this->perspectiveFar = perspectiveFar; RecalculateProjection(); }

		float GetOrthographicSize() const { return orthographicSize; }
		void SetOrthographicSize(float orthographicSize) { this->orthographicSize = orthographicSize; RecalculateProjection(); }

		float GetOrthographicNearClip() const { return orthographicNear; }
		void SetOrthographicNearClip(float orthographicNear) { this->orthographicNear = orthographicNear; RecalculateProjection(); }

		float GetOrthographicFarClip() const { return orthographicFar; }
		void SetOrthographicFarClip(float orthographicFar) { this->orthographicFar = orthographicFar; RecalculateProjection(); }

		ProjectionType GetProjectionType() const { return projectionType; }
		void SetProjectionType(ProjectionType projectionType) { this->projectionType = projectionType; RecalculateProjection(); }

	private:
		void RecalculateProjection();

	private:
		ProjectionType projectionType = ProjectionType::Perspective;

		float orthographicSize = 10.0f;
		float orthographicNear = -1.0f, orthographicFar = 1.0f;

		float verticalFov = glm::radians(45.0f);
		float perspectiveNear = 0.1f, perspectiveFar = 1000.0f;

		float aspectRatio = 0.0f;
	};
}

