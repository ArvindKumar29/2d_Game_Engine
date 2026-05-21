#include "hzpch.h"
#include "Renderer.h"
#include "VertexArray.h"

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

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_VP", m_SceneData->VPMatrix);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}