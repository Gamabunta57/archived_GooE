#pragma once
#include <GooE/Core/Core.h>

namespace GooE {

	class Input {
	public:
		static bool IsKeyPressed(int keyCode);
		static bool IsMouseButtonPressed(int keyCode);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};

}
