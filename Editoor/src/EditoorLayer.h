#pragma once

#include <GooE.h>

namespace GooE {
	class EditoorLayer : public Layer {
	public:
		EditoorLayer();
		virtual ~EditoorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnEvent(Event& e) override;

		virtual void OnImGuiRender() override;

	private:

		uint32_t mapWidth, mapHeight;
		glm::vec2 viewportSize = { 0.0f, 0.0f };
		glm::vec4 color = { 0.4f, 0.2f, 0.8f, 1.0f };

		OrthographicCameraController cameraController;

		Ref<Texture2D> texture;
		Ref<Texture2D> spriteSheet;
		Ref<SubTexture2D> barrel;
		Ref<FrameBuffer> frameBuffer;

		struct ProfileResult {
			const char* name;
			float time;
		};

		std::unordered_map<char, Ref<SubTexture2D>> textureMap;
	};
}