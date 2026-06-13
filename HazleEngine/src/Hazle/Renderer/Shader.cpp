#include "hzpch.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "Renderer.h"
#include "Log.h"

namespace Hazle
{
	Shader::Shader()
	{}

	Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{}

	Shader::~Shader()
	{}

	Shader* Shader::Create(const std::string& path)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			HZ_CORE_ERROR("RendererAPI::None is currently not supported!!!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLShader(path);
		}
		HZ_CORE_ERROR("Unknown RendererAPI!!!");
		return nullptr;
	}

	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			HZ_CORE_ERROR("RendererAPI::None is currently not supported!!!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLShader(vertexSrc, fragmentSrc);
		}
		HZ_CORE_ERROR("Unknown RendererAPI!!!");

	}	
}