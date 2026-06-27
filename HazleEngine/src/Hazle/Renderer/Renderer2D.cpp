#include "hzpch.h"
#include "Renderer2D.h"
#include "VertexArray.h"

namespace Hazle
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> m_SquareVA;
		Ref<Shader> m_TextureShader;
		Ref<Texture2D> m_WhiteTexture;
	};

	Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();
		s_Data->m_SquareVA = VertexArray::Create();
		float vertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f
		};

		uint32_t indices[6] = { 0, 1, 2, 1, 3, 2 };

		Ref<Hazle::VertexBuffer> squareVB;
		squareVB = VertexBuffer::Create(vertices, sizeof(vertices));
		//m_SquareVA->AddVertexBuffer(squareVB);
		Ref<IndexBuffer> squareIB;
		squareIB = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		s_Data->m_SquareVA->SetIndexBuffer(squareIB);

		s_Data->m_WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whitePixel = 0xffffffff;
		s_Data->m_WhiteTexture->SetData(&whitePixel, sizeof(whitePixel));

		BufferLayout layout2 = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoord"}
		};
		squareVB->SetLayout(layout2);
		s_Data->m_SquareVA->AddVertexBuffer(squareVB);

		s_Data->m_TextureShader = Shader::Create("Assets/Shaders/Texture.glsl");
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->m_TextureShader->Bind();
		s_Data->m_TextureShader->SetMat4("u_VP", camera.GetVPMatrix());
	}
	
	void Renderer2D::EndScene()
	{
	}
	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}
	
	void Renderer2D::DrawQuad(const glm::vec2 & position, const float rotation, const glm::vec2 & size, const glm::vec4 & color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, rotation, size, color);
	}
	
	void Renderer2D::DrawQuad(const glm::vec3& position, const float rotation, const glm::vec2& size, const glm::vec4& color)
	{
		s_Data->m_TextureShader->SetFloat4("u_Color", color);
		s_Data->m_WhiteTexture->Bind(0);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * 
			glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) * 
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data->m_TextureShader->SetMat4("u_Transform", transform);
		//s_Data->m_TextureShader->Bind();

		s_Data->m_SquareVA->Bind();
		RenderCommand::DrawIndexed(s_Data->m_SquareVA);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const float rotation, const glm::vec2& size, const Ref<Texture2D> texture)
	{
		DrawQuad({position.x, position.y, 0.0f}, rotation, size, texture);
	}
		
	void Renderer2D::DrawQuad(const glm::vec3& position, const float rotation, const glm::vec2& size, const Ref<Texture2D> texture)
	{
		s_Data->m_TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));

		s_Data->m_TextureShader->Bind();
		s_Data->m_TextureShader->SetInt("u_Texture", 0);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * 
			glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) * 
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->m_TextureShader->SetMat4("u_Transform", transform);

		texture->Bind(0);

		s_Data->m_SquareVA->Bind();
		RenderCommand::DrawIndexed(s_Data->m_SquareVA);
	}
}