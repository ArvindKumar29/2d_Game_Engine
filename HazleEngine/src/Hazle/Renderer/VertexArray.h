#pragma once

#include "Buffer.h"
#include "Renderer.h"


namespace Hazle 
{
	class VertexArray
	{
	public:
		~VertexArray();

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const Hazle::Ref<Hazle::VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Hazle::Ref<Hazle::IndexBuffer>& indexBuffer) = 0;

		virtual std::vector<Hazle::Ref<Hazle::VertexBuffer>> GetVertexBuffers() const = 0;
		virtual Hazle::Ref<Hazle::IndexBuffer> GetIndexBuffer() const = 0;

		static Ref<Hazle::VertexArray> Create();

	private:
	};
};