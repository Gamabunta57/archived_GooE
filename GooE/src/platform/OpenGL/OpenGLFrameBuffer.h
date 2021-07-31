#pragma once

#include <GooE/Renderer/FrameBuffer.h>

namespace GooE {
	class OpenGLFrameBuffer : public FrameBuffer {
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& specification);
		virtual ~OpenGLFrameBuffer() override;

		void Invalidate();
		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetColorAttachmentRendererID() const override { return colorAttachment; }
		virtual const FrameBufferSpecification GetSpecification() const override { return specification; }

	private:
		uint32_t rendererID = 0;
		uint32_t colorAttachment = 0, depthAttachment = 0;
		FrameBufferSpecification specification;
	};
}

