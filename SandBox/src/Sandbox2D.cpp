#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

#include <Platform/OpenGL/OpenGLShader.h>

#include "Sandbox2D.h"

template<typename Fn>
class Timer {
public:
	Timer(const char* name, Fn&& function) : name(name), function(function), stopped(false) {
		startTimepoint = std::chrono::high_resolution_clock::now();
	}

	~Timer() {
		if (!stopped) Stop();
	}

	void Stop() {
		auto endTimepoint = std::chrono::high_resolution_clock::now();
		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(startTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		stopped = true;

		auto duration = (end - start) * 0.001f;
		std::cout << name << " duration: " << duration << "ms" << std::endl;
		function({ name, duration });
	}

private:
	const char* name;
	Fn function;
	std::chrono::time_point<std::chrono::steady_clock> startTimepoint;
	bool stopped;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) {profileResults.push_back(profileResult); })

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), cameraController(16.0f / 9.0f, true) {
}

void Sandbox2D::OnAttach() {
	texture = GooE::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach() {}

void Sandbox2D::OnUpdate(GooE::Timestep ts) {
	PROFILE_SCOPE("Sandbox2D::OnUpdate");
	{
		PROFILE_SCOPE("cameraController.OnUpdate");
		cameraController.OnUpdate(ts);
	}

	{
		PROFILE_SCOPE("Renderer clear");
		GooE::RenderCommand::Clear();
	}

	{
		PROFILE_SCOPE("Renderer draw");
		GooE::Renderer2D::BeginScene(cameraController.GetCamera());
		GooE::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, color);
		GooE::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.5f, 1.0f, 1.0f, 1.0f });
		GooE::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, texture, color);
		GooE::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnEvent(GooE::Event& e) {
	cameraController.OnEvent(e);
}

void Sandbox2D::OnImGuiRender() {
	ImGui::Begin("Color");
	ImGui::ColorEdit4("Color", glm::value_ptr(color));

	for (auto& result : profileResults) {
		char label[50];
		strcpy(label, "%.3fms ");
		strcat(label, result.name);
		ImGui::Text(label, result.time);
	}

	profileResults.clear();

	ImGui::End();
}
