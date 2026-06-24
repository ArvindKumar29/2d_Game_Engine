#include "hzpch.h"
#include "OpenGLShader.h"
#include "Hazle/Core/Log.h"
#include "glad/glad.h"
#include <fstream>


namespace Hazle
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;
		HZ_CORE_ASSERT(false, "Unknown shader type!!!");
		return 0;
	}
	OpenGLShader::OpenGLShader(const std::string& path)
	 {
		std::string Source = ReadFile(path);
		auto shaderSources = PreProcess(Source);
		compile(shaderSources);

		// Extract name from filepath
		auto lastSlash = path.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = path.rfind('.');
		auto count = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;

		m_Name = path.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_Name(name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		//auto shaderSources = PreProcess(sources);
		compile(sources);

		m_Name = name;
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	const std::string& OpenGLShader::GetName() const
	{
		return m_Name;
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		Bind();
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		Bind();
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& values)
	{
		Bind();
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, values.x, values.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values)
	{
		Bind();
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, values.x, values.y, values.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& values)
	{
		Bind();
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, values.x, values.y, values.z, values.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& mat)
	{
		Bind();
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, &mat[0][0]);
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& mat)
	{
		Bind();
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
	}

	std::string OpenGLShader::ReadFile(const std::string& path)
	{
		std::string result;
		std::ifstream in(path, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			HZ_CORE_ERROR("Could not open shader file {0}", path);
			HZ_CORE_ASSERT(false, "Shader compilation failure!!!");
		}
		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess (const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;
		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);

		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			HZ_CORE_ASSERT(eol != std::string::npos, "Syntax error!!!");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			type.erase(std::remove_if(type.begin(), type.end(), ::isspace), type.end());

			HZ_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified!!!");
			
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}
		return shaderSources;
	}

	void OpenGLShader::compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		// CREATE AND ATTACH //
		HZ_CORE_ASSERT(shaderSources.size() > 0, "WTF! Shader file is completely empty or #type is missing!");

		GLuint program = glCreateProgram();
		HZ_CORE_ASSERT(shaderSources.size() <= 2, "Only support for up to 2 shaders!");
		std::array<GLenum, 2> shaderIDs;
		int glShaderIDIndex = 0;

		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);
			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);
			glCompileShader(shader);
			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);
				HZ_CORE_ERROR("Failed to compile {0} shader!!!", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"));
				HZ_CORE_ERROR("{0}", std::string(infoLog.data()));
				
				HZ_CORE_ASSERT(false, "Shader compilation failure!!!");
				return;
			}

			glAttachShader(program, shader);
			shaderIDs[glShaderIDIndex++] = shader;
		} 
		glLinkProgram(program);

		// DELETE AND DETACH //
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength); // Isko bhi Program kar dena

			// 2. SAFETY CHECK: Agar maxLength 0 se bada ho, tabhi vector access karo
			if (maxLength > 0)
			{
				std::vector<GLchar> infoLog(maxLength);
				glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
				HZ_CORE_ERROR("{0}", infoLog.data());
			}
			else
			{
				HZ_CORE_ERROR("Linking failed but no log was generated.");
			}

			glDeleteProgram(program);
			for (auto id : shaderIDs)
			{
				glDeleteShader(id);
			}

			HZ_CORE_ASSERT(false, "Shader link failure!!!");
			return;
		}

		for (auto id : shaderIDs)
			glDetachShader(program, id);

		m_RendererID = program;

	}
}