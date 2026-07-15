#include <hzpch.h>
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "Renderer.h"
#include "Hazle/Core/Log.h"

namespace Hazle
{
	Shader::Shader()
	{}

	Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{}

	Shader::~Shader()
	{}

	Ref<Shader> Shader::Create(const std::string& path)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			HZ_CORE_ERROR("RendererAPI::None is currently not supported!!!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(path);
		}
		HZ_CORE_ERROR("Unknown RendererAPI!!!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			HZ_CORE_ERROR("RendererAPI::None is currently not supported!!!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}
		HZ_CORE_ERROR("Unknown RendererAPI!!!");
		return nullptr;
	}	
	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto name = shader->GetName();
		HZ_CORE_ASSERT(m_Shaders.find(name) == m_Shaders.end(), "Shader already exists in the library!!!");
		m_Shaders[shader->GetName()] = shader;
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		HZ_CORE_ASSERT(m_Shaders.find(name) == m_Shaders.end(), "Shade r already exists in the library!!!");
		m_Shaders[name] = shader;

	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		auto it = m_Shaders.find(name);
		if (it != m_Shaders.end())
			return it->second;
		return Ref<Shader>();
	}
	
	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& path)
	{
		auto shader = Shader::Create(path);
		Add(name, shader);
		return shader;
	}
	
	Ref<Shader> ShaderLibrary::Load(const std::string& path)
	{
		auto shader = Shader::Create(path);
		Add(shader);
		return shader;
	}
}