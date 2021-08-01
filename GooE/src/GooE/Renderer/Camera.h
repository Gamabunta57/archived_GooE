#pragma once

#include <glm/glm.hpp>

namespace GooE {
	class Camera {
	public:
		Camera(const glm::mat4& projection) : projection(projection) {}

		const glm::mat4& GetProjection() const { return projection; }

	private:
		glm::mat4 projection;
	};
}