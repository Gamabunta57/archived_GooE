#include "gepch.h"
#include "OpenGLVertexArray.h"
#include "glad/glad.h"

namespace GooE {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
		switch (type) {
			case ShaderDataType::Float:  return GL_FLOAT;
			case ShaderDataType::Float2: return GL_FLOAT;
			case ShaderDataType::Float3: return GL_FLOAT;
			case ShaderDataType::Float4: return GL_FLOAT;

			case ShaderDataType::Mat3:   return GL_FLOAT;
			case ShaderDataType::Mat4:   return GL_FLOAT;

			case ShaderDataType::Int:    return GL_INT;
			case ShaderDataType::Int2:   return GL_INT;
			case ShaderDataType::Int3:   return GL_INT;
			case ShaderDataType::Int4:   return GL_INT;

			case ShaderDataType::Bool:   return GL_BOOL;
		}

		GOOE_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray() {
		glCreateVertexArrays(1, &rendererId);
	}

	OpenGLVertexArray::~OpenGLVertexArray() {
		glDeleteVertexArrays(1, &rendererId);
	}

	void OpenGLVertexArray::Bind() const {
		glBindVertexArray(rendererId);
	}

	void OpenGLVertexArray::Unbind() const {
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) {
		glBindVertexArray(rendererId);
		vertexBuffer->Bind();

		GOOE_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has not layout!");

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.type),
				element.normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.offset
			);
			index++;
		}

		vertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) {
		glBindVertexArray(rendererId);
		indexBuffer->Bind();

		this->indexBuffer = indexBuffer;
	}
}