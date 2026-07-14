#include <hzpch.h>
#include "VertexArray.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"


namespace Hazle
{
	VertexArray::~VertexArray()
	{}

	Ref<Hazle::VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexArray>();
		}
		HZ_CORE_ASSERT(false, "RendererAPIError::Unknown Renderer API!!!"); return nullptr;

	}

}