#pragma once

#include "Entity.h"

namespace GooE {
	class ScriptableEntity {
		friend class Scene;
	public:
		virtual ~ScriptableEntity() = default;

		template<typename T>
		T& GetComponent() {
			return entity.GetComponent<T>();
		}

	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep ts) {}

	private:
		Entity entity;
	};
}
