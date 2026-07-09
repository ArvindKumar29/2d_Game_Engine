#include <hzpch.h>
#include "Sandbox2D.h"

static const uint32_t s_MapWidth = 25;
static const char* s_MapTiles = 
"WWWWWWWWWWWWWWWWWWWWWWWWD"
"WWWWWWWDDDDDDWWWWWWWWWWWW"
"WWWWWDDDDWDDDDDDDWWWWWWWW"
"WWWDDDDDDDDDDDDDDDDDWWWWW"
"WWWWWDDWWWWWDDDDDDDDDWWWW"
"WWWDDDDDDDDDDDDWWWDDDWWWW"
"WWWDDWWWDDDDDDDDDWWWWDWWW"
"WWWDDDDDDDDDDDDDDDDDDDWWW"
"WWWDDWWDDDDDDDDDWWWDWDWWW"
"WWWDDDDDDDDDWWWWWWWWWDWWW"
"WWWWWDDDDDDDDDDDDDDDWWWWW"
"WWWWWWDDDDDDDDDDDDWWWWWWW"	
"WWWWWWWWWWDDDDDWWWWWWWWWW"	
"WWWWWWWWWWWWWWWWWWWWWWWWW"								
"WWWWWWWWWWWWWWWWWWWWWWWWW"
;

Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"),
	m_CameraController(1280.0f, 720.0f, true)
{}


void Sandbox2D::OnAttach()
{
	m_CheckerboardTexture = Hazle::Texture2D::Create("Assets/Textures/checkerboard.png");
	m_SpriteSheet = Hazle::Texture2D::Create("Assets/RPG_base_assets/kenney_rpg-base/Spritesheet/RPGpack_sheet_2X.png");
	m_SubTexture = Hazle::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 1, 11 }, { 128, 128 }, { 1, 1 });


	m_MapWidth = s_MapWidth;
	m_MapHeight = strlen(s_MapTiles) / s_MapWidth;
	m_TextureMap['W'] = Hazle::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11, 11 }, { 128, 128 }, { 1, 1 });
	m_TextureMap['D'] = Hazle::SubTexture2D::CreateFromCoords(m_SpriteSheet, {6, 11}, {128, 128}, {1, 1});
	
	// Init here
	m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 1.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };

	m_CameraController.SetZoomLevel(10.0f);

}

void Sandbox2D::OnDetach()
{

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
	m_SquareRotation += ts * glm::radians(360.0f);
	Hazle::Renderer2D::ResetStats();
	{
		HZ_PROFILE_SCOPE("Sandbox2D::OnUpdate::RenderPrep");
		Hazle::RenderCommand::Clear();
		Hazle::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	}

	{
		HZ_PROFILE_SCOPE("Sandbox2D::OnUpdate::Rendering");
		Hazle::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Hazle::Renderer2D::DrawQuad({ -0.5f, 0.0f }, glm::radians(0.0f),glm::vec2(1.0f), { 0.8f, 0.2f, 0.1f, 1.0f });
		Hazle::Renderer2D::DrawQuad({ 2.0f, 0.2f }, glm::radians(45.0f), { 0.8f, 0.6f }, { 0.3f, 0.9f, 0.1f, 1.0f });
		Hazle::Renderer2D::DrawQuad({ -2.0f, -0.2f }, glm::radians(0.0f), { 0.8f, 0.4f }, { 0.3f, 0.6f, 0.1f, 1.0f });
		Hazle::Renderer2D::DrawQuad({ -0.25f, -0.25f, glm::radians(-0.1f) }, 0.0f, { 10.0f, 10.0f }, m_CheckerboardTexture, glm::vec4(1.0f));
		Hazle::Renderer2D::DrawQuad({ 2.0f, -2.0f, 0.1f }, glm::radians(0.0f), { 1.0f, 1.0f }, m_CheckerboardTexture, glm::vec4(1.0f), 5.0f);
		//Hazle::Renderer2D::EndScene();
	
		//Hazle::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.6f };
				Hazle::Renderer2D::DrawQuad({ x, y }, glm::radians(0.0f), { 0.45f, 0.45f }, color);
			}
		}
		Hazle::Renderer2D::EndScene();
	}
	

	//static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
	//glm::mat4 QuadTransform;
	//std::dynamic_pointer_cast<Hazle::OpenGLShader>(m_Shader)->Bind();
	//std::dynamic_pointer_cast<Hazle::OpenGLShader>(m_Shader)->UploadUniformFloat4("u_Color", m_QuadColor);

	if (Hazle::Input::IsMouseButtonPressed(Hazle::Mouse::ButtonLeft))
	{
		auto [x, y] = Hazle::Input::GetMousePosition();
		auto width = Hazle::Application::Get().GetWindow().GetWidth();
		auto height = Hazle::Application::Get().GetWindow().GetHeight();

		auto bounds = m_CameraController.GetBounds();
		auto pos = m_CameraController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_Particle.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 5; i++)
		{
			m_ParticleSystem.Emit(m_Particle);
		}
	}

#if 0
	Hazle::Renderer2D::BeginScene(m_CameraController.GetCamera());
	//Hazle::Renderer2D::DrawQuad({ -0.25f, -0.25f, 0.5f }, glm::radians(0.0f), { 1.0f, 1.0f }, m_SubTexture, glm::vec4(1.0f));

	for (uint32_t y = 0; y < m_MapHeight; y++)
	{
		for (uint32_t x = 0; x < m_MapWidth; x++)
		{
			char tileType = s_MapTiles[x + y * m_MapWidth];
			auto texture = m_TextureMap[tileType];
			float xPos = x - m_MapWidth / 2.0f, yPos = y - m_MapHeight / 2.0f;
			if (texture)
				Hazle::Renderer2D::DrawQuad({xPos, -yPos, 0.5f}, glm::radians(0.0f), glm::vec2(1.0f), texture);
			else
				Hazle::Renderer2D::DrawQuad({ xPos, yPos, 0.5f }, glm::radians(0.0f), { 1.0f, 1.0f }, m_SubTexture, glm::vec4(1.0f));
		}
	}
#endif
	Hazle::Renderer2D::EndScene();

	m_ParticleSystem.OnUpdate(ts);
	m_ParticleSystem.OnRender(m_CameraController.GetCamera());

}


void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	//ImGui::ColorEdit4("Quad Color", glm::value_ptr(m_QuadColor));

	auto stats = Hazle::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.QuadCount * 4);
	ImGui::Text("Indices: %d", stats.QuadCount * 6);

	m_ProfileResults.clear();

	ImGui::End();
}