#include "gepch.h"

#include <glad/glad.h>
#include "OpenGLBuffer.h"

namespace GooE {

	// VertexBuffer ---------------

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) {
		GOOE_PROFILE_FUNCTION();

		glCreateBuffers(1, &renderedId);
		glBindBuffer(GL_ARRAY_BUFFER, renderedId);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {
		GOOE_PROFILE_FUNCTION();

		glCreateBuffers(1, &renderedId);
		glBindBuffer(GL_ARRAY_BUFFER, renderedId);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		GOOE_PROFILE_FUNCTION();

		glDeleteBuffers(1, &renderedId);
	}

	void OpenGLVertexBuffer::Bind() const {
		GOOE_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, renderedId);
	}

	void OpenGLVertexBuffer::Unbind() const {
		GOOE_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size) {
		glBindBuffer(GL_ARRAY_BUFFER, renderedId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	// IndexBuffer ---------------

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) : count(count) {
		GOOE_PROFILE_FUNCTION();

		glCreateBuffers(1, &renderedId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderedId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer() {
		GOOE_PROFILE_FUNCTION();

		glDeleteBuffers(1, &renderedId);
	}

	void OpenGLIndexBuffer::Bind() const {
		GOOE_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderedId);
	}

	void OpenGLIndexBuffer::Unbind() const {
		GOOE_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
