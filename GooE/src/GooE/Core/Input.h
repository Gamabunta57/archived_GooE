#pragma once
#include <GooE/Core/Core.h>

namespace GooE {

	class GOOE_API Input {
	public:
		inline static bool IsKeyPressed(int keyCode) { return instance->IsKeyPressedImpl(keyCode); }
		inline static bool IsMouseButtonPressed(int keyCode) { return instance->IsMouseButtonPressedImpl(keyCode); }
		inline static std::pair<float, float> GetMousePosition() { return instance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return instance->GetMouseYImpl(); }

	protected:
		virtual bool IsKeyPressedImpl(int keyCode) = 0;
		virtual bool IsMouseButtonPressedImpl(int keyCode) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

	private:
		static Input* instance;
	};

}
