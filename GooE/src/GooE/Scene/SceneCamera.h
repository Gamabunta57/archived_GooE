#pragma once
#include <GooE/Renderer/Camera.h>

namespace GooE {
	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetViewportSize(uint32_t width, uint32_t height);

		float GetOrthographicSize() const { return orthographicSize; }
		void SetOrthographicSize(float orthographicSize) { this->orthographicSize = orthographicSize; RecalculateProjection(); }
	private:
		void RecalculateProjection();

	private:
		float orthographicSize = 10.0f;
		float orthographicNear = -1.0f, orthographicFar = 1.0f;

		float aspectRatio = 0.0f;
	};
}

