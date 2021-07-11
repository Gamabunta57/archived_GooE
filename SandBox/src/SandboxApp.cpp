#include <imgui/imgui.h>

#include <GooE.h>

class TestLayer : public GooE::Layer {
public:
    TestLayer() : Layer("Test"), camera(-1.6f, 1.6f, -0.9f, 0.9f) {
		vertexArray.reset(GooE::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.5f, 0.1f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.5f, 0.8f, 0.1f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.1f, 0.5f, 0.8f, 1.0f,
		};

		std::shared_ptr<GooE::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(GooE::VertexBuffer::Create(vertices, sizeof(vertices)));
		GooE::BufferLayout layout = {
			{ GooE::ShaderDataType::Float3, "position" },
			{ GooE::ShaderDataType::Float4, "color" },
		};

		vertexBuffer->SetLayout(layout);
		vertexArray->AddVertexBuffer(vertexBuffer);

		unsigned int indices[3] = { 0, 1 ,2 };
		std::shared_ptr<GooE::IndexBuffer> indexBuffer;
		indexBuffer.reset(GooE::IndexBuffer::Create(indices, 3));
		vertexArray->SetIndexBuffer(indexBuffer);

		squareVertexArray.reset(GooE::VertexArray::Create());
		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		std::shared_ptr<GooE::VertexBuffer> squareVB;
		squareVB.reset(GooE::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ GooE::ShaderDataType::Float3, "position" }
		});
		squareVertexArray->AddVertexBuffer(squareVB);

		unsigned int squareIndices[6] = { 0, 1 ,2, 2, 3, 0 };
		std::shared_ptr<GooE::IndexBuffer> squareIb;
		squareIb.reset(GooE::IndexBuffer::Create(squareIndices, sizeof(squareIndices)));
		squareVertexArray->SetIndexBuffer(squareIb);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 position;
			layout(location = 1) in vec4 color;

			uniform mat4 viewProjection;

			out vec3 vPosition;
			out vec4 vColor;
			
			void main() {
				vPosition = position;
				vColor = color;
				gl_Position = viewProjection * vec4(position, 1.0);
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

			uniform mat4 viewProjection;

			out vec3 vPosition;
			
			void main() {
				vPosition = position;
				gl_Position = viewProjection * vec4(position, 1.0);
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

		squareShader.reset(new GooE::Shader(vertexSrc2, fragmentSrc2));
		shader.reset(new GooE::Shader(vertexSrc, fragmentSrc));
	}

    void OnUpdate(GooE::Timestep ts) override {

		GOOE_TRACE("Delta time: {0}s ({1}ms))", ts.GetSeconds(), ts.GetMilliseconds());

		if (GooE::Input::IsKeyPressed(GOOE_KEY_A)) {
			position.x -= cameraMoveSpeed * ts;
		} else if (GooE::Input::IsKeyPressed(GOOE_KEY_D)) {
			position.x += cameraMoveSpeed * ts;
		}

		if (GooE::Input::IsKeyPressed(GOOE_KEY_W)) {
			position.y += cameraMoveSpeed * ts;
		}
		else if (GooE::Input::IsKeyPressed(GOOE_KEY_S)) {
			position.y -= cameraMoveSpeed * ts;
		}

		if (GooE::Input::IsKeyPressed(GOOE_KEY_Q)) {
			rotation += cameraRotationSpeed * ts;
		}
		else if (GooE::Input::IsKeyPressed(GOOE_KEY_E)) {
			rotation -= cameraRotationSpeed * ts;
		}

		GooE::RenderCommand::Clear();

		camera.SetPosition(position);
		camera.SetRotation(rotation);

		GooE::Renderer::BeginScene(camera);

		GooE::Renderer::Submit(squareShader, squareVertexArray);
		GooE::Renderer::Submit(shader, vertexArray);

		GooE::Renderer::EndScene();
    }

    virtual void OnImGuiRender() override {
    }

private:
	std::shared_ptr<GooE::Shader> shader;
	std::shared_ptr<GooE::VertexArray> vertexArray;

	std::shared_ptr<GooE::Shader> squareShader;
	std::shared_ptr<GooE::VertexArray> squareVertexArray;

	float cameraMoveSpeed = 10.0f;
	float cameraRotationSpeed = 90.0f;
	float rotation = 0.0f;
	glm::vec3 position = {0.0f, 0.0f ,0.0f };

	GooE::OrthographicCamera camera;
};

class Sandbox : public GooE::Application {
public:
    Sandbox() : Application() {
        PushLayer(new TestLayer());		
    }

    ~Sandbox() {}
};

GooE::Application* GooE::CreateApplication() {
    return new Sandbox();
}
