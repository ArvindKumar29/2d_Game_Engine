#include <hzpch.h>
#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Hazle
{
	VertexBuffer::~VertexBuffer()
	{}
	
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)		// allocates memoory of size for the vertex buffer
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexBuffer>(size);
		}

		HZ_CORE_ASSERT(false, "Unknown RendererAPI!!!");
		return nullptr;
	}

	Ref<Hazle::VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)		// allocates memory of size for the vertex buffer and copies the vertices data into it
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:
				return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		}

		HZ_CORE_ASSERT(false, "Unknown RendererAPI!!!");
		return nullptr;
	}


	

	IndexBuffer::~IndexBuffer()
	{}

	Ref<Hazle::IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)		// allocates memory of size for the index buffer and copies the indices data into it
	{	
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:
				return std::make_shared<OpenGLIndexBuffer>(indices, size);
		}
		HZ_CORE_ASSERT(false, "Unknown RendererAPI!!!");
		return nullptr;
	}
}