#pragma once
#include "Hazle/Renderer/FrameBuffer.h"

namespace Hazle
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecifications& spec);
		~OpenGLFrameBuffer();

		virtual const FrameBufferSpecifications GetSpecifications() const override { return m_Specifications; }
		virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }
		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;

	private:
		uint32_t m_RendererID = 0, m_ColorAttachment = 0, m_DepthAttachment = 0;
		FrameBufferSpecifications m_Specifications;
	
	};
}
