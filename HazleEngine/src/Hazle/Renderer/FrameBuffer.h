#pragma once
#include "Renderer.h"

namespace Hazle
{
	struct FrameBufferSpecifications
	{
		uint32_t Width, Height;
		uint32_t Samples = 1;
		bool SwapChainTarget = false;

	};

	class FrameBuffer
	{
	public:
		FrameBuffer();
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		static Ref<FrameBuffer> Create(const FrameBufferSpecifications& spec);

		virtual const FrameBufferSpecifications& GetSpecifications() const = 0;
		virtual uint32_t GetColorAttachmentRendererID() const = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

	private:
		
	};
}