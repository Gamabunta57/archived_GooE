#pragma once

#include <GooE/Renderer/FrameBuffer.h>

namespace GooE {
	class OpenGLFrameBuffer : public FrameBuffer {
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& specification);
		virtual ~OpenGLFrameBuffer();

		void Invalidate();
		virtual void Bind() override;
		virtual void Unbind() override;

		virtual uint32_t GetColorAttachmentRendererID() const override { return colorAttachment; }
		virtual const FrameBufferSpecification GetSpecification() const override { return specification; }

	private:
		uint32_t rendererID;
		uint32_t colorAttachment, depthAttachment;
		FrameBufferSpecification specification;
	};
}

