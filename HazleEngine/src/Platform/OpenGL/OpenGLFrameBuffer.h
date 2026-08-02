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
		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { HZ_CORE_ASSERT(index < m_ColorAttachments.size()); return m_ColorAttachments[index]; }
		void Invalidate();

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual void ClearColorAttachments(uint32_t attachmentIndex, const int value) override;

		virtual void Bind() override;
		virtual void Unbind() override;



	private:
		uint32_t m_RendererID = 0, m_ColorAttachment = 0;
		FrameBufferSpecifications m_Specifications;

		std::vector<FramebufferTextureSpecifications> m_ColorAttachmentSpecs;
		FramebufferTextureSpecifications m_DepthAttachmentSpecs = FramebufferTextureFormat::None;
	
		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachments = 0;

	};
}
