#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Platform/OpenGL/OpenGLShader.h>
#include <GooE.h>

class TestLayer : public GooE::Layer {
public:
	TestLayer() : Layer("Test"), cameraController(1.6f / 0.9f, true), squarePosition({0.0f}) {
		vertexArray.reset(GooE::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.5f, 0.1f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.5f, 0.8f, 0.1f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.1f, 0.5f, 0.8f, 1.0f,
		};

		GooE::Ref<GooE::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(GooE::VertexBuffer::Create(vertices, sizeof(vertices)));
		GooE::BufferLayout layout = {
			{ GooE::ShaderDataType::Float3, "position" },
			{ GooE::ShaderDataType::Float4, "color" },
		};

		vertexBuffer->SetLayout(layout);
		vertexArray->AddVertexBuffer(vertexBuffer);

		unsigned int indices[3] = { 0, 1 ,2 };
		GooE::Ref<GooE::IndexBuffer> indexBuffer;
		indexBuffer.reset(GooE::IndexBuffer::Create(indices, 3));
		vertexArray->SetIndexBuffer(indexBuffer);

		squareVertexArray.reset(GooE::VertexArray::Create());
		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		GooE::Ref<GooE::VertexBuffer> squareVB;
		squareVB.reset(GooE::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ GooE::ShaderDataType::Float3, "position" },
			{ GooE::ShaderDataType::Float2, "texCoords" }
		});
		squareVertexArray->AddVertexBuffer(squareVB);

		unsigned int squareIndices[6] = { 0, 1 ,2, 2, 3, 0 };
		GooE::Ref<GooE::IndexBuffer> squareIb;
		squareIb.reset(GooE::IndexBuffer::Create(squareIndices, sizeof(squareIndices)));
		squareVertexArray->SetIndexBuffer(squareIb);

		shaderLibrary.Load("assets/shaders/square.glsl");
		shaderLibrary.Load("assets/shaders/default.glsl");
		auto textureShader = shaderLibrary.Load("assets/shaders/texture.glsl");

		texture = GooE::Texture2D::Create("assets/textures/Checkerboard.png");
		transparentTexture = GooE::Texture2D::Create("assets/textures/transparent.png");

		textureShader->Bind();
		std::dynamic_pointer_cast<GooE::OpenGLShader>(textureShader)->UploadUniformInt("_texture", 0);
	}

    void OnUpdate(GooE::Timestep ts) override {
		cameraController.OnUpdate(ts);

		GooE::RenderCommand::Clear();
		GooE::Renderer::BeginScene(cameraController.GetCamera());

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		auto squareShader = std::dynamic_pointer_cast<GooE::OpenGLShader>(shaderLibrary.Get("square"));
		squareShader->Bind();
		squareShader->UploadUniformFloat3("color", squareColor);

		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

				GooE::Renderer::Submit(squareShader, squareVertexArray, transform);
			}
		}

		auto textureShader = shaderLibrary.Get("texture");
		auto defaultShader = shaderLibrary.Get("default");

		texture->Bind();
		GooE::Renderer::Submit(textureShader, squareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		transparentTexture->Bind();
		GooE::Renderer::Submit(textureShader, squareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		GooE::Renderer::Submit(defaultShader, vertexArray);
		GooE::Renderer::Submit(defaultShader, vertexArray, scale);

		GooE::Renderer::EndScene();
    }

	void OnEvent(GooE::Event& e) override {
		cameraController.OnEvent(e);
	}

    virtual void OnImGuiRender() override {
		ImGui::Begin("Color");
		ImGui::ColorEdit3("Square color", glm::value_ptr(squareColor));
		ImGui::End();
    }

private:
	GooE::ShaderLibrary shaderLibrary;

	GooE::Ref<GooE::VertexArray> vertexArray;
	GooE::Ref<GooE::VertexArray> squareVertexArray;

	GooE::Ref<GooE::Texture2D> texture, transparentTexture;

	GooE::OrthographicCameraController cameraController;

	glm::vec3 squarePosition;
	glm::vec3 squareColor = { 0.3f, 0.6f, 0.4f };
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
