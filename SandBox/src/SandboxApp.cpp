#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Platform/OpenGL/OpenGLShader.h>
#include <GooE.h>

class TestLayer : public GooE::Layer {
public:
	TestLayer() : Layer("Test"), camera(-1.6f, 1.6f, -0.9f, 0.9f), squarePosition({0.0f}) {
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

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 position;
			layout(location = 1) in vec4 color;

			uniform mat4 viewProjection;
			uniform mat4 transform;

			out vec3 vPosition;
			out vec4 vColor;
			
			void main() {
				vPosition = position;
				vColor = color;
				gl_Position = viewProjection * transform * vec4(position, 1.0);
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
			uniform mat4 transform;
			
			void main() {
				gl_Position = viewProjection * transform * vec4(position, 1.0);
			}
		)";

		std::string fragmentSrc2 = R"(
			#version 330 core

			layout(location = 0) out vec3 oColor;
			uniform vec3 color;

			void main() {
				oColor = color;
			}
		)";

		std::string vertexSrcTex = R"(
			#version 330 core

			layout(location = 0) in vec3 position;
			layout(location = 1) in vec2 texCoords;

			uniform mat4 viewProjection;
			uniform mat4 transform;

			out vec2 vTexCoords;
			
			void main() {
				vTexCoords = texCoords;
				gl_Position = viewProjection * transform * vec4(position, 1.0);
			}
		)";

		std::string fragmentSrcTex = R"(
			#version 330 core

			out vec4 oColor;
			in vec2 vTexCoords;

			uniform sampler2D _texture;

			void main() {
				oColor = texture(_texture, vTexCoords);
			}
		)";

		squareShader.reset(GooE::Shader::Create(vertexSrc2, fragmentSrc2));
		shader.reset(GooE::Shader::Create(vertexSrc, fragmentSrc));

		textureShader.reset(GooE::Shader::Create(vertexSrcTex, fragmentSrcTex));
		texture = GooE::Texture2D::Create("assets/textures/Checkerboard.png");

		std::dynamic_pointer_cast<GooE::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<GooE::OpenGLShader>(textureShader)->UploadUniformInt("_texture", 0);
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

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		squareShader->Bind();
		std::dynamic_pointer_cast<GooE::OpenGLShader>(squareShader)->UploadUniformFloat3("color", squareColor);

		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				glm::vec3 pos(x * 0.11f, y * 0.110f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

				GooE::Renderer::Submit(squareShader, squareVertexArray, transform);
			}
		}

		texture->Bind();
		GooE::Renderer::Submit(textureShader, squareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		//GooE::Renderer::Submit(shader, vertexArray);
		//GooE::Renderer::Submit(shader, vertexArray, scale);

		GooE::Renderer::EndScene();
    }

    virtual void OnImGuiRender() override {
		ImGui::Begin("Color");
		ImGui::ColorEdit3("Square color", glm::value_ptr(squareColor));
		ImGui::End();
    }

private:
	GooE::Ref<GooE::Shader> shader;
	GooE::Ref<GooE::VertexArray> vertexArray;

	GooE::Ref<GooE::Shader> squareShader, textureShader;
	GooE::Ref<GooE::VertexArray> squareVertexArray;

	GooE::Ref<GooE::Texture2D> texture;

	float cameraMoveSpeed = 10.0f;
	float cameraRotationSpeed = 90.0f;
	float rotation = 0.0f;
	glm::vec3 position = {0.0f, 0.0f ,0.0f };
	GooE::OrthographicCamera camera;

	glm::vec3 squarePosition;
	float squareMoveSpeed = 3.0f;

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
