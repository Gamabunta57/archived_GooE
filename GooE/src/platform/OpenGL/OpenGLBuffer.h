#pragma once

#include <cstdint>

#include "GooE/Renderer/Buffer.h"

namespace GooE {

	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		
		virtual ~OpenGLVertexBuffer();
		virtual void Bind() const;
		virtual void Unbind() const;

	private:
		uint32_t renderedId;
	};

	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(uint32_t* vertices, uint32_t count);

		virtual ~OpenGLIndexBuffer();
		virtual void Bind() const;
		virtual void Unbind() const;
		virtual uint32_t GetCount() const;

	private:
		uint32_t renderedId;
		uint32_t count;
	};
}
