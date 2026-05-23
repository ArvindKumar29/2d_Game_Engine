#include "hzpch.h"
#include "RendererAPI.h"
#include "VertexArray.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Hazle
{
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	RendererAPI* RendererAPI::Create()
	{
		switch (s_API)
		{
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLRendererAPI();
		}

		HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	void RendererAPI::DrawIndexed(const Hazle::Ref<Hazle::VertexArray>& vertexArray)
	{
		
	}
}