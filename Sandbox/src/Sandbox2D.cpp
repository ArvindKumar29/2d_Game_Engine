#include <hzpch.h>
#include "Sandbox2D.h"


Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"),
	m_CameraController(1280.0f, 720.0f, true)
{}


void Sandbox2D::OnAttach()
{
	m_CheckerboardTexture = Hazle::Texture2D::Create("Assets/Textures/checkerboard.png");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	ImGui::Text("Hello Hazle!!!!!");
	for (auto& result : m_ProfileResults)
	{
		//////////////CHERNO WAY/////////////////
		//char label[50];
		//strcpy(label, result.name);
		//strcat(label, " %.3fms");
		//ImGui::Text(label, result.time);

		ImGui::Text("%.3fms: %s", result.time, result.name);
	}
	m_ProfileResults.clear();

	ImGui::End();
}

void Sandbox2D::OnEvent(Hazle::Event & e)
{
	m_CameraController.OnEvent(e);
}

void Sandbox2D::OnUpdate(Hazle::Timestep ts)
{
	HZ_PROFILE_FUNCTION();
	// Update
	{
		HZ_PROFILE_SCOPE("Sandbox2D::OnUpdate");
		m_CameraController.OnUpdate(ts);
	}
	
	// Render
	{
		HZ_PROFILE_SCOPE("Sandbox2D::OnUpdate::RenderPrep");
		Hazle::RenderCommand::Clear();
		Hazle::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	}
	{
		HZ_PROFILE_SCOPE("Sandbox2D::OnUpdate::Rendering");
		Hazle::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Hazle::Renderer2D::DrawQuad({ -0.5f, 0.5f }, 0.0f, glm::vec2(1.0f), { 0.8f, 0.2f, 0.1f, 1.0f });
		Hazle::Renderer2D::DrawQuad({ 1.0f, 0.2f }, 0.0f, { 0.8f, 0.6f }, { 0.3f, 0.9f, 0.1f, 1.0f });
		Hazle::Renderer2D::DrawQuad({ 0.5f, 1.2f, -0.5f }, 0.0f, { 0.5f, 0.5f }, m_CheckerboardTexture);

		Hazle::Renderer2D::EndScene();
	}

	//static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
	//glm::mat4 squareTransform;
	//std::dynamic_pointer_cast<Hazle::OpenGLShader>(m_Shader)->Bind();
	//std::dynamic_pointer_cast<Hazle::OpenGLShader>(m_Shader)->UploadUniformFloat4("u_Color", m_SquareColor);

}