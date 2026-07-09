#pragma once
#include "Hazle/Renderer/FrameBuffer.h"

namespace Hazle
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecifications& spec);
		~OpenGLFrameBuffer();

		virtual FrameBufferSpecifications& GetSpecifications() const override { return m_Specifications; }
		virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }
		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

	private:
		uint32_t m_RendererID, m_ColorAttachment, m_DepthAttachment;
		FrameBufferSpecifications& m_Specifications;
	
	};
}
