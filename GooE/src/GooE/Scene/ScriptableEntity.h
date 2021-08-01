#pragma once

#include "Entity.h"

namespace GooE {
	class ScriptableEntity {
		friend class Scene;
	public:
		template<typename T>
		T& GetComponent() {
			return entity.GetComponent<T>();
		}

	private:
		Entity entity;
	};
}
