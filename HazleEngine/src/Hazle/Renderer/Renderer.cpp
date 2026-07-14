#include <hzpch.h>
#include "Renderer.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazle
{

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData();

	void Renderer::BeginScene(const OrthographicCamera& camera)
	{
		m_SceneData->VPMatrix = camera.GetVPMatrix();
	}
	
	void Renderer::EndScene()
	{
	
	}

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::Submit(const Hazle::Ref<Hazle::Shader>& shader, const Hazle::Ref<Hazle::VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_VP", m_SceneData->VPMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}