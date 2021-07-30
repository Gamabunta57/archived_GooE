#pragma once

#include <GooE.h>
#include "ParticleSystem.h"

class Sandbox2D : public GooE::Layer {
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(GooE::Timestep ts) override;
	virtual void OnEvent(GooE::Event& e) override;

	virtual void OnImGuiRender() override;

private:
	GooE::OrthographicCameraController cameraController;

	GooE::Ref<GooE::Texture2D> texture;
	GooE::Ref<GooE::Texture2D> spriteSheet;
	GooE::Ref<GooE::SubTexture2D> stairs;
	GooE::Ref<GooE::SubTexture2D> barrel;
	GooE::Ref<GooE::SubTexture2D> tree;

	struct ProfileResult {
		const char* name;
		float time;
	};

	glm::vec4 color = { 0.4f, 0.2f, 0.8f, 1.0f };

	ParticleProps particle;
	ParticleSystem particleSystem;
};
