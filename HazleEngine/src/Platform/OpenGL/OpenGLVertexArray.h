#pragma once

#include "Hazle/Renderer/VertexArray.h"

namespace Hazle
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Hazle::Ref<Hazle::VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Hazle::Ref<Hazle::IndexBuffer>& indexBuffer) override;

		virtual std::vector<Hazle::Ref<Hazle::VertexBuffer>> GetVertexBuffers() const { return m_VertexBuffers; }
		virtual Hazle::Ref<Hazle::IndexBuffer> GetIndexBuffer() const { return m_IndexBuffer; }

	private:
		uint32_t m_RendererID;
		uint32_t m_VertexBufferIndex = 0;
		std::vector<Ref<Hazle::VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}
