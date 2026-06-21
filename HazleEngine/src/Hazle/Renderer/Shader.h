#pragma once
#include <string>
//#include <glm/glm.hpp>

namespace Hazle
{
	class Shader
	{
	public:
		Shader();
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& path);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);	
	};
	
	class ShaderLibrary
	{
	public:
		void Add(const Ref<Shader>& shader);
		void Add(const std::string& name,const Ref<Shader>& shader);
		Ref<Shader> Get(const std::string& name);
		Ref<Shader> Load(const std::string& name, const std::string& path);
		Ref<Shader> Load(const std::string& path);


	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;

	};

}