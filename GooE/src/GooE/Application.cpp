#include "gepch.h"

#include <GooE/Renderer/Renderer.h>
#include <GooE/Renderer/RenderCommand.h>

#include "Application.h"
#include "Input.h"

namespace GooE {
	Application* Application::instance = nullptr;

	Application::Application() {
		GOOE_CORE_ASSERT(!instance, "Application already exists!");
		instance = this;

		window = std::unique_ptr<Window>(Window::Create());
		window->SetEventCallback(GOOE_BIND_EVENT_FN(Application::OnEvent));

		imguiLayer = new ImGuiLayer();
		PushOverlay(imguiLayer);

		vertexArray.reset(VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.5f, 0.1f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.5f, 0.8f, 0.1f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.1f, 0.5f, 0.8f, 1.0f,
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		BufferLayout layout = {
			{ ShaderDataType::Float3, "position" },
			{ ShaderDataType::Float4, "color" },
		};

		vertexBuffer->SetLayout(layout);
		vertexArray->AddVertexBuffer(vertexBuffer);

		unsigned int indices[3] = { 0, 1 ,2 };
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, 3));
		vertexArray->SetIndexBuffer(indexBuffer);

		squareVertexArray.reset(VertexArray::Create());
		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "position" }
		});
		squareVertexArray->AddVertexBuffer(squareVB);

		unsigned int squareIndices[6] = { 0, 1 ,2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIb;
		squareIb.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices)));
		squareVertexArray->SetIndexBuffer(squareIb);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 position;
			layout(location = 1) in vec4 color;
			out vec3 vPosition;
			out vec4 vColor;
			
			void main() {
				vPosition = position;
				vColor = color;
				gl_Position = vec4(position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			in vec3 vPosition;
			in vec4 vColor;

			void main() {
				color = vec4(vPosition * 0.5 + 0.5, 1.0);
				color = vColor;
			}
		)";
		
		std::string vertexSrc2 = R"(
			#version 330 core

			layout(location = 0) in vec3 position;

			out vec3 vPosition;
			
			void main() {
				vPosition = position;
				gl_Position = vec4(position, 1.0);
			}
		)";

		std::string fragmentSrc2 = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			in vec3 vPosition;

			void main() {
				color = vec4(vPosition * 0.5 + 0.5, 1.0);
			}
		)";

		squareShader.reset(new Shader(vertexSrc2, fragmentSrc2));
		shader.reset(new Shader(vertexSrc, fragmentSrc));
	}

	Application::~Application() {
	}

	void Application::Run() {
		RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });

		while (isRunning) {
			RenderCommand::Clear();

			Renderer::BeginScene();

			squareShader->Bind();
			Renderer::Submit(squareVertexArray);

			shader->Bind();
			Renderer::Submit(vertexArray);

			Renderer::EndScene();

			for (Layer* layer : layerStack)
				layer->OnUpdate();

			imguiLayer->Begin();
			for (Layer* layer : layerStack)
				layer->OnImGuiRender();
			imguiLayer->End();

			window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(GOOE_BIND_EVENT_FN(Application::OnWindowClose));

		GOOE_CORE_TRACE("{0}", e);

		for (auto it = layerStack.end(); it != layerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.isHandled) break;
		}
	}

	void Application::PushLayer(Layer* layer) {
		layerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer) {
		layerStack.PushOverlay(layer);
	}

	bool Application::OnWindowClose(WindowCloseEvent &e) {
		isRunning = false;
		return true;
	}
}
