#pragma once
#include <string>
#include <glm/glm.hpp>

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

		static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);
	};
}