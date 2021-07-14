#pragma once

#include <GooE.h>

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
	GooE::Ref<GooE::Shader> shader;
	GooE::OrthographicCameraController cameraController;

	GooE::Ref<GooE::VertexArray> vertexArray;
	GooE::Ref<GooE::VertexArray> squareVertexArray;


	glm::vec3 squarePosition;
	glm::vec4 color = { 0.4f, 0.2f, 0.8f, 1.0f };
};