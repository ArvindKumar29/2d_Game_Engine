#include <hzpch.h>
#include "EditorLayer.h"

namespace Hazle
{
	EditorLayer::EditorLayer()
		:Layer("EditorLayer"),
		m_CameraController(1280.0f, 720.0f, true)
	{}


	void EditorLayer::OnAttach()
	{
		m_CheckerboardTexture = Texture2D::Create("Assets/Textures/checkerboard.png");
		m_SpriteSheet = Texture2D::Create("Assets/RPG_base_assets/kenney_rpg-base/Spritesheet/RPGpack_sheet_2X.png");
		m_SubTexture = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 1, 11 }, { 128, 128 }, { 1, 1 });


		/*m_MapWidth = s_MapWidth;
		m_MapHeight = strlen(s_MapTiles) / s_MapWidth;
		m_TextureMap['W'] = Hazle::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11, 11 }, { 128, 128 }, { 1, 1 });
		m_TextureMap['D'] = Hazle::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6, 11 }, { 128, 128 }, { 1, 1 });
		

		// Init here
		m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
		m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
		m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
		m_Particle.LifeTime = 1.0f;
		m_Particle.Velocity = { 0.0f, 0.0f };
		m_Particle.VelocityVariation = { 3.0f, 1.0f };
		m_Particle.Position = { 0.0f, 0.0f };*/

		m_CameraController.SetZoomLevel(10.0f);

		Hazle::FrameBufferSpecifications fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_FrameBuffer = Hazle::FrameBuffer::Create(fbSpec);

		m_ActiveScene = CreateRef<Scene>();
		auto square = m_ActiveScene->CreateEntity();
		m_ActiveScene->Reg().emplace<CTransform>(square, glm::mat4(1.0f));
		m_ActiveScene->Reg().emplace<CSpriteRenderer>(square, glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});	

		m_SquareEntity = square;
	}

	void EditorLayer::OnDetach()
	{

	}


	void EditorLayer::OnEvent(Hazle::Event& e)
	{
		m_CameraController.OnEvent(e);
	}

	void EditorLayer::OnUpdate(Hazle::Timestep ts)
	{
		HZ_PROFILE_FUNCTION();
		// Update
		m_FrameBuffer->Bind();
		{
			if (m_ViewportFocused)
			{
				HZ_PROFILE_SCOPE("EditorLayer::OnUpdate");
				m_CameraController.OnUpdate(ts);
			}
		}


		// Render
		m_SquareRotation += ts * glm::radians(360.0f);
		Hazle::Renderer2D::ResetStats();
		{
			HZ_PROFILE_SCOPE("EditorLayer::OnUpdate::RenderPrep");
			Hazle::RenderCommand::Clear();
			Hazle::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		}

		{
			HZ_PROFILE_SCOPE("EditorLayer::OnUpdate::Rendering");
			Hazle::Renderer2D::BeginScene(m_CameraController.GetCamera());

			// UPDATE SCENE
			m_ActiveScene->OnUpdate(ts);

			//Hazle::Renderer2D::DrawQuad({ -0.5f, 0.0f }, glm::radians(0.0f), glm::vec2(1.0f), { 0.8f, 0.2f, 0.1f, 1.0f });
			//Hazle::Renderer2D::DrawQuad({ 2.0f, 0.2f }, glm::radians(45.0f), { 0.8f, 0.6f }, { 0.3f, 0.9f, 0.1f, 1.0f });
			//Hazle::Renderer2D::DrawQuad({ -2.0f, -0.2f }, glm::radians(0.0f), { 0.8f, 0.4f }, { 0.3f, 0.6f, 0.1f, 1.0f });
			//Hazle::Renderer2D::DrawQuad({ -0.25f, -0.25f, glm::radians(-0.1f) }, 0.0f, { 10.0f, 10.0f }, m_CheckerboardTexture, glm::vec4(1.0f));
			//Hazle::Renderer2D::DrawQuad({ 2.0f, -2.0f, 0.1f }, glm::radians(0.0f), { 1.0f, 1.0f }, m_CheckerboardTexture, glm::vec4(1.0f), 5.0f);
			Hazle::Renderer2D::EndScene();
		}


		//static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		//glm::mat4 QuadTransform;
		//std::dynamic_pointer_cast<Hazle::OpenGLShader>(m_Shader)->Bind();
		//std::dynamic_pointer_cast<Hazle::OpenGLShader>(m_Shader)->UploadUniformFloat4("u_Color", m_QuadColor);

		/*if (Hazle::Input::IsMouseButtonPressed(Hazle::Mouse::ButtonLeft))
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
		}*/

		Hazle::Renderer2D::BeginScene(m_CameraController.GetCamera());
#if 0
		//Hazle::Renderer2D::DrawQuad({ -0.25f, -0.25f, 0.5f }, glm::radians(0.0f), { 1.0f, 1.0f }, m_SubTexture, glm::vec4(1.0f));

		for (uint32_t y = 0; y < m_MapHeight; y++)
		{
			for (uint32_t x = 0; x < m_MapWidth; x++)
			{
				char tileType = s_MapTiles[x + y * m_MapWidth];
				auto texture = m_TextureMap[tileType];
				float xPos = x - m_MapWidth / 2.0f, yPos = y - m_MapHeight / 2.0f;
				if (texture)
					Hazle::Renderer2D::DrawQuad({ xPos, -yPos, 0.5f }, glm::radians(0.0f), glm::vec2(1.0f), texture);
				else
					Hazle::Renderer2D::DrawQuad({ xPos, yPos, 0.5f }, glm::radians(0.0f), { 1.0f, 1.0f }, m_SubTexture, glm::vec4(1.0f));
			}
		}
#endif

		if (Input::IsKeyPressed(Key::Escape))
		{
			Application::Get().Close();
		}

		Hazle::Renderer2D::EndScene();
		/*
		m_ParticleSystem.OnUpdate(ts);
		m_ParticleSystem.OnRender(m_CameraController.GetCamera());
		*/
		m_FrameBuffer->Unbind();
	}


	void EditorLayer::OnImGuiRender()
	{
		static bool dockSpaceOpen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (dockSpaceOpen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockSpaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (dockSpaceOpen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Options"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				if (ImGui::MenuItem("Exit"))
				{
					Hazle::Application::Get().close();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::End();

		ImGui::Begin("Settings");

		//ImGui::ColorEdit4("Quad Color", glm::value_ptr(m_QuadColor));

		auto stats = Hazle::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.QuadCount * 4);
		ImGui::Text("Indices: %d", stats.QuadCount * 6);

		auto& squareColor = m_ActiveScene->Reg().get<CSpriteRenderer>(m_SquareEntity).Color;
		ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));

		m_ProfileResults.clear();
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
		{
			m_FrameBuffer->Resize(uint32_t(viewportPanelSize.x), uint32_t(viewportPanelSize.y));
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y);
		}

		HZ_CORE_WARN("Focoused: {0}", m_ViewportFocused);
		HZ_CORE_WARN("Hovered: {0}", m_ViewportHovered);
		uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ viewportPanelSize.x, viewportPanelSize.y }, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::Begin("File Explorer");
		ImGui::BulletText("File 1");
		ImGui::BulletText("File 2");
		ImGui::End();
	}
}