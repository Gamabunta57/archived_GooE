#include "gepch.h"

#include <glad/glad.h>

#include "OpenGLVertexArray.h"

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
		GOOE_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &rendererId);
	}

	OpenGLVertexArray::~OpenGLVertexArray() {
		GOOE_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &rendererId);
	}

	void OpenGLVertexArray::Bind() const {
		GOOE_PROFILE_FUNCTION();

		glBindVertexArray(rendererId);
	}

	void OpenGLVertexArray::Unbind() const {
		GOOE_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const GooE::Ref<VertexBuffer>& vertexBuffer) {
		GOOE_PROFILE_FUNCTION();

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

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
		GOOE_PROFILE_FUNCTION();

		glBindVertexArray(rendererId);
		indexBuffer->Bind();

		this->indexBuffer = indexBuffer;
	}
}
