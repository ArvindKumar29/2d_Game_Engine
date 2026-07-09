#include <hzpch.h>
#include "FrameBuffer.h"
#include "Platform/OpenGL/OpenGLFramrBuffer.h"

namespace Hazle
{
	FrameBuffer::FrameBuffer()
	{}

	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecifications & spec)
	{
		switch (Hazle::Renderer::GetAPI())
		{
			case RendererAPI::API::OpenGL:
				return Ref<FrameBuffer>(new OpenGLFrameBuffer(spec));
		}
		return nullptr;
	}
}