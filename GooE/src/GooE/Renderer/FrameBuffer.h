#pragma once

#include <GooE/Core/Core.h>

namespace GooE {

	struct FrameBufferSpecification {
		uint32_t width, height;
		//FrameBufferFormat format = ;
		uint32_t samples = 1;
		bool swapChainTarget = false;
	};

	class FrameBuffer {
	public:
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;

		virtual const FrameBufferSpecification GetSpecification() const = 0;

		static Ref<FrameBuffer> Create(const FrameBufferSpecification& specification);


	private:
	};
}

